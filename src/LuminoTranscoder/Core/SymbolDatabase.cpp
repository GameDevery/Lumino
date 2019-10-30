﻿
#include "SymbolDatabase.hpp"

ln::Ref<TypeSymbol>	PredefinedTypes::voidType;
ln::Ref<TypeSymbol>	PredefinedTypes::nullptrType;
ln::Ref<TypeSymbol>	PredefinedTypes::boolType;
ln::Ref<TypeSymbol>	PredefinedTypes::intType;
ln::Ref<TypeSymbol>	PredefinedTypes::int16Type;
ln::Ref<TypeSymbol>	PredefinedTypes::uint32Type;
ln::Ref<TypeSymbol>	PredefinedTypes::floatType;
ln::Ref<TypeSymbol>	PredefinedTypes::stringType;
ln::Ref<TypeSymbol>	PredefinedTypes::stringRefType;
ln::Ref<TypeSymbol>	PredefinedTypes::objectType;
ln::Ref<TypeSymbol>	PredefinedTypes::EventConnectionType;

//==============================================================================
// DocumentInfo

ln::Result ParameterDocumentInfo::init(PIParamDocument* pi)
{
	LN_CHECK(pi);
	m_pi = pi;
	return true;
}

//==============================================================================
// DocumentInfo

ln::Result DocumentInfo::init(const PIDocument* pi)
{
	LN_CHECK(pi);
	m_pi = pi;

	for (auto& i : m_pi->params) {
		auto& s = ln::makeRef<ParameterDocumentInfo>();
		if (!s->init(i)) return false;
		m_params.add(s);
	}

	return true;
}

//==============================================================================
// MetadataInfo

const ln::String MetadataInfo::OverloadPostfixAttr = u"OverloadPostfix";

ln::Result MetadataInfo::init(PIMetadata* pi)
{
	m_pi = pi;
	if (m_pi) {
		m_name = m_pi->name;
		m_values = m_pi->values;
	}
	return true;
}

bool MetadataInfo::hasKey(const ln::StringRef& key) const
{
	auto itr = m_values.find(key);
	return (itr != m_values.end());
}

const ln::String* MetadataInfo::findValue(const ln::StringRef& key) const
{
	auto itr = m_values.find(key);
	if (itr != m_values.end())
		return &(itr->second);
	else
		return nullptr;
}

ln::String MetadataInfo::getValue(const ln::StringRef& key, const ln::String& defaultValue) const
{
	auto* s = findValue(key);
	if (s)
		return *s;
	else
		return defaultValue;
}


//==============================================================================
// Symbol

Symbol::Symbol(SymbolDatabase* db)
	: m_db(db)
{
}

ln::Result Symbol::init(const PIDocument* document, PIMetadata* metadata)
{
	m_document = ln::makeRef<DocumentInfo>();
	if (!m_document->init(document)) return false;

	m_metadata = ln::makeRef<MetadataInfo>();
	if (!m_metadata->init(metadata)) return false;

	return true;
}

//==============================================================================
// MetadataInfo
//==============================================================================
//
//void MetadataSymbol::AddValue(const ln::String& key, const ln::String& value)
//{
//	values[key] = value;
//}

//==============================================================================
// FieldSymbol

FieldSymbol::FieldSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result FieldSymbol::init(PIField* pi)
{
	if (!Symbol::init(pi->document, pi->metadata)) return false;
	LN_CHECK(pi);
	m_pi = pi;
	return true;
}

ln::Result FieldSymbol::link()
{
	m_type = db()->getTypeSymbol(m_pi->typeRawName);
	if (!m_type) return false;

	return true;
}

//==============================================================================
// ConstantSymbol

ConstantSymbol::ConstantSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result ConstantSymbol::init(TypeSymbol* type, PIConstant* pi)
{
	if (!Symbol::init(pi->document, pi->metadata)) return false;
	LN_CHECK(pi);
	m_pi = pi;
	m_type = type;
	m_value = m_pi->value;
	return true;
}

ln::Result ConstantSymbol::init(TypeSymbol* type, ln::Variant* value)
{
	m_type = type;
	m_value = value;
	return true;
}

ln::Result ConstantSymbol::link()
{
	return true;
}

