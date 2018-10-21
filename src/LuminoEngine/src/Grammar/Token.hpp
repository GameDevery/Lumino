#pragma once

namespace ln {

using SourceLocation = uint32_t;

/** ��{�I�ȃg�[�N����� (����ԋ���) */
enum class TokenGroup
{
	Unknown = 0,			/**< ���ʎ�ʂƂ��Ă͕��ނ���Ȃ� */
	SpaceSequence,			/**< �󔒕��� (C�� EscapeNewLine ���܂�) */
	NewLine,				/**< ���s ("\r" "\n" "\r\n" �̂����ꂩ�B���тł͂Ȃ��B"\r\n\r\n" ��2�� NewLine �g�[�N���ƂȂ�) */
	Identifier,				/**< ���ʎq */
	Keyword,				/**< �L�[���[�h */
	Operator,				/**< ���Z�q */
	ArithmeticLiteral,		/**< �Z�p�^���e���� (�����A�����BC����� char ���Z�p�^) */
	StringLiteral,			/**< ������ */
	Comment,				/**< �R�����g */
	MbsSequence,			/**< �}���`�o�C�g�������� */
	TextTokens,				/**< �v���v���Z�b�T�ȂǂŎg���郁�b�Z�[�W������ */
	Eof,					/**< �o�b�t�@ EOF */
};

class Token
{
public:
	Token(TokenGroup group, uint32_t kind, SourceLocation location, size_t length)
		: m_group(group)
		, m_kind(kind)
		, m_location(location)
		, m_length(length)
	{}

	TokenGroup group() const { return m_group; }
	uint32_t kind() const { return m_kind; }
	SourceLocation location() const { return m_location; }
	size_t length() const { return m_length; }

private:
	TokenGroup m_group;
	uint32_t m_kind;
	SourceLocation m_location;
	size_t m_length;
};

} // namespace ln
