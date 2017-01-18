
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "CSClassLibGenerator.h"

static const String MethodBodyTemplate =
	_T("{\n")
	_T("    %InitStmt%\n")
	_T("    var result = API.%APIFuncName%(%CallArgList%);\n")
	_T("    if (result != ResultCode.Ok) throw LuminoException.MakeExceptionFromLastError(result);\n")
	_T("    %ReturnStmt%\n")
	_T("}\n");

void CSClassLibGenerator::Generate()
{
	OutputBuffer classesText(1);
	OutputBuffer typeInfoRegistersText(3);
	OutputBuffer typeInfoPInvolesText(2);
	for (auto& classInfo : g_database.classes)
	{
		// TypeInfo
		if (!classInfo->IsStatic())
			MakeTypeInfoRegisters(classInfo, &typeInfoRegistersText, &typeInfoPInvolesText);

		// XmlDocument
		classesText.AppendLines(DotNetCommon::MakeXmlDocument(classInfo->document));

		// class header
		String modifier = (classInfo->IsStatic()) ? "static" : "";
		classesText.Append("public {0} class {1}", modifier, classInfo->name).NewLine();
		classesText.Append("{").NewLine();
		classesText.IncreaseIndent();

		// methods
		for (auto& methodInfo : classInfo->declaredMethods)
		{
			// �v���p�e�B�̏ꍇ�̓X�L�b�v
			if (methodInfo->ownerProperty != nullptr) continue;

			// params
			OutputBuffer params;
			for (auto& paramInfo : methodInfo->parameters)
			{
				params.AppendCommad("{0} {1}", DotNetCommon::MakeParamTypeName(paramInfo), paramInfo->name);
			}

			// xml document
			classesText.AppendLines(DotNetCommon::MakeXmlDocument(methodInfo->document));

			// method header
			String modifier = (methodInfo->isStatic) ? "static" : "";
			String returnType = (!methodInfo->isConstructor) ? DotNetCommon::MakeTypeName(methodInfo->returnType) : "";
			classesText.Append("public {0} {1} {2}({3})", modifier, returnType, methodInfo->name, params.ToString()).NewLine();

			// method body
			classesText.AppendLines(MakeMethodBody(methodInfo, false)).NewLine();
		}

		// property
		for (auto& propInfo : classInfo->declaredProperties)
		{
			OutputBuffer prop;

			// xml document
			classesText.AppendLines(DotNetCommon::MakeXmlDocument(propInfo->document));

			// property header
			classesText.Append("public {0} {1}", DotNetCommon::MakeTypeName(propInfo->type), propInfo->name).NewLine();
			classesText.Append("{").NewLine();
			classesText.IncreaseIndent();

			if (propInfo->getter != nullptr)
			{
				classesText.Append("get").NewLine();
				classesText.AppendLines(MakeMethodBody(propInfo->getter, true));
			}
			if (propInfo->setter != nullptr)
			{
				classesText.Append("set").NewLine();
				classesText.AppendLines(MakeMethodBody(propInfo->setter, true));
			}

			classesText.DecreaseIndent();
			classesText.Append("}").NewLine(2);
		}

		classesText.DecreaseIndent();
		classesText.Append("}").NewLine();
	}

	// output
	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "DotNet/CSClasses.template.cs"));
		src = src.Replace("%Classes%", classesText.ToString());
		src = src.Replace("%TypeInfoRegisters%", typeInfoRegistersText.ToString());
		src = src.Replace("%TypeInfoPInvoke%", typeInfoPInvolesText.ToString());
		FileSystem::WriteAllText(LUMINO_ROOT_DIR"/CSClasses.generated.cs", src);
	}
}

String CSClassLibGenerator::MakeMethodBody(MethodInfoPtr methodInfo, bool isProperty)
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
				initStmt.Append("{0} {1} = new {0}();", DotNetCommon::MakeTypeName(paramInfo->type), paramInfo->name);
			else
				initStmt.Append("{0} {1};", DotNetCommon::MakeTypeName(paramInfo->type), paramInfo->name);
		}

		// call args
		if (paramInfo->isThis)
			callArgs.AppendCommad("ref this");
		else if (paramInfo->isOut || paramInfo->isReturn)
			callArgs.AppendCommad("out " + paramInfo->name);
		else
			callArgs.AppendCommad((isProperty) ? "value" : paramInfo->name);

		// return stmt
		if (paramInfo->isReturn)
			returnStmt = String::Format("return {0};", paramInfo->name);
	}

	// method body
	methodBody.AppendLines(MethodBodyTemplate
		.Replace("%InitStmt%", initStmt.ToString())
		.Replace("%APIFuncName%", methodInfo->GetCAPIFuncName())
		.Replace("%CallArgList%", callArgs.ToString())
		.Replace("%ReturnStmt%", returnStmt));

	return methodBody.ToString();
}

void CSClassLibGenerator::MakeTypeInfoRegisters(TypeInfoPtr classInfo, OutputBuffer* typeInfoRegistersText, OutputBuffer* typeInfoPInvolesText)
{
	// �t�@�N�g��
	static const String template1 =
		"var _%Class% = new TypeInfo(){ Factory = (handle) =>\n"
		"{\n"
		"    var obj = new %Class%(_LNInternal.InternalBlock);\n"
		"    obj.SetHandle(handle);\n"
		"    return obj;\n"
		"}};\n"
		"_typeInfos.Add(_%Class%);\n"
		"LN%Class%_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));\n";
	typeInfoRegistersText->AppendLines(template1.Replace("%Class%", classInfo->name)).NewLine();

	// P/Invoke
	typeInfoPInvolesText->AppendLine("[DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]");
	typeInfoPInvolesText->AppendLine("private static extern void LN{0}_SetBindingTypeInfo(IntPtr data);", classInfo->name).NewLine();
}