//==============================================================================
// MethodParameterSymbol

MethodParameterSymbol::MethodParameterSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result MethodParameterSymbol::init(PIMethodParameter* pi)
{
	LN_CHECK(pi);
	m_pi = pi;
	m_name = m_pi->name;
	return true;
}

ln::Result MethodParameterSymbol::init(TypeSymbol* type, const ln::String& name)
{
	LN_CHECK(type);
	LN_CHECK(!name.isEmpty());
	m_type = type;
	m_name = name;
	return true;
}

ln::Result MethodParameterSymbol::link()
{
	if (m_pi) {
		m_type = db()->getTypeSymbol(m_pi->typeRawName);
		if (!m_type) return false;

		// io direction
		if (m_type->isClass()) {
			m_isIn = true;
		}
		else {
			// primitive, struct, enum ...
			if (!m_pi->isConst && m_pi->isPointer) {
				m_isOut = true;
			}
			else {
				m_isIn = true;
			}
		}

		// defaut value
		if (m_pi->defaultValue) {
			if (m_pi->defaultValue->isNumeric()) {
				if (m_type->isEnum()) {
					m_defaultValue = ln::makeRef<ConstantSymbol>(db());
					if (!m_defaultValue->init(m_type, m_pi->defaultValue)) return false;
				}
				else if (m_pi->defaultValue->type() == ln::VariantType::Float ||
					m_pi->defaultValue->type() == ln::VariantType::Double) {
					m_defaultValue = ln::makeRef<ConstantSymbol>(db());
					if (!m_defaultValue->init(PredefinedTypes::floatType, m_pi->defaultValue)) return false;
				}
				else {
					m_defaultValue = ln::makeRef<ConstantSymbol>(db());
					if (!m_defaultValue->init(PredefinedTypes::intType, m_pi->defaultValue)) return false;
				}
			}
			else {
				LN_NOTIMPLEMENTED();
				return false;
			}
		}
	}
	else {
		// 内部的に作成された Param は、あらかじめ型をセットしておく必要がある
		if (LN_REQUIRE(m_type)) return false;
	}
	return true;
}

//==============================================================================
// MethodSymbol

MethodSymbol::MethodSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result MethodSymbol::init(PIMethod* pi, TypeSymbol* ownerType)
{
	if (!Symbol::init(db()->resolveCopyDoc(pi->document), pi->metadata)) return false;
	LN_CHECK(pi);
	LN_CHECK(ownerType);
	m_pi = pi;
	m_accessLevel = m_pi->accessLevelAsEnum();
	m_ownerType = ownerType;
	m_shortName = m_pi->name;
	m_fullName = m_ownerType->fullName() + u"::" + m_shortName;

	for (auto& i : m_pi->parameters) {
		auto s = ln::makeRef<MethodParameterSymbol>(db());
		if (!s->init(i)) return false;
		m_parameters.add(s);
	}

	if (!pi->isStatic && m_shortName == u"init") {
		m_isConstructor = true;
		m_accessLevel = AccessLevel::Public;
	}

	return true;
}

ln::Result MethodSymbol::link()
{
	m_returnType = db()->getTypeSymbol(m_pi->returnTypeRawName);
	if (!m_returnType) return false;

	for (auto& p : m_parameters) {
		if (!p->link()) return false;
	}

	if (!makeFlatParameters()) return false;

	return true;
}

