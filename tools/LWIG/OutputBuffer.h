
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
	void Clear();

	/** �C���f���g���x�����ЂƂ��₷ */
	void IncreaseIndent();

	/** �C���f���g���x�����ЂƂ��炷 */
	void DecreaseIndent();
	
	/** �������ǉ����� */
	template<typename... TArgs>
	void Append(const StringRef& format, const TArgs&... args) { AppendInternal(String::Format(format, args...)); }
	
	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	template<typename... TArgs>
	void AppendLine(const StringRef& format, const TArgs&... args) { AppendLineInternal(String::Format(format, args...)); }
	
	/** �������ǉ����� (�e�s�̐擪���C���f���g) */
	template<typename... TArgs>
	void AppendLines(const StringRef& format, const TArgs&... args) { AppendLinesInternal(String::Format(format, args...)); }
	
	/** ���s���� */
	void NewLine(int count = 1);
	
	/** ���݂̃C���f���g���x�����̋󔒂�ǉ����� */
	void Indent();
	
	/** String �擾 */
	String ToString() const;
	
private:
	void AppendInternal(const StringRef& str);
	void AppendLineInternal(const StringRef& str);
	void AppendLinesInternal(const StringRef& str);
	
	StringBuilder	m_buffer;
	int				m_indentLevel;
	String			m_indent;
	String			m_newLineCode;
};

