
#include "Parser.h"
#include <Lumino/Base/Enumerable.h>
using namespace fl;



HeaderParser::HeaderParser(SymbolDatabase* database)
	: m_database(database)
{
}

void HeaderParser::ParseFiles(const List<PathName>& pathes)
{
	for (auto& path : pathes)
	{
		ParseFile(path);
	}
}

void HeaderParser::ParseFile(const PathName& path)
{
	/*
	�L�[���[�h�� { } ; �ɒ��ڂ��Đ؂蕪����p�[�T�B
	�� LN_CLASS() �� ( ) ���p�[�X�ł����������A�_���B���̊O���̑S�Ă� ( ) ���l������K�v������B����͂����ƌ�i�ŁB
	*/
	struct DeclsParser : public combinators::ParseLib<DeclsParser>
	{

		static ParserResult<Decl> Parse_EmptyDecl(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar(';'));
			return input.Success(Decl{ _T("EmptyDecl"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LocalBlock(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenChar('{'));
			LN_PARSE_RESULT(r2, Many(Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r3, TokenChar('}'));
			return input.Success(Decl{ _T("LocalBlock"), r1.GetMatchBegin(), r3.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LN_STRUCT(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_STRUCT"));
			LN_PARSE_RESULT(r2, TokenChar('{'));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_FIELD) || Parser<Decl>(Parse_LN_FUNCTION) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("Struct"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
		}

		static ParserResult<Decl> Parse_LN_CLASS(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_CLASS"));
			LN_PARSE_RESULT(r2, TokenChar('{'));
			LN_PARSE_RESULT(r3, Many(Parser<Decl>(Parse_LN_FUNCTION) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_DocumentComment)));
			LN_PARSE_RESULT(r4, TokenChar('}'));
			return input.Success(Decl{ _T("class"), r1.GetMatchBegin(), r4.GetMatchEnd(), r3.GetValue() });
		}

		static ParserResult<Decl> Parse_LN_FIELD(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_FIELD"));
			LN_PARSE_RESULT(r2, UntilMore(Parser<Decl>(Parse_EmptyDecl)));
			return input.Success(Decl{ _T("field"), r1.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_LN_FUNCTION(ParserContext input)
		{
			LN_PARSE_RESULT(r1, TokenString("LN_FUNCTION"));
			LN_PARSE_RESULT(r2, UntilMore(Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock)));
			return input.Success(Decl{ _T("method"), r1.GetMatchBegin(), r2.GetMatchEnd() });
		}

		static ParserResult<Decl> Parse_DocumentComment(ParserContext input)
		{
			LN_PARSE_RESULT(r1, Token(TokenGroup::Comment));
			return input.Success(Decl{ _T("DocumentComment"), r1.GetMatchBegin(), r1.GetMatchEnd() });
		}

		static ParserResult<List<Decl>> Parse_File(ParserContext input)
		{
			LN_PARSE(r1, Many(Parser<Decl>(Parse_LN_CLASS) || Parser<Decl>(Parse_LN_STRUCT) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
			return input.Success(r1);
		}

		static bool FilterToken(fl::Token* token)
		{
			if (token->GetTokenGroup() == TokenGroup::Comment &&
				StringTraits::Compare(token->GetBegin(), "/**", 3) == 0)
			{
				return true;
			}
			return
				token->EqualChar(';') || token->EqualChar('{') || token->EqualChar('}') ||
				token->EqualString("LN_STRUCT", 9) ||
				token->EqualString("LN_CLASS", 8) ||
				token->EqualString("LN_FIELD", 8) ||
				token->EqualString("LN_FUNCTION", 11) ||
				token->GetTokenGroup() == TokenGroup::Eof;	// TODO: ���ꂪ�����Ă������悤�ɂ������B���͂��ꂪ�Ȃ��ƁAMany����EOF�����Ƃ�OutOfRange����
		}
	};


	auto codeStr = FileSystem::ReadAllText(path);
	StringA code = codeStr.ToStringA();

	fl::AnalyzerContext ctx;
	auto file = ctx.RegisterInputMemoryCode("memory", code.c_str());
	ctx.LexFile(file);
	auto tokens = file->GetTokenList();

	auto result = DeclsParser::TryParse(DeclsParser::Parse_File, tokens);
	for (auto decl1 : result.GetValue())
	{
		//Console::WriteLine(decl1.type);

		if (decl1.type == "Struct") ParseStructDecl(decl1);
		if (decl1.type == "class") ParseClassDecl(decl1);

		for (auto decl2 : decl1.decls)
		{
			//Console::WriteLine("  {0}", decl2.type);
		}
	}
}

HeaderParser::TokenItr HeaderParser::ParseMetadataDecl(TokenItr begin, TokenItr end)
{
	// (...) ��T��
	auto paramBegin = std::find_if(begin, end, [](Token* t) { return t->EqualChar('('); }) + 1;
	auto paramEnd   = std::find_if(begin, end, [](Token* t) { return t->EqualChar(')'); });

	// (...) �̒��� ���ʎq, ',', '=' ���������o��
	//auto argTokens = tr::MakeEnumerator::from(tokens->cbegin() + paramBegin, tokens->cbegin() + paramEnd)
	//	.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->EqualChar(',') || t->EqualChar('='); });

	//for (Token* tt : argTokens)
	//{
	//	Console::WriteLine(tt->GetString());
	//}
	return paramEnd;
}

void HeaderParser::ParseStructDecl(const Decl& decl)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);
	auto name = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar('{'); }) - 2;

	auto info = std::make_shared<TypeInfo>();
	info->name = (*name)->GetString();
	info->isStruct = true;
	m_database->structs.Add(info);

	for (auto& member : decl.decls)
	{
		if (member.type == "field") ParseFieldDecl(member, info);
		if (member.type == "method") ParseMethodDecl(member, info);
	}
}

void HeaderParser::ParseFieldDecl(const Decl& decl, TypeInfoPtr parent)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);
	auto name = decl.end - 2;
	auto type = decl.end - 4;

	auto info = std::make_shared<FieldInfo>();
	info->name = (*name)->GetString();
	info->typeRawName = (*type)->GetString();
	parent->declaredFields.Add(info);
}

void HeaderParser::ParseMethodDecl(const Decl& decl, TypeInfoPtr parent)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);

	// param range
	auto lparen = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar('('); });
	auto rparen = std::find_if(lparen, decl.end, [](Token* t) { return t->EqualChar(')'); });

	// return type .. method name
	auto declTokens = tr::MakeEnumerator::from(paramEnd, lparen)
		.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->GetTokenGroup() == TokenGroup::Keyword || t->GetTokenGroup() == TokenGroup::Operator || t->GetTokenGroup() == TokenGroup::ArithmeticLiteral; })
		//.Select([](Token* t) { return t->GetString(); })
		.ToList();

	//for (auto t : declTokens) Console::WriteLine(t->GetString());

	auto info = std::make_shared<MethodInfo>();
	info->name = (declTokens.GetLast())->GetString();	// ( �̒��O���֐����Ƃ��Ď��o��
	parent->declaredMethods.Add(info);

	// return type
	{
		auto begin = declTokens.begin();
		auto end = declTokens.end() - 1;
		int pointerLevel;
		ParseParamType(begin, end, &info->returnTypeRawName, &pointerLevel);

		for (auto itr = begin; itr != end; ++itr)
		{
			if ((*itr)->EqualString("static"))
				info->isStatic = true;
		}
	}

	// mehod params
	auto paramBegin = lparen + 1;
	auto itr = paramBegin;
	for (; itr < rparen; ++itr)
	{
		if ((*itr)->EqualChar(','))
		{
			ParseParamsDecl(paramBegin, itr, info);
			paramBegin = itr + 1;
		}
	}
	if (paramBegin != rparen) ParseParamsDecl(paramBegin, rparen, info);

	// method attribute
	{
		auto begin = rparen + 1;
		auto end = std::find_if(begin, decl.end, [](Token* t) { return t->EqualChar('{'); });
		for (auto itr = begin; itr < end; ++itr)
		{
			if ((*itr)->EqualString("const"))
			{
				info->isConst = true;
			}
		}
	}

	//auto name = paramEnd;
	//for (auto itr = paramEnd; itr != lparen; ++itr)
	//{
	//	if ((*itr)->g)
	//}

	//struct ParamsParser : public combinators::ParseLib<ParamsParser>
	//{
	//	struct Param
	//	{
	//		TokenItr			beginType;
	//		TokenItr			endType;
	//		StringA				name;
	//		Nullable<StringA>	defaultValue;
	//	};

	//	static ParserResult<Decl> Parse_Param(ParserContext input)
	//	{
	//		LN_PARSE_RESULT(r1, UntilPrev(TokenChar('=') || TokenChar(',') || TokenChar(')')));
	//		LN_PARSE_RESULT(r1, Optional(Token(')')));

	//		

	//		return input.Success(Param{ r1.GetMatchBegin(), r1.GetMatchEnd() });
	//	}

	//	static ParserResult<List<Decl>> Parse_Params(ParserContext input)
	//	{
	//		LN_PARSE(r1, Many(Parser<Decl>(Parse_LN_CLASS) || Parser<Decl>(Parse_LN_STRUCT) || Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LocalBlock) || Parser<Decl>(Parse_DocumentComment)));
	//		return input.Success(r1);
	//	}

	//	static bool FilterToken(fl::Token* token)
	//	{
	//		return
	//			token->GetTokenGroup() == TokenGroup::Identifier ||
	//			token->GetTokenGroup() == TokenGroup::Keyword ||
	//			token->GetTokenGroup() == TokenGroup::Operator ||
	//			token->GetTokenGroup() == TokenGroup::Eof;	// TODO: ���ꂪ�����Ă������悤�ɂ������B���͂��ꂪ�Ȃ��ƁAMany����EOF�����Ƃ�OutOfRange����
	//	}
	//};
}