ln::Result MethodSymbol::makeFlatParameters()
{
	// static でなければ、第1引数は this などになる
	if (!isStatic())
	{
		if (m_ownerType->kind() == TypeKind::Struct)
		{
			auto s = ln::makeRef<MethodParameterSymbol>(db());
			if (!s->init(m_ownerType, m_ownerType->shortName().toLower())) return false;
			s->m_isIn = true;
			s->m_isThis = true;
			m_flatParameters.add(s);
		}
		else if (isConstructor())
		{
			// none
		}
		//else if (owner->isDelegate)
		//{
		//	auto info = ln::makeRef<ParameterSymbol>();
		//	info->name = _T("sender");
		//	info->type = PredefinedTypes::objectType;
		//	info->isThis = true;
		//	capiParameters.add(info);
		//}
		else if (m_ownerType->kind() == TypeKind::Class)
		{
			auto s = ln::makeRef<MethodParameterSymbol>(db());
			if (!s->init(m_ownerType, m_ownerType->shortName().toLower())) return false;
			s->m_isIn = true;
			s->m_isThis = true;
			m_flatParameters.add(s);
		}
		else {
			LN_UNREACHABLE();
			return false;
		}
	}

	// params
	for (auto& paramInfo : m_parameters) {
		m_flatParameters.add(paramInfo);
	}

	// return value
	if (m_returnType == PredefinedTypes::voidType || m_returnType == PredefinedTypes::EventConnectionType)
	{
		// "void", "EventConnection" は戻り値扱いしない
	}
	else
	{
		auto s = ln::makeRef<MethodParameterSymbol>(db());
		if (!s->init(m_returnType, u"outReturn")) return false;
		s->m_isIn = false;
		s->m_isOut = true;
		s->m_isReturn = true;
		m_flatParameters.add(s);
	}

	// constructor
	if (isConstructor()) {
		if (m_ownerType->isStruct()) {
		}
		else {
			auto s = ln::makeRef<MethodParameterSymbol>(db());
			if (!s->init(m_ownerType, ln::String::format(_T("out{0}"), m_ownerType->shortName()))) return false;
			s->m_isReturn = true;
			m_flatParameters.add(s);
		}
	}

	// check string decl
	{
		m_hasStringDecl = m_flatParameters.containsIf([](auto& x) { return x->type()->isString(); });
	}

	return true;
}


#if 0
void MethodSymbol::LinkParameters(SymbolDatabase* db)
{
	if (metadata->HasKey(_T("Event")))
	{
		printf("is event");	// TODO
	}

	for (auto& paramInfo : parameters)
	{
		paramInfo->type = db->findSymbol(paramInfo->src.typeRawName);

		// 今のところ、Class 型の 出力変数は許可しない
		if (paramInfo->isOut && paramInfo->type->IsClass())
		{
			paramInfo->isIn = true;
			paramInfo->isOut = false;
		}

		if (paramInfo->src.rawDefaultValue != nullptr)
		{
			ln::String text = paramInfo->src.rawDefaultValue.value();
			if (text.contains(_T("::")))
			{
				auto tokens = text.split(_T("::"));
				ln::Ref<TypeSymbol> dummy;
				db->findEnumTypeAndValue(tokens[0], tokens[1], &dummy, &paramInfo->defaultValue);
			}
			else
			{
				paramInfo->defaultValue = db->createConstantFromLiteralString(text);
			}
		}
	}
}


ln::String MethodSymbol::GetCApiSetOverrideCallbackFuncName()
{
	return GetCAPIFuncName() + "_SetOverrideCaller";
}

ln::String MethodSymbol::GetCApiSetOverrideCallbackTypeName()
{
	return ln::String::format(_T("LN{0}_{1}_OverrideCaller"), owner->shortName(), name);
}

ln::String MethodSymbol::GetAccessLevelName(AccessLevel accessLevel)
{
	switch (accessLevel)
	{
	case AccessLevel::Public:
		return "public";
	case AccessLevel::Protected:
		return "protected";
	case AccessLevel::Private:
		return "private";
	//case AccessLevel::Internal:
	//	return "internal";
	default:
		LN_UNREACHABLE();
		break;
	}
	return "";
}

void MethodSymbol::link(SymbolDatabase* db)
{
	if (!isStatic &&
		name == _T("initialize"))
	{
		isConstructor = true;
	}

	if (owner->isStruct &&
		owner->shortName() == name)
	{
		isConstructor = true;
	}

	ExpandCAPIParameters(db);
}
#endif

//==============================================================================
// TypeSymbol

TypeSymbol::TypeSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result TypeSymbol::init(PITypeInfo* piType)
{
	if (!Symbol::init(piType->document, piType->metadata)) return false;

	LN_CHECK(piType);
	m_piType = piType;
	setFullName(m_piType->rawFullName);

	for (auto& i : m_piType->fields) {
		auto s = ln::makeRef<FieldSymbol>(db());
		if (!s->init(i)) {
			return false;
		}
		m_fields.add(s);
	}

	for (auto& i : m_piType->constants) {
		auto s = ln::makeRef<ConstantSymbol>(db());
		if (!s->init(this, i)) {
			return false;
		}
		m_constants.add(s);
	}

	for (auto& i : m_piType->methods) {
		auto s = ln::makeRef<MethodSymbol>(db());
		if (!s->init(i, this)) {
			return false;
		}
		m_declaredMethods.add(s);
	}

	return true;
}

