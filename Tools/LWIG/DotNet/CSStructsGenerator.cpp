
#include "../SymbolDatabase.h"
#include "CSCommon.h"
#include "CSStructsGenerator.h"

static const String MethodBodyTemplate =
	_T("{\n")
	_T("    %InitStmt%\n")
	_T("    var result = API.%APIFuncName%(%CallArgList%);\n")
	_T("    if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);\n")
	_T("    %ReturnStmt%\n")
	_T("}\n");

void CSStructsGenerator::generate()
{
	OutputBuffer structText(1);
	for (auto& structInfo : db()->structs)
	{
		structText.AppendLines(CSCommon::MakeXmlDocument(structInfo->document));
		structText.append("[StructLayout(LayoutKind.Sequential)]").NewLine();
		structText.append("public struct {0}", structInfo->shortName()).NewLine();
		structText.append("{").NewLine();
		structText.IncreaseIndent();

		// fields
		for (auto& fieldInfo : structInfo->declaredFields)
		{
			structText.AppendLines(CSCommon::MakeXmlDocument(fieldInfo->document));
			structText.append("public {0} {1};", CSCommon::MakeTypeName(fieldInfo->type), fieldInfo->name.toTitleCase()).NewLine(2);
		}

		// �e�v�f�w��̃R���X�g���N�^�����
		{
			// ���������X�g �� (float x, float y) ��
			OutputBuffer params;
			for (auto& fieldInfo : structInfo->declaredFields)
				params.AppendCommad("{0} {1}", CSCommon::MakeTypeName(fieldInfo->type), fieldInfo->name);

			// �t�B�[���h�i�[ ����� �� X = x; Y = y; ��
			OutputBuffer body;
			for (auto& fieldInfo : structInfo->declaredFields)
				body.append("{0} = {1};", fieldInfo->name.toTitleCase(), fieldInfo->name).NewLine();

			// ���\�b�h��`
			structText.append("public {0}({1})", structInfo->shortName(), params.toString()).NewLine();
			structText.append("{").NewLine();
			structText.IncreaseIndent();
			structText.AppendLines(body.toString());
			structText.DecreaseIndent();
			structText.append("}").NewLine(2);
		}

		// methods
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			if (methodInfo->ownerProperty != nullptr) continue;								// �v���p�e�B�̏ꍇ�̓X�L�b�v
			if (methodInfo->isConstructor && methodInfo->parameters.isEmpty()) continue;	// C# �͍\���̂Ƀf�t�H���g�R���X�g���N�^�͒�`�ł��Ȃ�
			if (CheckAllMemberSetConstructor(structInfo, methodInfo)) continue;				// �P�ɑS�����o�� set ���邾���̃R���X�g���N�^�͕ʓr��`�����

			// params
			OutputBuffer params;
			for (auto& paramInfo : methodInfo->parameters)
			{
				params.AppendCommad("{0} {1}", CSCommon::MakeParamTypeName(paramInfo), paramInfo->name);
			}

			// xml document
			structText.AppendLines(CSCommon::MakeXmlDocument(methodInfo->document));

			// method header
			String modifier = MethodSymbol::GetAccessLevelName(methodInfo->accessLevel);
			if (methodInfo->isStatic) modifier += " static";
			if (methodInfo->isVirtual) modifier += " virtual";
			if (methodInfo->isConstructor)
				structText.append("{0} {1}({2})", modifier, methodInfo->name, params.toString()).NewLine();
			else
				structText.append("{0} {1} {2}({3})", modifier, CSCommon::MakeTypeName(methodInfo->returnType), methodInfo->name, params.toString()).NewLine();

			// method body
			structText.AppendLines(MakeMethodBody(methodInfo, false)).NewLine();
		}

		// property
		for (auto& propInfo : structInfo->declaredProperties)
		{
			OutputBuffer prop;

			// xml document
			structText.AppendLines(CSCommon::MakeXmlDocument(propInfo->document));

			// property header
			structText.append("public {0} {1}", CSCommon::MakeTypeName(propInfo->type), propInfo->name).NewLine();
			structText.append("{").NewLine();
			structText.IncreaseIndent();

			if (propInfo->getter != nullptr)
			{
				structText.append("get").NewLine();
				structText.AppendLines(MakeMethodBody(propInfo->getter, false));
			}
			if (propInfo->setter != nullptr)
			{
				structText.append("set").NewLine();
				structText.AppendLines(MakeMethodBody(propInfo->setter, true));
			}

			structText.DecreaseIndent();
			structText.append("}").NewLine(2);
		}

		structText.DecreaseIndent();
		structText.append("}").NewLine();
	}

	// output
	{
		String src = FileSystem::readAllText(makeTemplateFilePath("CSStructs.template.cs"));
		src = src.replace("%Structs%", structText.toString());
		FileSystem::writeAllText(makeOutputFilePath("CSStructs.generated.cs").c_str(), src);
	}
}

String CSStructsGenerator::MakeMethodBody(Ref<MethodSymbol> methodInfo, bool isSetProperty)
{
	OutputBuffer methodBody;

	// body
	OutputBuffer initStmt;
	OutputBuffer callArgs;
	String returnStmt;
	for (auto& paramInfo : methodInfo->capiParameters)
	{
		// init stmt
		if (paramInfo->isOut || paramInfo->isReturn)
		{
			if (paramInfo->type->isStruct)
				initStmt.append("{0} {1} = new {0}();", CSCommon::MakeTypeName(paramInfo->type), paramInfo->name);
			else
				initStmt.append("{0} {1};", CSCommon::MakeTypeName(paramInfo->type), paramInfo->name);
		}

		// call args
		String modifier = "";
		if (paramInfo->isThis || paramInfo->type->isStruct) modifier = "ref ";
		if (paramInfo->isOut || paramInfo->isReturn) modifier = "out ";
		String name = paramInfo->name;
		if (isSetProperty) name = "value";
		if (paramInfo->isThis) name = "this";
		callArgs.AppendCommad(modifier + name);

		// return stmt
		if (paramInfo->isReturn)
			returnStmt = String::format("return {0};", paramInfo->name);
	}

	// method body
	methodBody.AppendLines(MethodBodyTemplate
		.replace("%InitStmt%", initStmt.toString())
		.replace("%APIFuncName%", methodInfo->GetCAPIFuncName())
		.replace("%CallArgList%", callArgs.toString())
		.replace("%ReturnStmt%", returnStmt));

	return methodBody.toString();
}

bool CSStructsGenerator::CheckAllMemberSetConstructor(Ref<TypeSymbol> structInfo, Ref<MethodSymbol> methodInfo)
{
	if (!methodInfo->isConstructor) return false;

	int count = methodInfo->parameters.getCount();
	if (count != structInfo->declaredFields.getCount()) return false;

	for (int i = 0; i < count; i++)
	{
		if (methodInfo->parameters[i]->type != structInfo->declaredFields[i]->type)
			return false;
	}

	return true;
}
