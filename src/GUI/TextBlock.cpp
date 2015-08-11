/*
	�ERichTextBox
	�E�A�j���[�V���� (�X�v���C�g�e�L�X�g)
	����2�́A�\������O���t�͓����B���������Ⴄ�����B

*/
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/TextBlock.h>
#include <Lumino/Documents/DocumentsRenderer.h>
#include <Lumino/Documents/Run.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// TextBlock
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBlock, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBlock);

LN_PROPERTY_IMPLEMENT(TextBlock, String, TextProperty, "Text", m_text, String::GetEmpty(), &TextBlock::OnTextPropertyChanged);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock* TextBlock::Create(GUIManager* manager)
{
	auto obj = RefPtr<TextBlock>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::TextBlock(GUIManager* manager)
	: UIElement(manager)
{
	m_paragraph = RefPtr<Documents::Paragraph>::Create(m_manager->GetDocumentsManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::~TextBlock()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void TextBlock::SetText(const String& text)
//{
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBlock::MeasureOverride(const SizeF& availableSize)
{
	SizeF size = UIElement::MeasureOverride(availableSize);
	Size parSize = m_paragraph->Measure();

	size.Width = std::max(parSize.Width, parSize.Width);
	size.Height = std::max(parSize.Height, parSize.Height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBlock::ArrangeOverride(const SizeF& finalSize)
{
	return UIElement::ArrangeOverride(finalSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBlock::OnRender(Graphics::Painter* painter)
{
	UIElement::OnRender(painter);
	Documents::RenderTargetDocumentsRenderer r(painter);
	m_paragraph->Render(&r);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBlock::OnTextPropertyChanged(PropertyChangedEventArgs* e)
{
	m_paragraph->GetInlines()->Clear();
	RefPtr<Documents::Run> run(LN_NEW Documents::Run(m_text, m_manager->GetDocumentsManager()));
	m_paragraph->GetInlines()->Add(run);
}

} // namespace GUI
} // namespace Lumino

