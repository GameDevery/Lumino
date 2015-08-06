
#include "../Internal.h"
#include <Lumino/GUI/Decorator.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Decorator
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Decorator, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Decorator);

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
	// ���ɕʂ̎q�v�f�ƂȂ��Ă���ꍇ�̓G���[
	if (element != NULL && element->GetParent() != NULL) {
		LN_THROW(0, InvalidOperationException, _T("Specified element is already a child element of another element."));
	}

	m_child = element;

	if (m_child != NULL)
	{
		m_child->SetParent(this);
		m_child->SetTemplateModified(true);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::GetChild() const
{
	return m_child;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::AddChild(const Variant& value)
{
	UIElement* e = dynamic_cast<UIElement*>(value.GetObject());
	if (e == NULL) {
		LN_THROW(0, NotImplementedException);
	}
	SetChild(e);
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_child != NULL) {	// �q�v�f��D��
		UIElement* e = m_child->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Decorator::AddVisualChild(UIElement* child)
//{
//	// Decorator �� LogicalTree = VisualTree �Ȃ͂��Ȃ̂ł���ł����͂��E�E�E
//	SetChild(child);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);
	if (m_child != NULL) {
		m_child->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
	}
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Decorator::AddChildToVisualTree(UIElement* element)
//{
//
//}
#endif

} // namespace GUI
} // namespace Lumino