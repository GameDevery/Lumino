/*
	[2015/8/25]
		�E���ۂɉ�ʂɕ\������ Inline �� Visual �Ȃ񂽂�ɂ��Ă����B
		�@����͂����Ȃ�Θ_���v�f�ɑ΂��镨���v�f�B
		 �@�I�u�W�F�N�g�����Ă������ƂŁA�A�j���[�V���������₷���B

		�EAvalonEdit �ł́A�����s�N�Z�����_���s�̕ϊ��� Rendering.HeightTree �̖�ځB
		Rendering.HeightTree �́A���̕ϊ��Ƃ����������������ɍs���ԍ��؁B

		�EVisual �̍쐬�� Measure �ŁB

		�E���s��1�����B\r\n��1�����B
*/
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/TextBox.h>
#include <Lumino/Documents/DocumentsRenderer.h>
#include <Lumino/Documents/Run.h>

namespace Lumino
{
namespace GUI
{


// �{�i�I�ȃe�L�X�g�G�f�B�^�ł���Α����̂��߁A����ɒP��P�ʂɋ�؂����肷��B
// ������ TextBox �͕������Ƃɑ����Ƃ��͍l���Ȃ��V���v���Ȃ��̂Ȃ̂ŁA�܂��͍s�P�ʁB
class TextBox::LineSegment
	: public RefObject
{
public:
	LineSegment(const TCHAR* str, int len)
	{
		m_text.Append(str, len);
	}

	//int GetLength() const
	//{

	//}

public:
	StringBuilder	m_text;
	//int				m_realCharCount;

	RefPtr<Graphics::GlyphRun>		m_glyphRun;	// �{���Ȃ� View �ƕ�����ׂ��B�����A����̓V���v���d���ŁB
};

class TextBox::Document
{
public:
	//int GetLength() const {  }

	void Replace(int start, int length, const String& text)
	{
		// TODO:�Ƃ肠��������O��
		const TCHAR* begin = text.GetCStr();
		const TCHAR* end = begin + text.GetLength();
		int nlIndex, nlCount;
		while (StringTraits::IndexOfNewLineSequence(begin, end, &nlIndex, &nlCount))
		{
			RefPtr<TextBox::LineSegment> line(LN_NEW LineSegment(begin, nlIndex));
			m_lineSegments.Add(line);
			begin += (nlIndex + nlCount);	// ���s�����̎��̕������w��
		}
		if (begin != end)
		{
			RefPtr<TextBox::LineSegment> line(LN_NEW LineSegment(begin, nlIndex));
			m_lineSegments.Add(line);
		}
	}

public:
	Array< RefPtr<TextBox::LineSegment> >	m_lineSegments;
};




//=============================================================================
// TextBox
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBox, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBox);

LN_PROPERTY_IMPLEMENT_GETTER_SETTER(TextBox, String, TextProperty, "Text", get_Text, set_Text, PropertyMetadata(String::GetEmpty()));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox* TextBox::Create(GUIManager* manager)
{
	RefPtr<TextBox> obj(LN_NEW TextBox(manager));
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::TextBox(GUIManager* manager)
	: Control(manager)
	, m_document(NULL)
{
	m_document = LN_NEW Document();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::~TextBox()
{
	LN_SAFE_DELETE(m_document);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::OnRender(Graphics::Painter* painter)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::set_Text(const String& string)
{
	// TODO: �v���p�e�B�����l�Z�b�g����ĂȂ�
	m_document->Replace(0, 0, string);	// TODO: GetLength()
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& TextBox::get_Text() const
{
	return String::GetEmpty();
}

} // namespace GUI
} // namespace Lumino

