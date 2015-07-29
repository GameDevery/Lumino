
#include "../Internal.h"
#include <Lumino/GUI/ContentControl.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ContentControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentControl, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentControl);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::ContentControl(GUIManager* manager)
	: Control(manager)
	, m_childElement(NULL)
	, m_contentPresenter(NULL)
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

	// Object �^�ł���� UIElement �ƂƂ��Ď��o���Ă݂�
	if (m_content.GetType() == VariantType_Object)
	{
		UIElement* element = dynamic_cast<UIElement*>(m_content.GetObject());
		if (element != NULL) {
			m_childElement = element;
		}
		LN_THROW(m_childElement->GetParent() == NULL, InvalidOperationException);	// ���ɐe�v�f��������
	}

	//m_content.SetFloat(0);

	// m_contentPresenter ������΂��̎��_�ŃZ�b�g���Ă��܂�
	if (m_childElement != NULL &&
		m_contentPresenter != NULL)
	{
		m_contentPresenter->SetContent(m_childElement);
		m_childElement->SetTemplateModified(true);
	}
	else {
		// m_contentPresenter �� NULL �ł���Ƃ������Ƃ́A�܂� VisualTree �̍\�z���I����Ă��Ȃ��Ƃ������ƁB
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::PollingTemplateChildCreated(UIElement* newElement)
{
	ContentPresenter* presenter = dynamic_cast<ContentPresenter*>(newElement);
	if (presenter != NULL)
	{
		m_contentPresenter = presenter;

		// m_childElement ������΂��̎��_�ŃZ�b�g���Ă��܂�
		if (m_childElement != NULL &&
			m_contentPresenter != NULL)
		{
			m_contentPresenter->SetContent(m_childElement);
		}
	}

}

#if 0
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
UIElement* ContentControl::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_childElement != NULL) {	// �q�v�f��D��
		UIElement* e = m_childElement->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::MeasureLayout(const SizeF& availableSize)
{
	if (m_childElement != NULL) {
		m_childElement->MeasureLayout(availableSize);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::MeasureLayout(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ArrangeLayout(const RectF& finalLocalRect)
{
	if (m_childElement != NULL) {
		m_childElement->ArrangeLayout(finalLocalRect);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::ArrangeLayout(finalLocalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::OnRender()
{
	if (m_childElement != NULL) {
		m_childElement->Render();	//TODO: Render ������Ă�ł�E�E�E
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ContentControl::OnEvent(EventType type, RoutedEventArgs* args)
{
	if (m_childElement != NULL) {
		if (m_childElement->OnEvent(type, args)) { return true; }
	}
	return Control::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	Control::ApplyTemplateHierarchy(parent);
	if (m_childElement != NULL) {
		m_childElement->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
	}
}
#endif

} // namespace GUI
} // namespace Lumino