ln::Result TypeSymbol::init(const ln::String& primitveRawFullName)
{
	setFullName(primitveRawFullName);
	return true;
}

ln::Result TypeSymbol::link()
{
	for (auto& i : m_fields) {
		if (!i->link()) {
			return false;
		}
	}
	for (auto& i : m_constants) {
		if (!i->link()) {
			return false;
		}
	}
	for (auto& i : m_declaredMethods) {
		if (!i->link()) {
			return false;
		}
		if (i->accessLevel() == AccessLevel::Public) {
			m_publicMethods.add(i);
		}
	}

	if (isClass() && !isStatic()) {
		m_baseClass = db()->getTypeSymbol(m_piType->baseClassRawName);
		if (!m_baseClass) {
			LN_NOTIMPLEMENTED();
			return false;
		}
	}

	if (!linkOverload()) return false;

	collectVirtualMethods(&m_virtualMethods);

	return true;
}

void TypeSymbol::setFullName(const ln::String& value)
{
	m_fullName = value;

	int c = value.lastIndexOf(':');
	if (c >= 0)
		m_shortName = value.substr(c + 1);
	else
		m_shortName = value;
}

ln::Result TypeSymbol::linkOverload()
{
	for (auto& method1 : m_declaredMethods)
	{
		auto info = m_overloads.findIf([&method1](auto& x) { return x->m_methods[0]->shortName() == method1->shortName(); });
		if (info) {
			(*info)->m_methods.add(method1);
			method1->m_overloadInfo = (*info);

			//// verify: representative 以外は OverloadPostfix を持っていなければならない
			//if (!method1->metadata()->hasKey(MetadataInfo::OverloadPostfixAttr))
			//{
			//	LN_NOTIMPLEMENTED();
			//	return false;
			//}
		}
		else {
			auto newInfo = ln::makeRef<MethodOverloadInfo>();
			newInfo->m_methods.add(method1);
			m_overloads.add(newInfo);
			method1->m_overloadInfo = newInfo;
		}
	}

	for (auto& overload : m_overloads) {
		for (int i = 0; i < overload->m_methods.size(); i++) {
			// OverloadPostfix を持っていないものを代表として採用する
			if (!overload->m_methods[i]->metadata()->hasKey(MetadataInfo::OverloadPostfixAttr)) {
				if (overload->m_representativeIndex >= 0) {
					// 複数の代表が見つかってしまった
					// verify: representative 以外は OverloadPostfix を持っていなければならない
					LN_NOTIMPLEMENTED();
					return false;
				}
				else {
					overload->m_representativeIndex = i;
				}
			}
		}
		if (overload->m_representativeIndex < 0) {
			// 代表が見つからなかったら 0 番を採用
			overload->m_representativeIndex = 0;
		}
	}


#if 0
	for (auto& method1 : m_methods)
	{
		if (!method1->m_overloadChildren.isEmpty() || method1->m_overloadParent)
			continue;	// processed (method1 is child)

		// find same names
		ln::List<MethodSymbol*> sameNames;
		for (auto& method2 : m_methods)
		{
			//if (isStruct() && method2->isConstructor() && method2->parameters().isEmpty())
			//{
			//	// 構造体の引数無しコンストラクタは Clear という別名にしたい。
			//	continue;
			//}

			if (method1->name() == method2->name())
			{
				sameNames.add(method2);
			}
		}

		// select parent
		auto f = sameNames.findIf([](MethodSymbol* m) { return !m->metadata()->hasKey(MetadataInfo::OverloadPostfixAttr); });
		if (!f) {
			LN_NOTIMPLEMENTED();
			return false;
		}
		MethodSymbol* parent = *f;

		// link
		for (auto& method2 : sameNames)
		{
			if (method2 == parent) continue;

			if (!method2->metadata()->hasKey(MetadataInfo::OverloadPostfixAttr))
			{
				//db->diag();
				LN_NOTIMPLEMENTED();
				return false;
			}

			parent->m_overloadChildren.add(method2);
			method2->m_overloadParent = parent;
		}
	}
#endif

	return true;
}

