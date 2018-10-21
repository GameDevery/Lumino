
#include "Internal.hpp"
#include "CppLexer.hpp"

namespace ln {

int CppLexer::readToken(const Range& buffer)
{
	int len;

	// �󔒕���
	len = ReadSpaceSequence(buffer);
	if (len > 0) { return len; }
	// �v���v���Z�b�T�g�[�N����
	//len = ReadPPTokens(buffer);
	//if (len > 0) { return len; }
	// ���s
	len = ReadNewLine(buffer);
	if (len > 0) { return len; }
	// �\���
	len = ReadKeyword(buffer);
	if (len > 0) { return len; }
	// �������e����
	len = ReadCharLiteral(buffer);
	if (len > 0) { return len; }
	// �����񃊃e����
	len = ReadStringLiteral(buffer);
	if (len > 0) { return len; }
	// ���ʎq
	len = ReadIdentifier(buffer);
	if (len > 0) { return len; }
	// ���l���e����
	len = ReadNumericLiteral(buffer);
	if (len > 0) { return len; }
	// �u���b�N�R�����g
	len = ReadBlockComment(buffer);
	if (len > 0) { return len; }
	// �s�R�����g (���Z�q����Ɍ��Ă���)
	len = ReadLineComment(buffer);
	if (len > 0) { return len; }
	// ���Z�q
	len = ReadOperator(buffer);
	if (len > 0) { return len; }
	// �s���G�X�P�[�v
	len = ReadEscapeNewLine(buffer);
	if (len > 0) { return len; }
	// �}���`�o�C�g��������
	len = ReadMBSSequence(buffer);
	if (len > 0) { return len; }


	/*
	* �����񃊃e�����͎��ʎq�̑O�ɉ�͂���B����́AL ���̃v���t�B�b�N�X�Ή��̂��߁B
	* �Ȃ��ACheckHexLiteralStart() �� 0x ���v���t�B�b�N�X�Ƃ��ē��ʈ������Ă��邪�A
	* ������ł͂��̂悤�� L ����ʈ����͂��Ȃ��B
	* ����� C++11 �� raw string �Ή�������ɓ��ꂽ���́B
	*
	* raw string �͎��̂悤�ɏ������Ƃ��ł���B
	*		str = R"**(AAA )" BBB)**";
	* ���̃��e�����ł́A
	* �E�J�n�g�[�N��		R"**(
	* �E������			AAA )" BBB
	* �E�I�[�g�[�N��		)**"
	* �ł���B
	*
	* �v���t�B�b�N�X�� L R U ���l�X����A��������ʈ������ăp�[�X���Ă��A
	* ���� " ���ɂ́u���̎�ނł��邩�v�� CheckStringStart() �ɓn���Ȃ���΂Ȃ�Ȃ��B
	* �v���t�B�b�N�X�� " �̊Ԃɋ󔒂��\�ł���΂܂������A�킴�킴���ʈ������Ċ֐��𕪂���Ƌt�ɕ��G�ɂȂ��Ă��܂��B
	*/
	return 0;

}

int CppLexer::IsSpaceChar(const Range& r)
{
	if (r.pos[0] == ' ' || r.pos[0] == '\t' || r.pos[0] == '\f' || r.pos[0] == '\v')
	{
		return 1;
	}
	return 0;
}

int CppLexer::ReadSpaceSequence(const Range& buffer)
{
	// �A������X�y�[�X�����̐���Ԃ�
	// (�S�p�X�y�[�X�����e����ꍇ�͂���S�̂̕��������J�E���g)
	Range r = buffer;
	while (r.pos < r.end)
	{
		int len = IsSpaceChar(r);
		if (len == 0) {
			break;
		}
		r.pos += len;
	}

	// �g�[�N���쐬
	if (buffer.pos < r.pos) {
		AddToken(TokenGroup::SpaceSequence, buffer.pos, r.pos);
	}
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsKeyword(const Range& buffer, int* langTokenType)
{
	struct WordData
	{
		const char*		word;
		int				length;
		int				type;
	};
	static const WordData wordList[] =
	{
		{ ("asm"),					3,	TT_CppKW_asm },
		{ ("auto"),					4,	TT_CppKW_auto },
		{ ("bool"),					4,	TT_CppKW_bool },
		{ ("break"),					5,	TT_CppKW_break },
		{ ("case"),					4,	TT_CppKW_case },
		{ ("catch"),					5,	TT_CppKW_catch },
		{ ("char"),					4,	TT_CppKW_char },
		{ ("class"),					5,	TT_CppKW_class },
		{ ("const"),					5,	TT_CppKW_const },
		{ ("const_cast"),			10,	TT_CppKW_const_cast },
		{ ("continue"),				8,	TT_CppKW_continue },
		{ ("default"),				7,	TT_CppKW_default },
		{ ("delete"),				6,	TT_CppKW_delete },
		{ ("do"),					2,	TT_CppKW_do },
		{ ("double"),				6,	TT_CppKW_double },
		{ ("dynamic_cast"),			12,	TT_CppKW_dynamic_cast },
		{ ("else"),					4,	TT_CppKW_else },
		{ ("enum"),					4,	TT_CppKW_enum },
		{ ("explicit"),				8,	TT_CppKW_explicit },
		{ ("export"),				6,	TT_CppKW_export },
		{ ("extern"),				6,	TT_CppKW_extern },
		{ ("false"),					5,	TT_CppKW_false },
		{ ("float"),					5,	TT_CppKW_float },
		{ ("for"),					3,	TT_CppKW_for },
		{ ("friend"),				6,	TT_CppKW_friend },
		{ ("goto"),					4,	TT_CppKW_goto },
		{ ("if"),					2,	TT_CppKW_if },
		{ ("inline"),				6,	TT_CppKW_inline },
		{ ("int"),					3,	TT_CppKW_int },
		{ ("long"),					4,	TT_CppKW_long },
		{ ("mutable"),				7,	TT_CppKW_mutable },
		{ ("namespace"),				9,	TT_CppKW_namespace },
		{ ("new"),					3,	TT_CppKW_new },
		{ ("operator"),				8,	TT_CppKW_operator },
		{ ("private"),				7,	TT_CppKW_private },
		{ ("protected"),				9,	TT_CppKW_protected },
		{ ("public"),				6,	TT_CppKW_public },
		{ ("register"),				8,	TT_CppKW_register },
		{ ("reinterpret_cast"),		16,	TT_CppKW_reinterpret_cast },
		{ ("return"),				6,	TT_CppKW_return },
		{ ("short"),					5,	TT_CppKW_short },
		{ ("signed"),				6,	TT_CppKW_signed },
		{ ("sizeof"),				6,	TT_CppKW_sizeof },
		{ ("static"),				6,	TT_CppKW_static },
		{ ("static_cast"),			11,	TT_CppKW_static_cast },
		{ ("struct"),				6,	TT_CppKW_struct },
		{ ("switch"),				6,	TT_CppKW_switch },
		{ ("template"),				8,	TT_CppKW_template },
		{ ("this"),					4,	TT_CppKW_this },
		{ ("throw"),					5,	TT_CppKW_throw },
		{ ("true"),					4,	TT_CppKW_true },
		{ ("try"),					3,	TT_CppKW_try },
		{ ("typedef"),				7,	TT_CppKW_typedef },
		{ ("typeid"),				6,	TT_CppKW_typeid },
		{ ("typename"),				8,	TT_CppKW_typename },
		{ ("union"),					5,	TT_CppKW_union },
		{ ("unsigned"),				8,	TT_CppKW_unsigned },
		{ ("using"),					5,	TT_CppKW_using },
		{ ("virtual"),				7,	TT_CppKW_virtual },
		{ ("void"),					4,	TT_CppKW_void },
		{ ("volatile"),				8,	TT_CppKW_volatile },
		{ ("wchar_t"),				7,	TT_CppKW_wchar_t },
		{ ("while"),					5,	TT_CppKW_while },
	};

	const int len = LN_ARRAY_SIZE_OF(wordList);
	int keyLen = 0;
	int type = 0;
	for (int i = 0; i < len; ++i)
	{
		if (wordList[i].word[0] == buffer.pos[0] &&		// �܂��͐擪�����𒲂ׂ�
			strncmp(wordList[i].word, buffer.pos, wordList[i].length) == 0)	// �擪����v������c��𒲂ׂ�
		{
			type = (int)wordList[i].type;
			keyLen = wordList[i].length;
			break;
		}
	}

	// �{���ɃL�[���[�h�����ʎq�Ƃ��Ċ������Ă���H
	if (keyLen > 0 && IsIdentifierLetter(Range(buffer.pos + keyLen, buffer.end)) == 0)
	{
		*langTokenType = type;
		return keyLen;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadKeyword(const Range& buffer)
{
	int lnagTokenType = 0;
	int len = IsKeyword(buffer, &lnagTokenType);
	if (len > 0) {
		AddToken(TokenGroup::Keyword, buffer.pos, buffer.pos + len, lnagTokenType);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadEnclosingTokenHelper(const Range& buffer, TokenCheckCallback startCallback, TokenCheckCallback endCallback, const char* chars, bool* outNotFoundEndToken)
{
	Range r = buffer;
	*outNotFoundEndToken = true;

	// ������̊J�n�`�F�b�N
	int startCount = startCallback(r);
	if (startCount == 0) {
		return 0;
	}
	r.pos += startCount;

	while (r.pos < r.end)
	{
		// �G�X�P�[�v�V�[�P���X�̃`�F�b�N
		int len = 0;
		if (chars != nullptr && r.pos[0] == '\\')
		{
			for (const char* c = chars; *c != '\0'; ++c)
			{
				if (r.pos[1] == *c)
				{
					len = 2;
					break;
				}
			}
		}
		if (len > 0)
		{
			r.pos += len;
			continue;	// ���̌�̉�͂ɂ͉񂳂Ȃ��Ŏ��̕�����
		}

		// �I�[�`�F�b�N
		len = endCallback(r);
		if (len > 0) {
			r.pos += len;
			*outNotFoundEndToken = false;
			break;		// �I��
		}

		r.pos++;
	}

	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::ReadCharLiteral(const Range& buffer)
{
	//const TokenChar chars[] = { '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\0' };
	const char chars[] = { '\'', '\0' };	// �������ړI�Ȃ̂� ' �����G�X�P�[�v��OK
	bool notFoundEndToken;
	int len = ReadEnclosingTokenHelper(buffer, IsCharLiteralStart, IsCharLiteralEnd, chars, &notFoundEndToken);

	if (len > 0)
	{
		int type = TT_NumericLitaralType_Char;
		if (buffer.pos[0] == 'L') {
			type = TT_NumericLitaralType_WideChar;
		}
		AddToken(TokenGroup::ArithmeticLiteral, buffer.pos, buffer.pos + len, type);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsCharLiteralStart(const Range& buffer)
{
	if (buffer.pos[0] == '\'') {
		return 1;
	}
	if (buffer.pos[0] == 'L' && buffer.pos[1] == '\'') {
		return 2;	// L �v���t�B�b�N�X�t�������񃊃e����
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsCharLiteralEnd(const Range& buffer)
{
	if (buffer.pos[0] == '\'') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//int CppLexer::IsAnyChar(const Range& buffer, const TokenChar* chars, int len)
//{
//	for (int i = 0; i < len; ++i)
//	{
//		if (buffer.pos[0] == chars[i])
//		{
//			return 1;
//		}
//	}
//	return 0;
//}


//------------------------------------------------------------------------------
int CppLexer::ReadStringLiteral(const Range& buffer)
{
	bool notFoundEndToken;

	int len = 0;
	//if (m_seqPPDirective != PPDirectiveSeq::FoundInclude)
	//{
	//const TokenChar chars[] = { '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\0' };
	const char chars[] = { '"', '\0' };	// �������ړI�Ȃ̂� " �����G�X�P�[�v��OK
	len = ReadEnclosingTokenHelper(buffer, IsStringLiteralStart, IsStringLiteralEnd, chars, &notFoundEndToken);
	//}
	//else
	//{
	//	// #include ���̏ꍇ�͂�����B�G�X�P�[�v�͖���
	//	len = ReadEnclosingTokenHelper(buffer, IsStringLiteralStartInIncludeDirective, IsStringLiteralEndIncludeDirective, nullptr, &notFoundEndToken);
	//}

	if (len > 0)
	{
		int type = TT_NumericLitaralType_AsciiString;
		if (buffer.pos[0] == 'L') {
			type = TT_NumericLitaralType_WideString;
		}
		AddToken(TokenGroup::StringLiteral, buffer.pos, buffer.pos + len, type);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralStart(const Range& buffer)
{
	if (buffer.pos[0] == '"') {
		return 1;
	}
	if (buffer.pos[0] == 'L' && buffer.pos[1] == '"') {
		return 2;	// L �v���t�B�b�N�X�t�������񃊃e����
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralEnd(const Range& buffer)
{
	if (buffer.pos[0] == '"') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadIdentifier(const Range& buffer)
{
	/*
	a b c d e f g h i j k l m
	n o p q r s t u v w x y z
	A B C D E F G H I J K L M
	N O P Q R S T U V W X Y Z _
	0 1 2 3 4 5 6 7 8 9
	*/
	Range r = buffer;

	// ���ʎq�̐擪�������H
	int len = IsIdentifierStart(buffer);
	if (len == 0) {
		return 0;	// ���ʎq�ł͂Ȃ�
	}
	r.pos += len;

	// ��ɑ��������͐��������H
	while (r.pos < r.end)
	{
		len = IsIdentifierLetter(r);
		if (len == 0) {
			break;
		}
		r.pos += len;
	}

	// �g�[�N���쐬
	AddToken(TokenGroup::Identifier, buffer.pos, r.pos);
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsIdentifierStart(const Range& buffer)
{
	if (isalpha(buffer.pos[0]) || buffer.pos[0] == '_')
	{
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsIdentifierLetter(const Range& buffer)
{
	if (isalnum(buffer.pos[0]) || buffer.pos[0] == '_')
	{
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadNumericLiteral(const Range& buffer)
{
	/* 123	10�i�����e����
	* 012	8�i�����e����
	*
	*/

	// TODO: ���̓��e�� LangDef �Ɏ����Ă����ׂ������H

	bool	bExistPeriod = false;		/* �����_���o�ꂵ����TRUE */
	bool	bExistExp = false;		/* �w�����o�ꂵ����TRUE */
	bool	bExistSuffix = false;		/* �ڔ��ꂪ�o�ꂵ����TRUE */
	bool	bHexMode = false;		/* 16�i���ŗL��ꍇ��TRUE */

									// ���l���e�����̌^�B�T�t�B�b�N�X�Ƃ��Ȃ���΃f�t�H���g�� Int32
	int	litaralType = TT_NumericLitaralType_Int32;

	// 16�i���̐擪�`�F�b�N
	//const TokenChar* pPos = buffer.pos;
	Range r = buffer;
	int len = IsHexLiteralStart(r);
	if (len > 0) {
		r.pos += len;
		bHexMode = true;	// 16 �i���ł���
	}

	int nStep = 0;	// ��ԑJ�ڂ�\��

	bool bEnd = false;
	while (r.pos < r.end && !bEnd)
	{
		if (r.pos[0] > 255) {	// wchar_t �͈̔̓`�F�b�N
			break;
		}
		Flags<AlphaNumTypeFlags> charType = GetAlphaNumType(r.pos[0]);

		// �}���`�o�C�g�R�[�h�܂��͐��䕶���܂��� . �ȊO�̉��Z�q�ł���ΏI��
		if (charType.hasFlag(AlphaNumTypeFlags::MBC) ||
			charType.hasFlag(AlphaNumTypeFlags::Control) ||
			(charType.hasFlag(AlphaNumTypeFlags::OpChar) && r.pos[0] != '.')) {
			break;	// �I��
		}

		// �T�t�B�b�N�X�̃`�F�b�N (�T�t�B�b�N�X�͈�ԍŌゾ���A���낢��ȂƂ���ɏ�����B1f 1.0f .1f 1.0-e2f)
		if (charType.hasFlag(AlphaNumTypeFlags::Alphabet) /*|| charType.TestFlag(AlphaNumTypeFlags::HexAlpha)*/)
		{
			// �����^
			len = IsIntegerSuffix(r, &litaralType);
			if (len > 0) {
				r.pos += len;
				bExistSuffix = true;	// �T�t�B�b�N�X��������
				nStep = 10;
				continue;
			}
			// �����^
			if (!bHexMode)	// 16�V���� F �� float �� F ����ʂ�����
			{
				len = IsRealSuffix(r, &litaralType);
				if (len > 0) {
					r.pos += len;
					bExistSuffix = true;	// �T�t�B�b�N�X��������
					nStep = 10;
					continue;
				}
			}
			// �� 50LL �̂悤�ɏd���̉\�������邽�߁A�����������Ă��G���[�ɂ��Ȃ�
		}

		if (bExistSuffix) {
			return 0;	// �T�t�B�b�N�X�̌�ɁA�T�t�B�b�N�X�ł͂Ȃ������������B��������
		}

		switch (nStep)
		{
		case 0:		// ��������
		{
			if (bHexMode && charType.hasFlag(AlphaNumTypeFlags::HexAlpha)) {
				r.pos++;		// 16 �i���Ȃ� HexAlpha �������Čp��
			}
			else if (charType.hasFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// ���ʂ̐��l�� OK
			}
			else if (charType.hasFlag(AlphaNumTypeFlags::OpChar) && r.pos[0] == '.') {
				nStep = 1;	// . �����������B���������ֈڍs
				r.pos++;
				litaralType = TT_NumericLitaralType_Double;	// �Ƃ肠���� Double �^�Ƃ���
			}
			else {
				len = IsExponentStart(r);
				if (len > 0)		// e E + -
				{
					nStep = 2;		// e- �Ƃ������������B�w�������ֈڍs
					r.pos += len;
					litaralType = TT_NumericLitaralType_Double;	// �w����������̂łƂ肠���� Double �^�Ƃ���
				}
				else {
					return 0;		// 10.5G ���A�ςȃT�t�B�b�N�X�Ƃ��A���̎��ʎq�Ƃ̊ԂɃX�y�[�X�������Ƃ�
				}
			}
			break;
		}
		case 1:		// ��������
		{
			if (bHexMode && charType.hasFlag(AlphaNumTypeFlags::HexAlpha)) {
				r.pos++;		// 16 �i���Ȃ� HexAlpha �������Čp��
			}
			else if (charType.hasFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// ���ʂ̐��l�� OK
			}
			else
			{
				len = IsExponentStart(r);
				if (len > 0)		// e E + -
				{
					nStep = 2;		// e- �Ƃ������������B�w�������ֈڍs
					r.pos += len;
					//litaralType = TT_NumericLitaralType_Double;	// �w����������̂łƂ肠���� Double �^�Ƃ���
				}
				else {
					return 0;		// 10.5G ���A�ςȃT�t�B�b�N�X�Ƃ��A���̎��ʎq�Ƃ̊ԂɃX�y�[�X�������Ƃ�
				}
			}
			break;
		}
		case 2:		// �w������
		{
			if (charType.hasFlag(AlphaNumTypeFlags::Number)) {
				r.pos++;		// ���ʂ̐��l�� OK
			}
			else {
				return 0;	// ����ȊO�͎��s
			}
			break;
		}
		}
	}

	if (r.pos - buffer.pos > 0)
	{
		// �����܂ŗ������͐���
		AddToken(TokenGroup::ArithmeticLiteral, buffer.pos, r.pos, litaralType);
	}
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsHexLiteralStart(const Range& buffer)
{
	if (buffer.pos[0] == '0' && (buffer.pos[1] == 'x' || buffer.pos[1] == 'X'))
		return 2;
	return 0;
	// VB �͂����� &H
}

//------------------------------------------------------------------------------
int CppLexer::IsIntegerSuffix(const Range& buffer, int* outLiteralType)
{
	/*
	u U
	l L
	ul Ul uL UL
	ll LL
	ull Ull uLL uLL
	*/
	if (buffer.pos[0] == 'u' || buffer.pos[0] == 'U')
	{
		if (buffer.pos[1] == 'l' || buffer.pos[1] == 'L')
		{
			if (buffer.pos[2] == 'l' || buffer.pos[2] == 'L')
			{
				*outLiteralType = TT_NumericLitaralType_UInt64;
				return 3;
			}
			else
			{
				*outLiteralType = TT_NumericLitaralType_UInt32;
				return 2;
			}
		}
		else
		{
			*outLiteralType = TT_NumericLitaralType_UInt32;
			return 1;
		}
	}

	if (buffer.pos[0] == 'l' || buffer.pos[0] == 'L')
	{
		if (buffer.pos[1] == 'l' || buffer.pos[1] == 'L')
		{
			*outLiteralType = TT_NumericLitaralType_Int64;
			return 2;
		}
		else
		{
			*outLiteralType = TT_NumericLitaralType_Int32;
			return 1;
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsRealSuffix(const Range& buffer, int* outLiteralType)
{
	if (buffer.pos[0] == 'f' || buffer.pos[0] == 'F')
	{
		*outLiteralType = TT_NumericLitaralType_Float;
		return 1;
	}
	if (buffer.pos[0] == 'd' || buffer.pos[0] == 'D')
	{
		*outLiteralType = TT_NumericLitaralType_Double;
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsExponentStart(const Range& buffer)
{
	if (buffer.pos[0] == 'e' ||
		buffer.pos[0] == 'E')
	{
		if (buffer.pos[1] == '+' ||
			buffer.pos[1] == '-') {
			return 2;
		}
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadBlockComment(const Range& buffer)
{
	bool notFoundEndToken;
	int len = ReadEnclosingTokenHelper(buffer, IsBlockCommentStart, IsBlockCommentEnd, nullptr, &notFoundEndToken);
	if (len > 0)
	{
		if (notFoundEndToken)
		{
			diag()->reportError(_T("DiagnosticsCode::UnexpectedEOFInBlockComment"));
			return 0;
		}
		AddToken(TokenGroup::Comment, buffer.pos, buffer.pos + len);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsBlockCommentStart(const Range& buffer)
{
	if (buffer.pos[0] == '/' &&
		buffer.pos[1] == '*')
		return 2;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsBlockCommentEnd(const Range& buffer)
{
	if (buffer.pos[0] == '*' &&
		buffer.pos[1] == '/')
		return 2;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadLineComment(const Range& buffer)
{
	// �s�R�����g�̊J�n�`�F�b�N
	int len = IsLineCommentStart(buffer);
	if (len == 0) {
		return 0;
	}

	Range r = buffer;
	while (r.pos < r.end)
	{
		// �s�� \ �`�F�b�N
		len = IsEscapeNewLine(r);
		if (len > 0) {
			r.pos += len;
			continue;		// �p��
		}

		// �I���`�F�b�N (EOF �� end ���B�Ń��[�v������̂œ��ɋC�ɂ��Ȃ��ėǂ�)
		len = IsNewLine(r);
		if (len > 0) {
			// ���s�̓R�����g�ɂ͊܂܂Ȃ�
			break;			// �I��
		}

		++r.pos;
	}

	AddToken(TokenGroup::Comment, buffer.pos, r.pos);
	return r.pos - buffer.pos;
}

//------------------------------------------------------------------------------
int CppLexer::IsLineCommentStart(const Range& buffer)
{
	if (buffer.pos[0] == '/' &&
		buffer.pos[1] == '/')
		return 2;
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadOperator(const Range& buffer)
{
	int lnagTokenType = 0;
	int len = IsOperator(buffer, &lnagTokenType);
	if (len > 0) {
		AddToken(TokenGroup::Operator, buffer.pos, buffer.pos + len, lnagTokenType);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsOperator(const Range& buffer, int* langTokenType)
{
	if (GetAlphaNumType(buffer.pos[0]).hasFlag(AlphaNumTypeFlags::OpChar))
	{
		struct WordData
		{
			const char*		word;
			int				length;
			int				type;
		};
		// ++ �� + �̂悤�ɁA1�����ڂ��������͕̂������̑���������Ɍ��������悤�ɂ���
		static const WordData wordList[] =
		{
			{ ("##"),	2,	TT_CppOP_SharpSharp },
		{ ("#"),		1,	TT_CppOP_Sharp },
		{ ("->*"),	3,	TT_CppOP_ArrowAsterisk },
		{ ("->"),	2,	TT_CppOP_Arrow },
		{ (","),		1,	TT_CppOP_Comma },
		{ ("++"),	2,	TT_CppOP_Increment },
		{ ("--"),	2,	TT_CppOP_Decrement },
		{ ("&&"),	2,	TT_CppOP_LogicalAnd },
		{ ("||"),	2,	TT_CppOP_LogicalOr },
		{ ("<="),	2,	TT_CppOP_LessThenEqual },
		{ (">="),	2,	TT_CppOP_GreaterThenEqual },
		{ ("=="),	2,	TT_CppOP_CmpEqual },
		{ ("<<="),	3,	TT_CppOP_LeftShiftEqual },
		{ (">>="),	3,	TT_CppOP_RightShiftEqual },
		{ ("+="),	2,	TT_CppOP_PlusEqual },
		{ ("-="),	2,	TT_CppOP_MinusEqual },
		{ ("*="),	2,	TT_CppOP_MulEqual },
		{ ("/="),	2,	TT_CppOP_DivEqual },
		{ ("%="),	2,	TT_CppOP_ModEqual },
		{ ("&="),	2,	TT_CppOP_AndEqual },
		{ ("|="),	2,	TT_CppOP_OrEqual },
		{ ("!="),	2,	TT_CppOP_NotEqual },
		{ ("="),		1,	TT_CppOP_Equal },
		{ ("<<"),	2,	TT_CppOP_LeftShift },
		{ (">>"),	2,	TT_CppOP_RightShift },
		{ ("+"),		1,	TT_CppOP_Plus },
		{ ("-"),		1,	TT_CppOP_Minus },
		{ ("*"),		1,	TT_CppOP_Asterisk },
		{ ("/"),		1,	TT_CppOP_Slash },
		{ ("%"),		1,	TT_CppOP_Parseint },
		{ ("&"),		1,	TT_CppOP_Ampersand },
		{ ("|"),		1,	TT_CppOP_Pipe },
		{ ("~"),		1,	TT_CppOP_Tilde },
		{ ("^"),		1,	TT_CppOP_Caret },
		{ ("!"),		1,	TT_CppOP_Exclamation },
		{ ("..."),	3,	TT_CppOP_Ellipsis },
		{ (".*"),	2,	TT_CppOP_DotAsterisk },
		{ ("."),		1,	TT_CppOP_Dot },
		{ ("::"),	2,	TT_CppOP_DoubleColon },
		{ ("?"),		1,	TT_CppOP_Question },
		{ (":"),		1,	TT_CppOP_Colon },
		{ (";"),		1,	TT_CppOP_Semicolon },
		{ ("{"),		1,	TT_CppOP_LeftBrace },
		{ ("}"),		1,	TT_CppOP_RightBrace },
		{ ("["),		1,	TT_CppOP_LeftBracket },
		{ ("]"),		1,	TT_CppOP_RightBracket },
		{ ("("),		1,	TT_CppOP_LeftParen },
		{ (")"),		1,	TT_CppOP_RightParen },
		{ ("<"),		1,	TT_CppOP_LeftAngle },
		{ (">"),		1,	TT_CppOP_RightAngle },
		};
		const int count = LN_ARRAY_SIZE_OF(wordList);
		for (int i = 0; i < count; ++i)
		{
			if (wordList[i].word[0] == buffer.pos[0] &&		// �܂��͐擪�����𒲂ׂ�
				strncmp(wordList[i].word, buffer.pos, wordList[i].length) == 0)	// �擪����v������c��𒲂ׂ�
			{
				*langTokenType = (int)wordList[i].type;
				return wordList[i].length;
			}
		}
		return 0;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadEscapeNewLine(const Range& buffer)
{
	int len = IsEscapeNewLine(buffer);
	if (len > 0) {
		AddToken(TokenGroup::SpaceSequence, buffer.pos, buffer.pos + len, TT_EscapeNewLine);
		return len;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsEscapeNewLine(const Range& buffer)
{
	if (buffer.pos[0] == '\\')
	{
		if (buffer.pos[1] == '\r' &&
			buffer.pos[2] == '\n')
		{
			return 3;
		}
		if (buffer.pos[1] == '\r' ||
			buffer.pos[1] == '\n')
		{
			return 2;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::ReadPPHeaderName(const Range& buffer)
{
	// �G�X�P�[�v�͖���
	bool notFoundEndToken;
	int len = ReadEnclosingTokenHelper(buffer, IsStringLiteralStartInIncludeDirective, IsStringLiteralEndIncludeDirective, nullptr, &notFoundEndToken);
	if (len > 0)
	{
		AddToken(TokenGroup::StringLiteral, buffer.pos, buffer.pos + len, TT_HeaderName);
	}
	return len;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralStartInIncludeDirective(const Range& buffer)
{
	if (buffer.pos[0] == '"' || buffer.pos[0] == '<') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int CppLexer::IsStringLiteralEndIncludeDirective(const Range& buffer)
{
	if (buffer.pos[0] == '"' || buffer.pos[0] == '>') {
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
// �v���v���Z�b�T�g�[�N����(1�s)�̓ǂݎ��B
// ���s�͊܂܂Ȃ��B
// #define AAA �� �u#, define, space, AAA, NewLine�v�ŏo�Ă���B
// EscapeNewLine �����l�B
//		#error AAA\
//		BBB
// �́A�u#, error, space, PPTokens, EscapeNewLine, PPTokens�v
//------------------------------------------------------------------------------
int CppLexer::ReadPPTokens(const Range& buffer)
{
	Range r = buffer;
	while (r.pos < r.end)
	{
		// ���s�܂œǂ�
		int len = IsEscapeNewLine(r);
		if (len > 0) {
			break;		// �I��
		}
		len = IsNewLine(r);
		if (len > 0) {
			break;		// �I��
		}
		++r.pos;
	}

	if (r.pos - buffer.pos > 0)
	{
		AddToken(TokenGroup::TextTokens, buffer.pos, r.pos, TT_PPTokens);
	}
	return r.pos - buffer.pos;
}

} // namespace ln
