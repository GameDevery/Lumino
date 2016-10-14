﻿
#include "Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "DocumentsManager.h"
#include "DocumentElements.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// Document
//==============================================================================

//------------------------------------------------------------------------------
Document::Document()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
Document::~Document()
{
}

//------------------------------------------------------------------------------
void Document::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
}


//==============================================================================
// TextElement
//==============================================================================

//------------------------------------------------------------------------------
TextElement::TextElement()
	: m_manager(nullptr)
	, m_fontDataModified(false)
	, m_position()
	, m_size()
	, m_margin()
	, m_padding()
	, m_anchor(AlignmentAnchor::None)
	, m_horizontalAlignment(HorizontalAlignment::Left)
	, m_verticalAlignment(VerticalAlignment::Top)
	, m_parent(nullptr)
	, m_desiredSize()
	, m_finalLocalRect()
{
}

//------------------------------------------------------------------------------
TextElement::~TextElement()
{
}

//------------------------------------------------------------------------------
void TextElement::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_fontData.Family = String::GetEmpty();
	m_fontData.Size = 20;
	m_fontData.IsBold = false;
	m_fontData.IsItalic = false;
	m_fontData.IsAntiAlias = true;
	m_fontDataModified = true;
}

//------------------------------------------------------------------------------
void TextElement::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
void TextElement::Render(IDocumentsRenderer* renderer)
{
}

//------------------------------------------------------------------------------
SizeF TextElement::MeasureOverride(const SizeF& constraint)
{
	if (m_fontDataModified)
	{
		OnFontDataChanged(m_fontData);
		m_fontDataModified = false;
	}
	return ILayoutElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
const PointF& TextElement::GetLayoutPosition() const { return m_position; }
const SizeF& TextElement::GetLayoutSize() const { return m_size; }
const ThicknessF& TextElement::GetLayoutMargin() const { return m_margin; }
const ThicknessF& TextElement::GetLayoutPadding() const { return m_padding; }
AlignmentAnchor TextElement::GetLayoutAnchor() const { return m_anchor; }
HorizontalAlignment TextElement::GetLayoutHorizontalAlignment() const { return m_horizontalAlignment; }
VerticalAlignment TextElement::GetLayoutVerticalAlignment() const { return m_verticalAlignment; }
ILayoutElement* TextElement::GetLayoutParent() const { return m_parent; }
//int TextElement::GetLayoutChildCount() const { return 0; }
//ILayoutElement* TextElement::GetLayoutChild(int index) const { return nullptr; }
HorizontalAlignment* TextElement::GetLayoutContentHorizontalAlignment() { return nullptr; }
VerticalAlignment* TextElement::GetLayoutContentVerticalAlignment() { return nullptr; }
const SizeF& TextElement::GetLayoutDesiredSize() const { return m_desiredSize; }
void TextElement::SetLayoutDesiredSize(const SizeF& size) { m_desiredSize = size; }
void TextElement::SetLayoutFinalLocalRect(const RectF& rect) { m_finalLocalRect = rect; }

//==============================================================================
// Paragraph
//==============================================================================

//------------------------------------------------------------------------------
Paragraph::Paragraph()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Paragraph::~Paragraph()
{
}

//------------------------------------------------------------------------------
void Paragraph::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}

//------------------------------------------------------------------------------
void Paragraph::AddInline(Inline* inl)
{
	LN_CHECK_ARG(inl != nullptr);
	LN_CHECK_ARG(inl->GetParent() == nullptr);
	m_inlines.Add(inl);
	inl->SetParent(this);
}

//------------------------------------------------------------------------------
void Paragraph::ClearInlines()
{
	for (Inline* inl : m_inlines) inl->SetParent(nullptr);
	m_inlines.Clear();
}

//------------------------------------------------------------------------------
void Paragraph::Render(IDocumentsRenderer* renderer)
{
	for (Inline* inl : m_inlines) inl->Render(renderer);
}

//==============================================================================
// Inline
//==============================================================================

//------------------------------------------------------------------------------
Inline::Inline()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Inline::~Inline()
{
}

//------------------------------------------------------------------------------
void Inline::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}


//==============================================================================
// Run
//==============================================================================

//------------------------------------------------------------------------------
Run::Run()
	: Inline()
{
}

//------------------------------------------------------------------------------
Run::~Run()
{
}

//------------------------------------------------------------------------------
void Run::Initialize(DocumentsManager* manager)
{
	Inline::Initialize(manager);

	// TODO: 本当に画面に表示されている分だけ作ればいろいろ節約できそう
	m_glyphRun = RefPtr<GlyphRun>::MakeRef();
	m_glyphRun->Initialize(manager->GetGraphicsManager());
}

//------------------------------------------------------------------------------
void Run::SetText(const StringRef& text)
{
	//m_text.Clear();
	//m_text.Append(GetManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), text.GetLength()));
	m_glyphRun->SetText(text);
}

//------------------------------------------------------------------------------
void Run::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
SizeF Run::MeasureOverride(const SizeF& constraint)
{
	SizeF size = Inline::MeasureOverride(constraint);
	const SizeI& runSize = m_glyphRun->GetRenderSize();

	size.width = std::max(size.width, (float)runSize.width);
	size.height = std::max(size.height, (float)runSize.height);
	return size;
}

//------------------------------------------------------------------------------
void Run::Render(IDocumentsRenderer* renderer)
{
	renderer->OnDrawGlyphRun(m_glyphRun, Point::Zero);
}

} // namespace detail
LN_NAMESPACE_END