void TypeSymbol::collectVirtualMethods(ln::List<Ref<MethodSymbol>>* virtualMethods)
{
	if (m_baseClass) {
		m_baseClass->collectVirtualMethods(virtualMethods);
	}

	for (auto& method : m_declaredMethods) {
		if (method->isVirtual()) {
			int index = virtualMethods->indexOfIf([&](auto& x) { return x->shortName() == method->shortName(); });
			if (index >= 0) {
				virtualMethods->at(index) = method;	// override
			}
			else {
				virtualMethods->add(method);
			}
		}
	}
}

#if 0
void TypeSymbol::Link(SymbolDatabase* db)
{
	// find base class
	if (!src.baseClassRawName.isEmpty())
	{
		baseClass = db->findSymbol(src.baseClassRawName);
	}

	for (auto methodInfo : declaredMethods)
	{
		methodInfo->link(db);
	}

	MakeProperties();
	LinkOverload(db);
	ResolveCopyDoc();

}


void TypeSymbol::MakeProperties()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->metadata->HasKey(_T("Property")))
		{
			ln::String name;
			ln::String namePrefix;
			bool isGetter = false;
			if (methodInfo->name.indexOf(_T("get"), 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substr(3);
				isGetter = true;
			}
			else if (methodInfo->name.indexOf(_T("is"), 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substr(2);
				namePrefix = _T("is");
				isGetter = true;
			}
			else if (methodInfo->name.indexOf(_T("set"), 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substr(3);
				isGetter = false;
			}
			else
			{
				// 上記以外 (longName() など名詞系) は getter とする
				name = methodInfo->name;
				isGetter = true;
			}

			ln::Ref<PropertySymbol> propInfo;
			{
				auto ptr = declaredProperties.findIf([name](auto info) { return info->name == name; });
				if (ptr)
				{
					propInfo = ln::makeRef<PropertySymbol>();
					propInfo->name = name;
					declaredProperties.add(propInfo);
				}
				else
				{
					propInfo = *ptr;
				}
				propInfo->namePrefix = namePrefix;
			}

			if (isGetter)
			{
				LN_DCHECK(propInfo->getter == nullptr);
				propInfo->getter = methodInfo;
				if (propInfo->type == nullptr)	// return 型をプロパティの型とする
					propInfo->type = methodInfo->returnType;
			}
			else
			{
				LN_DCHECK(propInfo->setter == nullptr);
				propInfo->setter = methodInfo;
				if (propInfo->type == nullptr)	// 第1引数の型をプロパティの型とする
					propInfo->type = methodInfo->parameters[0]->type;
			}

			methodInfo->ownerProperty = propInfo;
		}
	}

	// create document
	for (auto& propInfo : declaredProperties)
	{
		propInfo->MakeDocument();
	}
}



void TypeSymbol::ResolveCopyDoc()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->document != nullptr && methodInfo->document->IsCopyDoc())
		{
			auto e = stream::MakeStream::from(declaredMethods)
				| stream::op::concat(stream::MakeStream::from(declaredMethodsForDocument));
			for (auto& methodInfo2 : e)
			{
				if (methodInfo2->name == methodInfo->document->copydocMethodName &&
					methodInfo2->paramsRawSignature == methodInfo->document->copydocSignature)
				{
					methodInfo->document = methodInfo2->document;
					break;
				}
			}
		}
	}
}
#endif

//==============================================================================
// PropertySymbol
//void PropertySymbol::MakeDocument()
//{
//	document = ln::makeRef<DocumentSymbol>();
//
//	if (getter != nullptr)
//	{
//		document->summary += getter->document->summary;
//		document->details += getter->document->details;
//	}
//	if (setter != nullptr)
//	{
//		if (!document->summary.isEmpty()) document->summary += _T("\n");
//		if (!document->details.isEmpty()) document->details += _T("\n\n");
//
//		document->summary += setter->document->summary;
//		document->details += setter->document->details;
//	}
//}


