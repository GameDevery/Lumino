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
#include "../Animation/AnimationCurve.h"
#include "../Graphics/FontGlyphTextureCache.h"
#include "../Graphics/GraphicsHelper.h"
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{

class TextBox::Selection
{
public:
	int	Start;		///< text.GetLength() �͗L���l�BSelect(text.GetLength(), 0) �ŃR���e���c�̖����ɃL�����b�g���ړ�����
	int Length;

public:
	Selection()
		: Start(0)
		, Length(0)
	{}
};


// �{�i�I�ȃe�L�X�g�G�f�B�^�ł���Α����̂��߁A����ɒP��P�ʂɋ�؂����肷��B
// ������ TextBox �͕������Ƃɑ����Ƃ��͍l���Ȃ��V���v���Ȃ��̂Ȃ̂ŁA�܂��͍s�P�ʁB
// �Ȃ��A���s�����͊܂܂Ȃ��B
class TextBox::LineSegment
	: public RefObject
{
public:
	LineSegment(Document* document, const TCHAR* str, int len);

	void UpdateGlyphRuns();


	int GetLength() const
	{
		return m_utf32Text.GetLength();
	}

	int GetLinePixelHeight() const
	{
		return m_glyphRun->GetRenderSize().Height;
	}

	int GetPixelPosFromLineHead(int contentPos)
	{
		if (contentPos == 0) { return 0; }
		int c = contentPos - 1;
		Graphics::TextLayoutResult* r = Graphics::Helper::GetGlyphData(m_glyphRun);
		Graphics::FontGlyphLocation* loc = &r->Items[c].Location;
		return loc->OuterTopLeftPosition.X + loc->BitmapSize.Width;
	}


	void Replace(int start, int length, const uint32_t* text, int len)
	{
		m_utf32Text.Replace(start, length, text, len);
		UpdateGlyphRuns();
	}


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

	void Replace(const Selection* sel, const UTF32* text, int len)
	{
		int beginLineNumber, beginPosFromLineHead;
		int endLineNumber, endPosFromLineHead;
		GetLineNumber(sel->Start, &beginLineNumber, &beginPosFromLineHead);
		GetLineNumber(sel->Start + sel->Length, &endLineNumber, &endPosFromLineHead);
		if (beginLineNumber == endLineNumber) {
			m_lineSegments[beginLineNumber]->Replace(beginPosFromLineHead, sel->Length, text, len);
		}
		else
		{
			LN_THROW(0, NotImplementedException);
		}
	}

	// �w�肵���R���e���c�ʒu (�����ʒu) �����s�ڂ���Ԃ�
	void GetLineNumber(int contentPos, int* lineNumber, int* lenFromLineHead, int* yLocation = NULL)
	{
		int len = 0;
		int y = 0;
		for (int line = 0; line < m_lineSegments.GetCount(); ++line)
		{
			int head = len;
			len += m_lineSegments[line]->GetLength();
			if (contentPos <= len) {
				*lenFromLineHead = contentPos - head;
				if (yLocation != NULL) { *yLocation = y; }
				*lineNumber = line;
				return;
			}
			y += m_lineSegments[line]->GetLinePixelHeight();
		}
		LN_THROW(0, InvalidOperationException);
	}

	// calet ��`�悷��ׂ�����̍��W
	Point GetCaretLocation(int contentPos, int* curLineHeight)
	{
		int lineNumber, lenFromLineHead, yLocation;
		GetLineNumber(contentPos, &lineNumber, &lenFromLineHead, &yLocation);
		*curLineHeight = m_lineSegments[lineNumber]->GetLinePixelHeight();
		return Point(
			m_lineSegments[lineNumber]->GetPixelPosFromLineHead(lenFromLineHead),
			yLocation);
	}

	const Array< RefPtr<TextBox::LineSegment> >& GetLineSegments() const { return m_lineSegments; }

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
	, m_selection(NULL)
	, m_caretAnimation(NULL)
{
	m_document = LN_NEW Document();
	m_selection = LN_NEW Selection();
	m_caretAnimation = LN_NEW Animation::FloatAnimationCurve();
	m_caretAnimation->SetWrapMode(Animation::WrapMode_Loop);

	Animation::FloatKeyFrame key;
	key.Mode = Animation::InterpolationMode_None;
	key.FrameTime = 0.0;
	key.Value = 1.0f;
	m_caretAnimation->AddKeyFrame(key);
	key.FrameTime = 0.5;
	key.Value = 0.0f;
	m_caretAnimation->AddKeyFrame(key);
	key.FrameTime = 1.0;
	key.Value = 1.0f;
	m_caretAnimation->AddKeyFrame(key);

	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(TextBox, KeyEventArgs, UIElement::CharInputEvent, Handler_CharInput);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::~TextBox()
{
	LN_SAFE_RELEASE(m_caretAnimation);
	LN_SAFE_DELETE(m_selection);
	LN_SAFE_DELETE(m_document);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Select(int start, int length)
{
	m_selection->Start = start;
	m_selection->Length = length;
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

	m_caretAnimation->SetTime(m_manager->GetTime());

	// �L�����b�g
	int caretHeight;
	Point caretPos = m_document->GetCaretLocation(m_selection->Start + m_selection->Length, &caretHeight);
	RectF caret((float)caretPos.X, (float)caretPos.Y, 1.0f, (float)caretHeight);
	painter->SetBrush(Graphics::ColorBrush::Red);
	painter->SetOpacity(m_caretAnimation->GetValue());
	painter->DrawRectangle(caret);
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Handler_CharInput(KeyEventArgs* e)
{
	UTF32 buf[1] = { e->Char };
	m_document->Replace(m_selection, buf, 1);
	m_selection->Start++;
}

} // namespace GUI
} // namespace Lumino

