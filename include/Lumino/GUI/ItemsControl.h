
#pragma once
#include "Control.h"
#include "ItemsPresenter.h"
#include "Controls/Panel.h"

namespace Lumino
{
namespace GUI
{

LN_ENUM(ListChangedAction)
{
	Add	= 0,	///< 1�ȏ�̗v�f�����X�g�ɒǉ����ꂽ
	Remove,		///< 1�ȏ�̗v�f�����X�g�����菜���ꂽ
	Replace,	///< 1�ȏ�̗v�f���ύX���ꂽ
	Move,		///< 1�ȏ�̗v�f�����X�g�����ړ�����
};
LN_ENUM_DECLARE(ListChangedAction);

class EventArgs
	: public CoreObject
{

};

class ListChangedEventArgs
	: public EventArgs
{
public:
	ListChangedEventArgs(ListChangedAction action)
	{
		Action = action;
	}

public:
	ListChangedAction	Action;
	VariantList			NewItems;
};
	





///**
//	@brief
//*/
//class ListModel
//	: public CoreObject
//{
//public:
//	/// �����̊�ɂȂ郊�X�g��ݒ肵�܂��B
//	void SetSourceList(VariantList* list) { m_list = list; }
//
//	bool CanFilter() const { return false; }	// TODO
//
//	bool CanGroup() const;
//
//	bool CanSort() const { return false; }		// TODO
//
//
//	// ���[�e�B���e�B
//	void AddGroup(const String& name);
//
//private:
//	RefPtr<VariantList>	m_list;
//};
//
//// ItemSource �� NULL �̎��A�����I�Ɏg����B
//class InternalItemViewModel
//{
//
//};


/**
	@brief
*/
class ItemList
	: public GenericVariantList<UIElementPtr>
{
public:
	Event01<ListChangedEventArgs*>	ItemsChanged;

protected:
	virtual void OnItemAdded(const Variant& item)	// TODO �C���f�b�N�X����肽������
	{
		auto args = RefPtr<ListChangedEventArgs>::Create(ListChangedAction::Add);	// TODO �L���b�V��������
		args->NewItems.AddVariant(item);
		ItemsChanged(args);
	}
	virtual void OnItemRemoved(const Variant& item) 
	{
		//auto args = RefPtr<ListChangedEventArgs>::Create(ListChangedAction::Remove);
		//ItemsChanged(args);
	}

private:
};

/**
	@brief
*/
class ItemsControlItem
	: public ContentControl
{
public:
	void SetGroup(const Variant& group);
	const Variant& GetGroup() const;

protected:
	ItemsControlItem(GUIManager* manager);
	virtual ~ItemsControlItem();

private:
	
};

/**
	@brief
*/
class GroupItem
	: public ContentControl
{
protected:
	GroupItem(GUIManager* manager);
	virtual ~GroupItem();

private:
	
};


/**
	@brief
*/
class ItemsControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsControl);
public:
	LN_PROPERTY(ControlTemplate*,	ItemsPanelTemplateProperty);
	LN_PROPERTY(Style*,				GroupStyleProperty);			// TODO: StylePtr �ɂł��Ȃ��H

public:
	static ItemsControl* Create(GUIManager* manager);

	ItemsControl(GUIManager* manager);
	virtual ~ItemsControl();

	void SetItemsPanelTemplate(ControlTemplate* controlTemplate) { m_itemsPanelTemplate = controlTemplate; }
	ControlTemplate* GetItemsPanelTemplate() const { return m_itemsPanelTemplate; }

	//void SetGroupStyle(Style* style) { SetTypedPropertyValue(GroupStyleProperty, style); }

	ItemList* GetItems() const { return m_itemList; }

private:

	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

	void Items_ListChanged(ListChangedEventArgs* e);

	void OnItemGroupKeyChanged(ItemsControlItem* item);

private:
	RefPtr<ControlTemplate>	m_itemsPanelTemplate;
	ItemsPresenter*			m_visualItemsPresenter;	///< VisualTree ���� ItemsPresenter
	RefPtr<Panel>			m_hostPanel;			///< �A�C�e���R���g���[���̒ǉ���
	RefPtr<ItemList>		m_itemList;

	StylePtr				m_groupStyle;

	class ItemInfo
	{

	};
};


} // namespace GUI
} // namespace Lumino
