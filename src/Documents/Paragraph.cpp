
#include "../Internal.h"
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Documents/Paragraph.h>
#include "DocumentsHelper.h"

namespace Lumino
{
namespace Documents
{

//=============================================================================
// InlineList
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InlineList::OnItemAdded(const Variant& item)
{
	m_owner->OnInlineAdded(static_cast<Inline*>(item.GetObject()));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InlineList::OnItemRemoved(const Variant& item)
{
	m_owner->OnInlineRemoved(static_cast<Inline*>(item.GetObject()));
}

//=============================================================================
// Paragraph
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Paragraph, TextElement);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Paragraph::Paragraph(DocumentsManager* manager)
	: TextElement(manager)
{
	m_inlines.Attach(LN_NEW InlineList(this));

	// Paragraph �� GUI::TextBlock �̃��[�g�v�f�ɂ��Ȃ�̂ŁA�t�H���g�f�[�^�͂����ŏ��������Ă���
	Imaging::Font* font = m_manager->GetGraphicsManager()->GetFontManager()->GetDefaultFont();
	m_fontData.Family = font->GetName();
	m_fontData.Size = font->GetSize();
	m_fontData.EdgeSize = font->GetEdgeSize();
	m_fontData.IsBold = font->IsBold();
	m_fontData.IsItalic = font->IsItalic();
	m_fontData.IsAntiAlias = font->IsAntiAlias();
	m_fontDataModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Paragraph::~Paragraph()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size Paragraph::Measure()
{
	// TODO: �e�L�X�g���ς��Ȃ���Όv�Z����K�v�Ȃ��̂� modified �t���O�p�ӂ���ׂ�
	Size size = Size::Zero;
	for (Inline* inl : *m_inlines)
	{
		Size childSize = inl->Measure();

		switch (GetFlowDirection())
		{
		case Graphics::FlowDirection::LeftToRight:
			size.Width += childSize.Width;
			size.Height = std::max(childSize.Height, childSize.Height);
			break;
		case Graphics::FlowDirection::RightToLeft:
			size.Width = std::max(childSize.Width, childSize.Width);
			size.Height += childSize.Height;
			break;
		case Graphics::FlowDirection::TopToBottom:
			LN_THROW(0, NotImplementedException);
			break;
		}
	}
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Paragraph::OnInlineAdded(Inline* item)
{
	LN_ASSERT(item != NULL);
	Helper::TextElement_SetParent(item, this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Paragraph::OnInlineRemoved(Inline* item)
{
	LN_ASSERT(item != NULL);
	Helper::TextElement_SetParent(item, NULL);
}

} // namespace Documents
} // namespace Lumino

