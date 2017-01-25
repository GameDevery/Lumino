﻿
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "CSClassLibGenerator.h"
#include "DotNetPInvokeLibGenerator.h"

static const String MethodBodyTemplate =
	_T("{\n")
	_T("    %InitStmt%\n")
	_T("    var result = API.%APIFuncName%(%CallArgList%);\n")
	_T("    if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);\n")
	_T("    %ReturnStmt%\n")
	_T("}\n");

void CSClassLibGenerator::Generate()
{
	OutputBuffer classesText(1);
	OutputBuffer typeInfoRegistersText(3);
	OutputBuffer typeInfoPInvolesText(2);
	for (auto& classInfo : g_database.classes)
	{
		// ランタイムへの TypeInfo 登録コード
		if (!classInfo->IsStatic())
			MakeTypeInfoRegisters(classInfo, &typeInfoRegistersText, &typeInfoPInvolesText);

		// XmlDocument
		classesText.AppendLines(DotNetCommon::MakeXmlDocument(classInfo->document));

		// class header
		String modifier = (classInfo->IsStatic()) ? "static" : "";
		classesText.Append("public {0} class {1}", modifier, classInfo->name).NewLine();
		if (classInfo->baseClass != nullptr) classesText.Append("    : " + classInfo->baseClass->name).NewLine();
		classesText.Append("{").NewLine();
		classesText.IncreaseIndent();

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

		// methods
		for (auto& methodInfo : classInfo->declaredMethods)
		{
			// プロパティの場合はスキップ
			if (methodInfo->ownerProperty != nullptr) continue;

			// params
			OutputBuffer params;
			for (auto& paramInfo : methodInfo->parameters)
			{
				params.AppendCommad("{0} {1}", DotNetCommon::MakeParamTypeName(paramInfo), paramInfo->name);

				// default value
				if (paramInfo->defaultValue != nullptr)
				{
					params.Append(" = 1");
				}
			}

			// xml document
			classesText.AppendLines(DotNetCommon::MakeXmlDocument(methodInfo->document));

			// method header
			String modifier = MethodInfo::GetAccessLevelName(methodInfo->accessLevel);
			if (methodInfo->isStatic) modifier += " static";
			if (methodInfo->isVirtual) modifier += " virtual";
			if (methodInfo->isConstructor)
			{
				classesText.Append("{0} {1}({2}) : base(_LNInternal.InternalBlock)", modifier, classInfo->name, params.ToString()).NewLine();
			}
			else
			{
				classesText.Append("{0} {1} {2}({3})", modifier, DotNetCommon::MakeTypeName(methodInfo->returnType), methodInfo->name, params.ToString()).NewLine();
			}

			// method body
			classesText.AppendLines(MakeMethodBody(methodInfo, false)).NewLine();

			// override callback
			if (methodInfo->isVirtual)
			{
				classesText.AppendLines(MakeOverrideCallbackMethodBody(methodInfo)).NewLine();
				typeInfoRegistersText.Append("API.LN{0}_{1}_SetOverrideCaller({0}.{1}_OverrideCallback);", classInfo->name, methodInfo->name);
				
			}
		}

		// internal constructor
		if (!classInfo->IsStatic())
			classesText.Append("internal {0}(_LNInternal i) : base(i) {{}}", classInfo->name).NewLine(2);

		classesText.DecreaseIndent();
		classesText.Append("}").NewLine();
	}

	// output
	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "DotNet/CSClasses.template.cs"));
		src = src.Replace("%Classes%", classesText.ToString());
		src = src.Replace("%TypeInfoRegisters%", typeInfoRegistersText.ToString());
		src = src.Replace("%TypeInfoPInvoke%", typeInfoPInvolesText.ToString());
		FileSystem::WriteAllText(PathName(g_csOutputDir, "CSClasses.generated.cs"), src);
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
				initStmt.Append("{0} {1} = new {0}();", DotNetCommon::MakePInvokeTypeName(paramInfo->type), paramInfo->name);
			else
				initStmt.Append("{0} {1};", DotNetCommon::MakePInvokeTypeName(paramInfo->type), paramInfo->name);
		}

		// call args
		if (paramInfo->isThis)
			callArgs.AppendCommad("Handle");
		else if (paramInfo->isOut || paramInfo->isReturn)
			callArgs.AppendCommad("out " + paramInfo->name);
		else if (paramInfo->type->IsClass())
			callArgs.AppendCommad(((isProperty) ? "value" : paramInfo->name) + ".Handle");
		else
			callArgs.AppendCommad((isProperty) ? "value" : paramInfo->name);

		// return stmt
		if (paramInfo->isReturn)
		{
			if (methodInfo->isConstructor)	// クラスコンストラクタの場合は Manager 登録を行う
				returnStmt = String::Format("InternalManager.RegisterWrapperObject(this, {0}); API.LNObject_Release({0});", paramInfo->name);
			else
				returnStmt = String::Format("return {0};", paramInfo->name);
		}
	}

	// method body
	String suffix = (methodInfo->isVirtual) ? "_CallVirtualBase" : "";
	methodBody.AppendLines(MethodBodyTemplate
		.Replace("%InitStmt%", initStmt.ToString())
		.Replace("%APIFuncName%", methodInfo->GetCAPIFuncName() + suffix)
		.Replace("%CallArgList%", callArgs.ToString())
		.Replace("%ReturnStmt%", returnStmt));

	return methodBody.ToString();
}

String CSClassLibGenerator::MakeOverrideCallbackMethodBody(MethodInfoPtr methodInfo)
{
	static const String MethodTemplate =
		"internal static ResultCode %Method%_OverrideCallback(%CApiParams%)\n"
		"{\n"
		"	var obj = InternalManager.GetWrapperObject<%Class%>(gamescene);\n"
		"	try\n"
		"	{\n"
		"		obj.%Method%(%Args%);\n"
		"	}\n"
		"	catch (Exception)\n"
		"	{\n"
		"		return ResultCode.ErrorUnknown; \n"
		"	}\n"
		"	return ResultCode.OK;\n"
		"}\n";

	return MethodTemplate
		.Replace("%Method%", methodInfo->name)
		.Replace("%CApiParams%", DotNetPInvokeLibGenerator::MakePInvokeMethodDeclParamList(methodInfo))
		.Replace("%Class%", methodInfo->owner->name)
		.Replace("%Args%", "");

	//OutputBuffer method;
	//method.Append("private static ResultCode {0}_OverrideCallback({1})", methodInfo->name, DotNetPInvokeLibGenerator::MakePInvokeMethodDeclParamList(methodInfo)).NewLine();
	//method.Append("{").NewLine();
	//method.Append("}").NewLine();
	//return method.ToString();
}

void CSClassLibGenerator::MakeTypeInfoRegisters(TypeInfoPtr classInfo, OutputBuffer* typeInfoRegistersText, OutputBuffer* typeInfoPInvolesText)
{
	// ファクトリ
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