void HeaderParser::ParseParamsDecl(TokenItr begin, TokenItr end, MethodInfoPtr parent)
{
	auto declTokens = tr::MakeEnumerator::from(begin, end)
		.Where([](Token* t) { return t->GetTokenGroup() == TokenGroup::Identifier || t->GetTokenGroup() == TokenGroup::Keyword || t->GetTokenGroup() == TokenGroup::Operator || t->GetTokenGroup() == TokenGroup::ArithmeticLiteral; })
		.ToList();;
		//.Select([](Token* t) { return t->GetString(); })
		//

	//paramEnd = declTokens.end();
	auto paramEnd = std::find_if(declTokens.begin(), declTokens.end(), [](Token* t) { return t->EqualChar('='); });

	// , �܂��� =(�f�t�H���g����) �̒��O���������Ƃ���
	auto name = end - 1;

	StringA typeName;
	int pointerLevel;
	ParseParamType(declTokens.begin(), paramEnd - 1, &typeName, &pointerLevel);

	auto info = std::make_shared<ParameterInfo>();
	info->name = (*name)->GetString();
	info->typeRawName = typeName;
	parent->parameters.Add(info);
}

void HeaderParser::ParseParamType(TokenItr begin, TokenItr end, StringA* outName, int* outPointerLevel)
{
	auto type = begin;
	auto typeEnd = end;

	// lookup TypeName and count '*'
	TokenItr typeName;
	int pointerLevel = 0;
	for (auto itr = begin; itr != typeEnd; ++itr)
	{
		if ((*itr)->EqualChar('*')) pointerLevel++;
		if ((*itr)->GetTokenGroup() == TokenGroup::Identifier || (*itr)->GetTokenGroup() == TokenGroup::Keyword) typeName = itr;
	}

	*outName = (*typeName)->GetString();
	*outPointerLevel = pointerLevel;
}

void HeaderParser::ParseClassDecl(const Decl& decl)
{
	auto paramEnd = ParseMetadataDecl(decl.begin, decl.end);
	auto name = std::find_if(paramEnd, decl.end, [](Token* t) { return t->EqualChar('{'); }) - 2;

	auto info = std::make_shared<TypeInfo>();
	info->name = (*name)->GetString();
	m_database->classes.Add(info);

	for (auto& member : decl.decls)
	{
		if (member.type == "method") ParseMethodDecl(member, info);
	}

}