//==============================================================================
// DelegateSymbol

DelegateSymbol::DelegateSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result DelegateSymbol::init(PIDelegate* piDelegate)
{
	if (!Symbol::init(piDelegate->document, nullptr)) {
		return false;
	}

	m_pi = piDelegate;

	m_shortName = m_pi->name;

	for (auto& i : m_pi->parameters) {
		auto s = ln::makeRef<MethodParameterSymbol>(db());
		if (!s->init(i)) return false;
		m_parameters.add(s);
	}

	return true;
}

ln::Result DelegateSymbol::link()
{
	m_returnType = db()->getTypeSymbol(m_pi->returnTypeRawName);
	if (!m_returnType) return false;

	for (auto& p : m_parameters) {
		if (!p->link()) return false;
	}

	for (auto& paramInfo : m_parameters) {
		m_flatParameters.add(paramInfo);
	}

	return true;
}

//==============================================================================
// SymbolDatabase

SymbolDatabase::SymbolDatabase(ln::DiagnosticsManager* diag)
	: m_diag(diag)
{
}

ln::Result SymbolDatabase::initTypes(PIDatabase* pidb)
{
	LN_CHECK(pidb);
	m_pidb = pidb;
	initPredefineds();

	for (auto& t : m_pidb->types) {
		auto symbol = ln::makeRef<TypeSymbol>(this);
		if (!symbol->init(t)) {
			return false;
		}
		m_allTypes.add(symbol);
	}

	for (auto& d : m_pidb->delegates) {
		auto symbol = ln::makeRef<DelegateSymbol>(this);
		if (!symbol->init(d)) {
			return false;
		}
		m_delegates.add(symbol);
	}

	return true;
}

ln::Result SymbolDatabase::linkTypes()
{
	for (auto& t : m_allTypes) {
		if (!t->link()) return false;
	}
	for (auto& t : m_delegates) {
		if (!t->link()) return false;
	}

	return true;
#if 0
	// structs
	for (auto structInfo : structs)
	{
		for (auto fieldInfo : structInfo->declaredFields)
		{
			fieldInfo->type = findSymbol(fieldInfo->typeRawName);
		}
		for (auto methodInfo : structInfo->declaredMethods)
		{
			methodInfo->returnType = findSymbol(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
		}

		structInfo->Link(this);
	}

	// classes
	for (auto classInfo : classes)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = findSymbol(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
		}

		classInfo->Link(this);

		// ドキュメントとしてみるときのために、コンストラクタを先頭に出すようにする
		std::stable_sort(classInfo->declaredMethods.begin(), classInfo->declaredMethods.end(), [](MethodSymbol* lhs, MethodSymbol* rhs)
		{
			int lv = lhs->isConstructor ? 0 : 1;
			int rv = rhs->isConstructor ? 0 : 1;
			return lv < rv;
		});
	}

	// enums
	for (auto enumInfo : enums)
	{
		enumInfo->isEnum = true;
	}

	// delegates
	for (auto classInfo : delegates)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = findSymbol(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
		}

		classInfo->Link(this);
	}
#endif
}

//tr::Enumerator<ln::Ref<MethodSymbol>> SymbolDatabase::GetAllMethods()
//{
//	ln::List<ln::Ref<MethodSymbol>> dummy;
//	auto e = tr::MakeEnumerator::from(dummy);
//
//	for (auto& structInfo : structs)
//	{
//		e.Join(structInfo->declaredMethods);
//	}
//	for (auto& classInfo : classes)
//	{
//		e.Join(classInfo->declaredMethods);
//	}
//
//	return e;
//}

//void SymbolDatabase::FindEnumTypeAndValue(const ln::String& typeFullName, const ln::String& memberName, ln::Ref<TypeSymbol>* outEnum, ln::Ref<ConstantSymbol>* outMember)
//{
//	for (auto& enumInfo : enums)
//	{
//		if (enumInfo->fullName() == typeFullName)
//		{
//			for (auto& constantInfo : enumInfo->declaredConstants)
//			{
//				if (constantInfo->name == memberName)
//				{
//					*outEnum = enumInfo;
//					*outMember = constantInfo;
//					return;
//				}
//			}
//		}
//	}
//
//	LN_ENSURE(0, "Undefined enum: %s::%s", typeFullName.c_str(), memberName.c_str());
//}

