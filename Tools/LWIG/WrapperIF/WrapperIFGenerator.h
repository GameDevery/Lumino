
#pragma once
#include "../Generator.h"
class SymbolDatabase;

/** C����̗p�� API ���o�͂��� */
class WrapperIFGenerator
	: public Generator
{
public:
	WrapperIFGenerator();
	void generate();


private:
	String GenerateOverrideCallerFuncPtrs();

	String MakeInstanceParamName(Ref<TypeSymbol> info);
	//String MakeMethods(Ref<TypeSymbol> typeInfo);
	String MakeFuncHeader(Ref<MethodSymbol> methodInfo);
	String MakeFuncBody(Ref<TypeSymbol> typeInfo, Ref<MethodSymbol> methodInfo);

	String MakeDocumentComment(Ref<DocumentSymbol> doc);


};

