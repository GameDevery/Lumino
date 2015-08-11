
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ItemsControl.h>
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{
	

//=============================================================================
// ItemsControlItem
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsControlItem, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsControlItem);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControlItem::ItemsControlItem(GUIManager* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControlItem::~ItemsControlItem()
{

}

//=============================================================================
// GroupItemList
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItemList::GroupItemList(ItemsControl* owner)
	: m_owner(owner)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GroupItemList::OnItemAdded(const Variant& item)
{
	m_owner->OnGroupItemAdded(Variant::Cast<GroupItem*>(item));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GroupItemList::OnItemRemoved(const Variant& item)
{
	m_owner->OnGroupItemRemoved(Variant::Cast<GroupItem*>(item));
}

//=============================================================================
// ItemsControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsControl, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsControl); 

// Register property
LN_PROPERTY_IMPLEMENT(ItemsControl, ControlTemplate*, ItemsPanelTemplateProperty, "ItemsPanelTemplate", m_itemsPanelTemplate, NULL, NULL);
LN_PROPERTY_IMPLEMENT(ItemsControl, Style*, GroupStyleProperty, "GroupStyle", m_groupStyle, NULL, NULL);

/*	Measure/Arrange �̓x�[�X�� Control �N���X�ɔC����̂ŕK�v�Ȃ�
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl* ItemsControl::Create(GUIManager* manager)
{
	auto obj = RefPtr<ItemsControl>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl::ItemsControl(GUIManager* manager)
	: Control(manager)
	, m_visualItemsPresenter(NULL)
	, m_itemList()
{
	m_itemList.Attach(LN_NEW ItemList());
	m_itemList->ItemsChanged += LN_CreateDelegate(this, &ItemsControl::Items_ListChanged);

	m_groupItemList.Attach(LN_NEW GroupItemList(this));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl::~ItemsControl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItemPtr ItemsControl::AddGroup(const String& caption)
{
	auto group = GUIHelper::CreateUIElemenInstance<GroupItem>(m_manager);
	group->SetHeader(caption);
	m_groupItemList->Add(group);
	return group;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnApplyTemplate(CombinedLocalResource* localResource)
{
	Control::OnApplyTemplate(localResource);

	// m_itemsPanelTemplate �� m_visualItemsPresenter �͕K�������Ȃ���΂Ȃ�Ȃ�
	LN_THROW(m_itemsPanelTemplate != NULL, InvalidOperationException);
	LN_THROW(m_visualItemsPresenter != NULL, InvalidOperationException);



	RefreshHostPanel();
	RefreshHostPanelItems();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::PollingTemplateChildCreated(UIElement* newElement)
{
	auto ip = dynamic_cast<ItemsPresenter*>(newElement);
	if (ip != NULL) {
		m_visualItemsPresenter = ip;
	}
	Control::PollingTemplateChildCreated(newElement);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SizeF ItemsControl::MeasureOverride(const SizeF& constraint)
//{
//	// TODO ������ƓK��
//	if (!m_visualChildren.IsEmpty()) {
//		m_visualChildren[0]->MeasureLayout(constraint);
//	}
//	return Control::MeasureOverride(constraint);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SizeF ItemsControl::ArrangeOverride(const SizeF& finalSize)
//{
//	// TODO ������ƓK��
//	if (!m_visualChildren.IsEmpty()) {
//		m_visualChildren[0]->ArrangeOverride(finalSize);
//	}
//	return Control::ArrangeOverride(finalSize);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::Items_ListChanged(ListChangedEventArgs* e)
{
	if (e->Action == ListChangedAction::Add)
	{
		RefreshHostPanelItems();	// TODO: �x���]���ɂ�����
		//m_hostPanel->AddChild(e->NewItems.GetAtVariant(0));
	}
	else
	{
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnItemGroupKeyChanged(ItemsControlItem* item)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnGroupItemAdded(GroupItem* groupItem)
{
	RefreshHostPanelItems();	// TODO: �x���]���ɂ�����
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnGroupItemRemoved(GroupItem* groupItem)
{
	RefreshHostPanelItems();	// TODO: �x���]���ɂ�����
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::RefreshHostPanel()
{
	// �Â��̂���菜��
	if (m_hostPanel != NULL) {
		m_visualItemsPresenter->AttachHostPanel(NULL);
	}

	// �A�C�e����ێ����� Panel �����
	// TODO: Template �Ɋ֐��|�C���^���悤�ɂ��Ă����Ώ��������ł���
	RefPtr<CoreObject> obj(m_manager->CreateObject(m_itemsPanelTemplate->GetTargetType()));
	auto panel = dynamic_cast<Panel*>(obj.GetObjectPtr());
	LN_THROW(panel != NULL, InvalidOperationException);		// Panel �̃T�u�N���X�łȂ���΂Ȃ�Ȃ�
	m_hostPanel = panel;

	if (m_hostPanel != NULL) {
		m_visualItemsPresenter->AttachHostPanel(m_hostPanel);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::RefreshHostPanelItems()
{
	if (m_hostPanel != NULL)
	{
		auto childlen = m_hostPanel->GetChildren();
		childlen->Clear();

		if (IsGrouping())
		{
			for (GroupItem* group : *m_groupItemList)
			{
				childlen->Add(group);
			}
		}
		else
		{
			for (UIElement* element : *m_itemList)
			{
				childlen->Add(element);
			}
		}
	}
}

//=============================================================================
// GroupItem
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(GroupItem, ItemsControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(GroupItem);

// Register property
LN_PROPERTY_IMPLEMENT(GroupItem, Variant, HeaderProperty, "Header", m_header, NULL, NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItem::GroupItem(GUIManager* manager)
	: ItemsControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GroupItem::~GroupItem()
{
}


} // namespace GUI
} // namespace Lumino