//ln::Ref<ConstantSymbol> SymbolDatabase::createConstantFromLiteralString(const ln::String& valueStr)
//{
//	auto info = ln::makeRef<ConstantSymbol>();
//	if (valueStr == "true")
//	{
//		info->type = PredefinedTypes::boolType;
//		info->value = ln::makeVariant(true);
//	}
//	else if (valueStr == "false")
//	{
//		info->type = PredefinedTypes::boolType;
//		info->value = ln::makeVariant(false);
//	}
//	else if (valueStr == "nullptr")
//	{
//		info->type = PredefinedTypes::nullptrType;
//		info->value = ln::makeVariant(nullptr);
//	}
//	else if (valueStr.contains('.'))
//	{
//		info->type = PredefinedTypes::floatType;
//		info->value = ln::makeVariant(ln::StringHelper::toFloat(valueStr.c_str()));
//	}
//	else
//	{
//		info->type = PredefinedTypes::intType;
//		info->value = ln::makeVariant(ln::StringHelper::toInt32(valueStr.c_str()));
//	}
//
//	return info;
//}

const PIDocument* SymbolDatabase::resolveCopyDoc(const PIDocument* pi) const
{
	if (pi->copydocLocalSignature.isEmpty())
		return pi;
	else
		return m_pidb->findRelativeDocument(pi->copydocLocalSignature);
}

void SymbolDatabase::initPredefineds()
{
	auto addPredefined = [this](const ln::String& name) {
		auto t = ln::makeRef<TypeSymbol>(this);
		t->init(name);
		m_allTypes.add(t);
		return t;
	};
		
	PredefinedTypes::voidType = addPredefined(u"void");
	
	PredefinedTypes::nullptrType = addPredefined(u"nullptr_t");

	PredefinedTypes::boolType = addPredefined(u"bool");

	PredefinedTypes::intType = addPredefined(u"int");

	PredefinedTypes::int16Type = addPredefined(u"int16_t");

	PredefinedTypes::uint32Type = addPredefined(u"uint32_t");

	PredefinedTypes::floatType = addPredefined(u"float");

	PredefinedTypes::stringType = addPredefined(u"ln::String");

	PredefinedTypes::stringType = addPredefined(u"ln::StringRef");

	PredefinedTypes::objectType = addPredefined(u"ln::Object");

	//m_allTypes.add(ln::makeRef<TypeSymbol>(_T("ln::EventConnection")));
	//PredefinedTypes::EventConnectionType = m_allTypes.back();
}

TypeSymbol* SymbolDatabase::findTypeSymbol(const ln::String& typeFullName)
{
	auto type = m_allTypes.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type)
		return *type;
	else
		return nullptr;
}

TypeSymbol* SymbolDatabase::getTypeSymbol(const ln::String& typeFullName)
{
	auto type = findTypeSymbol(typeFullName);
	if (!type)
		m_diag->reportError(u"Undefined type : " + typeFullName);
	return type;
}

// typeFullName : const や &, * は除かれていること
//ln::Ref<TypeSymbol> SymbolDatabase::findSymbol(ln::StringRef typeFullName)
//{
//	ln::Optional<ln::Ref<TypeSymbol>> type;
//	
//	type = predefineds.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
//	if (type != nullptr) return *type;
//
//	type = structs.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
//	if (type != nullptr) return *type;
//
//	type = classes.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
//	if (type != nullptr) return *type;
//
//	type = enums.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
//	if (type != nullptr) return *type;
//
//	type = delegates.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
//	if (type)
//		return *type;
//
//	// aliases
//	if (typeFullName == _T("short")) return PredefinedTypes::int16Type;
//	if (typeFullName == _T("ln::ln::StringRef")) return PredefinedTypes::stringType;
//	if (typeFullName == _T("ln::EventConnection")) return PredefinedTypes::EventConnectionType;
//
//	LN_ENSURE(0, "Undefined type: %s", ln::String(typeFullName).c_str());
//	return nullptr;
//}

//void SymbolDatabase::verify(DiagManager* diag)
//{
//}


