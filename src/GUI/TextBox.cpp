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
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/TextBox.h>
#include <Lumino/Documents/DocumentsRenderer.h>
#include <Lumino/Documents/Run.h>
#include "../Graphics/FontGlyphTextureCache.h"
#include "../Graphics/GraphicsHelper.h"
#include "GUIHelper.h"

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
	LineSegment(Document* document, const TCHAR* str, int len);

	void UpdateGlyphRuns();

	//int GetLength() const
	//{

	//}

public:
	Document*		m_document;
	GenericStringBuilderCore<UTF32>	m_utf32Text;
	//int				m_realCharCount;

	RefPtr<Graphics::GlyphRun>		m_glyphRun;	// �{���Ȃ� View �ƕ�����ׂ��B�����A����̓V���v���d���ŁB
};

class TextBox::Document
{
public:
	//int GetLength() const {  }

	Document()
	{
		m_TCharToUTF32Converter.SetDestinationEncoding(Text::Encoding::GetUTF32Encoding());
		m_TCharToUTF32Converter.SetSourceEncoding(Text::Encoding::GetTCharEncoding());
		m_UTF32ToTCharConverter.SetDestinationEncoding(Text::Encoding::GetTCharEncoding());
		m_UTF32ToTCharConverter.SetSourceEncoding(Text::Encoding::GetUTF32Encoding());
	}

	void SetFontGlyphTextureCache(Graphics::Internal::FontGlyphTextureCache* cache)
	{
		m_glyphTextureCache = cache;
		for (auto& seg : m_lineSegments) {
			seg->UpdateGlyphRuns();
		}
	}

	Graphics::Internal::FontGlyphTextureCache* GetFontGlyphTextureCache()
	{
		return m_glyphTextureCache;
	}

	void Replace(int start, int length, const String& text)
	{
		// TODO:�Ƃ肠��������O��
		const TCHAR* pos = text.GetCStr();
		const TCHAR* end = pos + text.GetLength();
		int nlIndex = 0, nlCount = 0;
		while (StringTraits::IndexOfNewLineSequence(pos, end, &nlIndex, &nlCount))
		{
			RefPtr<TextBox::LineSegment> line(LN_NEW LineSegment(this, pos, nlIndex));
			m_lineSegments.Add(line);
			pos += (nlIndex + nlCount);	// ���s�����̎��̕������w��
		}
		if (pos != end)
		{
			RefPtr<TextBox::LineSegment> line(LN_NEW LineSegment(this, pos, end - pos));
			m_lineSegments.Add(line);
		}
	}

	void Render(Graphics::Painter* painter)
	{
		Point pt(0, 0);
		for (auto& seg : m_lineSegments) {
			painter->DrawGlyphRun(pt, seg->m_glyphRun);
			pt.Y += seg->m_glyphRun->GetRenderSize().Height;
		}
	}

public:
	Array< RefPtr<TextBox::LineSegment> >	m_lineSegments;
	RefPtr<Graphics::Internal::FontGlyphTextureCache>	m_glyphTextureCache;
	Text::EncodingConverter		m_TCharToUTF32Converter;
	Text::EncodingConverter		m_UTF32ToTCharConverter;
};




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::LineSegment::LineSegment(Document* document, const TCHAR* str, int len)
	: m_document(document)
{
	m_glyphRun.Attach(LN_NEW Graphics::GlyphRun());
	m_utf32Text.Clear();
	m_utf32Text.Append(m_document->m_TCharToUTF32Converter.Convert(str, len * sizeof(TCHAR)));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::LineSegment::UpdateGlyphRuns()
{
	Graphics::Helper::AttachGlyphTextureCache(m_glyphRun, m_document->GetFontGlyphTextureCache());
	m_glyphRun->Update(m_utf32Text.GetCStr(), m_utf32Text.GetLength());
}


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
SizeF TextBox::MeasureOverride(const SizeF& constraint)
{
	if (GUIHelper::UIElement_GetInvalidateFlags(this).TestFlag(InvalidateFlags::Font))
	{
		Graphics::GraphicsManager::FontData fontData;
		fontData.Family = GetFontFamily();
		fontData.Size = GetFontSize();
		fontData.IsBold = IsFontBold();
		fontData.IsItalic = IsFontItalic();
		fontData.IsAntiAlias = IsFontAntiAlias();
		RefPtr<Graphics::Internal::FontGlyphTextureCache> cache = m_manager->GetGraphicsManager()->LookupGlyphTextureCache(fontData);
		m_document->SetFontGlyphTextureCache(cache);
	}
	return Control::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::OnRender(Graphics::Painter* painter)
{
	Control::OnRender(painter);
	m_document->Render(painter);
	//painter->SetBrush(Graphics::ColorBrush::Red);
	//painter->DrawRectangle(RectF(0, 0, GetRenderSize()));
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

