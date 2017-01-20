
#pragma once
#include "../OutputBuffer.h"
class SymbolDatabase;

/** WrapperIFGenerator */
class WrapperIFGenerator
{
public:
	void Generate(SymbolDatabase* database);

private:
	String GenerateOverrideCallerFuncPtrs();
	String GenerateWrapperIFClasses();

	StringA MakeInstanceParamName(TypeInfoPtr info);
	StringA MakeMethods(TypeInfoPtr typeInfo);
	StringA MakeMethod(TypeInfoPtr typeInfo, MethodInfoPtr methodInfo, bool virtualBase);
	StringA MakeCApiParamTypeName(MethodInfoPtr methodInfo, ParameterInfoPtr paramInfo);
	StringA MakeCppTypeName(TypeInfoPtr typeInfo);
	String MakeDocumentComment(DocumentInfoPtr doc);

	SymbolDatabase*	m_database;
};

