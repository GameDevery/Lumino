
#include "../Internal.h"
#include <Lumino/GUI/Control.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Control
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Control, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Control);

// Register property
LN_PROPERTY_IMPLEMENT(Control, ControlTemplate*, TemplateProperty, "Template", m_controlTemplate, NULL, NULL);

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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::SetTemplate(ControlTemplate* controlTemplate)
{
	m_controlTemplate = controlTemplate;
	SetTemplateModified(true);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternal()
//{
//	return ApplyTemplateInternalMain(ControlTemplateTypeName);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternalMain(const String& typeName)
//{
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(typeName, &t))
//	{
//		t->Apply(this);
//		return true;
//	}
//	return false;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::OnApplyTemplate(CombinedLocalResource* localResource)
{
	// ControlTemplate �̓K�p���� (WPF �ł� ApplyTemplate �ōs���Ă���)
	if (m_controlTemplate != NULL) {
		m_controlTemplate->Apply(this);
	}

	//ControlTemplate* t;
	//if (localResource->TryGetControlTemplate(GetTypeID(), &t)) {
	//	t->Apply(this);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Control::MeasureOverride(const SizeF& constraint)
{
	if (m_templateChild != NULL) {
		m_templateChild->MeasureLayout(constraint);
		return m_templateChild->GetDesiredSize();
	}
	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Control::ArrangeOverride(const SizeF& finalSize)
{
	if (m_templateChild != NULL) {
		m_templateChild->ArrangeLayout(RectF(PointF::Zero, finalSize));
	}
	return UIElement::ArrangeOverride(finalSize);
}

//void Control::ApplyTemplateHierarchy(CombinedLocalResource* parent)
//{
//	// ���[�J�����\�[�X���X�V����
//	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
//		m_combinedLocalResource->Combine(parent, m_localResource);
//	}
//	else {
//		LN_REFOBJ_SET(m_combinedLocalResource, parent);
//	}
//
//	// ControlTemplate �̓K�p����
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(GetTypeID(), &t)) {
//		t->Apply(this);
//	}
//
//	OnApplyTemplate(m_combinedLocalResource);
//
//	// �q�v�f
//	if (m_templateChild != NULL) {
//		m_templateChild->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
//	}
//}
//


} // namespace GUI
} // namespace Lumino
