
#pragma once

/** �R�[�h�o�͗p�o�b�t�@ */
class OutputBuffer
{
public:
	OutputBuffer();
	OutputBuffer(int indent);
	
	/** ��ł��邩 */
	bool IsEmpty() const { return m_buffer.IsEmpty(); }
	
	/** �N���A���� */
	void clear();

	/** �C���f���g���x�����ЂƂ��₷ */
	void IncreaseIndent(int count = 1);

	/** �C���f���g���x�����ЂƂ��炷 */
	void DecreaseIndent();

	/** �������ǉ����� */
	OutputBuffer& Append(const StringRef& str) { AppendInternal(str); return *this; }

	/** �������ǉ����� */
	template<typename... TArgs>
	OutputBuffer& Append(const StringRef& format, const TArgs&... args) { AppendInternal(String::Format(format, args...)); return *this; }

	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	OutputBuffer& AppendLine(const StringRef& str) { AppendLineInternal(str); return *this; }

	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	template<typename... TArgs>
	OutputBuffer& AppendLine(const StringRef& format, const TArgs&... args) { AppendLineInternal(String::Format(format, args...)); return *this; }

	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	OutputBuffer& AppendLines(const StringRef& str) { AppendLinesInternal(str, String::GetEmpty()); return *this; }

	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	template<typename... TArgs>
	OutputBuffer& AppendLines(const StringRef& format, const TArgs&... args) { AppendLinesInternal(String::Format(format, args...), String::GetEmpty()); return *this; }
	
	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	OutputBuffer& AppendLinesHeaderd(const StringRef& str, const StringRef& lineHeader) { AppendLinesInternal(str, lineHeader); return *this; }

	/** ���ɕ����񂪑��݂���� , ��}�����ĕ������ǉ����� */
	template<typename... TArgs>
	OutputBuffer& AppendCommad(const StringRef& format, const TArgs&... args)
	{
		if (!IsEmpty()) AppendInternal(", ");
		AppendInternal(String::Format(format, args...));
		return *this;
	}

	/** ���s���� */
	OutputBuffer& NewLine(int count = 1);
	
	/** ���݂̃C���f���g���x�����̋󔒂�ǉ����� */
	void Indent();
	
	/** String �擾 */
	String ToString() const;
	
private:
	void AppendInternal(const StringRef& str);
	void AppendLineInternal(const StringRef& str);
	void AppendLinesInternal(const StringRef& str, const StringRef& lineHeader);
	
	enum class State
	{
		LineHead,
		Text,
	};

	StringBuilder	m_buffer;
	int				m_indentLevel;
	String			m_indent;
	String			m_newLineCode;
	State			m_state;
};

