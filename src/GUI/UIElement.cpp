
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/UIElement.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElement
//=============================================================================

const String UIElement::SizeProperty(_T("Size"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManager* manager)
	: m_manager(manager)
{
	LN_SAFE_ADDREF(m_manager);
	RegisterProperty(SizeProperty, SizeF(NAN, NAN));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::~UIElement()
{
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::RegisterProperty(const String& propertyName, const Variant& defaultValue)
{
	m_propertyDataStore.Add(propertyName, defaultValue);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetValue(const String& propertyName, const Variant& value)
{
	// TODO: �L�[��������Η�O
	m_propertyDataStore.SetValue(propertyName, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant UIElement::GetValue(const String& propertyName) const
{
	Variant value;
	if (m_propertyDataStore.TryGetValue(propertyName, &value)) {
		return value;
	}
	LN_THROW(0, ArgumentException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// �e�v�f����q�v�f��z�u�ł���͈�(availableSize)���󂯎��ADesiredSize ���X�V����B
	// �@ Pane �\[measure()   �c ���͈͓̔��Ȃ�z�u�ł����]�� Button
	// �A Pane ��[DesiredSize �c ���Ⴀ���̃T�C�Y�ł��肢���܂�]�\ Button
	// �B Pane �\[arrange()   �c ���̎q�v�f�Ƃ̌��ˍ����ōŏI�T�C�Y�̓R����]�� Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	const SizeF& size = GetSize();
	m_desiredSize.Width = std::min(size.Width, availableSize.Width);
	m_desiredSize.Height = std::min(size.Height, availableSize.Height);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalRect)
{
	m_finalRect = finalRect;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size = GetSize();

	// �T�C�Y����܂��Ă��Ȃ��ꍇ�̓��C�A�E�g������ł��Ȃ�
	if (Math::IsNaNOrInf(size.Width) || Math::IsNaNOrInf(size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	OnRender();
}

//=============================================================================
// Decorator
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Decorator::Decorator(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Decorator::~Decorator()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::SetChild(UIElement* element)
{
	m_child = element;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::GetChild() const
{
	return m_child;
}


//=============================================================================
// ButtonChrome
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_bgMargin(2)
{
	printf("TODO:");
	m_brush.Attach(LN_NEW Graphics::TextureBrush());
	m_brush->Create(_T("D:/Proj/Lumino/src/GUI/Resource/001-Blue01.png"), m_manager->GetGraphicsManager());	// TODO
	m_brush->SetSourceRect(Rect(128, 0, 64, 64));

	m_bgBrush.Attach(LN_NEW Graphics::TextureBrush());
	m_bgBrush->SetTexture(m_brush->GetTexture());
	m_bgBrush->SetSourceRect(Rect(0, 0, 128, 128));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::~ButtonChrome()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnRender()
{
	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO

	RectF rect = m_finalRect;


	painter.SetBrush(m_bgBrush);
	painter.DrawRectangle(rect);

	painter.SetBrush(m_brush);
	painter.DrawFrameRectangle(rect, 8);
}


//=============================================================================
// Control
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::Control(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::~Control()
{

}

//=============================================================================
// ContentControl
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::ContentControl(GUIManager* manager)
	: Control(manager)
	, m_childElement(NULL)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::~ContentControl()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::SetContent(Variant value)
{
	m_content = value;

	if (m_content.GetType() == VariantType_Object)
	{
		m_childElement = dynamic_cast<UIElement*>(m_content.GetObject());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::Render()
{
	OnRender();
	if (m_childElement != NULL) {
		m_childElement->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::MeasureLayout(const SizeF& availableSize)
{
	m_childElement->MeasureLayout(availableSize);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	Control::MeasureLayout(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ArrangeLayout(const RectF& finalRect)
{
	m_childElement->ArrangeLayout(finalRect);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	Control::ArrangeLayout(finalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::OnRender()
{
	if (m_childElement != NULL) {
		m_childElement->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ContentControl::OnEvent(EventType type, EventArgs* args)
{
	if (m_childElement != NULL) {
		if (m_childElement->OnEvent(type, args)) { return true; }
	}
	return false;
}

//=============================================================================
// RootPane
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane::RootPane(GUIManager* manager)
	: ContentControl(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane::~RootPane()
{

}

//=============================================================================
// Button
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::Button(GUIManager* manager)
	: ContentControl(manager)
{


	m_chrome.Attach(LN_NEW ButtonChrome(manager));
	SetContent(Variant(m_chrome));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::~Button()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnClick()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Button::OnEvent(EventType type, EventArgs* args)
{


	//ContentControl::OnEvent(type, args)

	//if (m_childElement != NULL) {
	//	if (m_childElement->OnEvent(type, args)) { return true; }
	//}
	//return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnRender()
{
}

} // namespace GUI
} // namespace Lumino
