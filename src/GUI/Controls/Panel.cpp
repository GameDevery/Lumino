
#include "../../Internal.h"
#include <Lumino/GUI/Controls/Panel.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Panel
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Panel, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Panel);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Panel::Panel(GUIManager* manager)
	: UIElement(manager)
	, m_children(LN_NEW UIElementCollection())
{
	m_children->ItemAdded += LN_CreateDelegate(this, &Panel::Children_ItemAdded);
	m_children->ItemRemoved += LN_CreateDelegate(this, &Panel::Children_ItemRemoved);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Panel::~Panel()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::Children_ItemAdded(UIElement* item)
{
	// �q�v�f�� VisualTree ���X�V���Ȃ���
	if (item != NULL)
	{
		LN_THROW(item->GetParent() == NULL, InvalidOperationException);	// ���ɐe�v�f��������

		// TODO:�e�v�f�ɒǉ�����Ƃ��̋��ʏ���������AUIElementList �ɂ܂Ƃ߂Ă�������
		m_visualChildren.Add(item);
		item->SetParent(this);
		item->ApplyTemplate();


		//m_visualChildren.Add(item);
		//AddChild(item);
		//item->ApplyTemplate();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::Children_ItemRemoved(UIElement* item)
{
	item->SetParent(NULL);
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Panel::CheckMouseHoverElement(const PointF& globalPt)
{
	// �q�v�f��D��
	LN_FOREACH(UIElement* child, *m_children) {
		UIElement* e = child->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}

	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::Render()
{
	OnRender();
	LN_FOREACH(UIElement* child, *m_children) {
		child->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Panel::OnEvent(EventType type, EventArgs* args)
{
	LN_FOREACH(UIElement* child, *m_children) {
		if (child->OnEvent(type, args)) { return true; }
	}
	return UIElement::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);
	LN_FOREACH(UIElement* child, *m_children) {
		child->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
	}
}
#endif

} // namespace GUI
} // namespace Lumino

