
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
	
/**
	@brief
*/
class ItemList
	: public VariantList
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
class ItemsControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsControl);
public:
	static const Property*	ItemsPanelTemplateProperty;

public:
	static ItemsControl* Create(GUIManager* manager);

	ItemsControl(GUIManager* manager);
	virtual ~ItemsControl();

	void SetItemsPanelTemplate(ControlTemplate* controlTemplate) { m_itemsPanelTemplate = controlTemplate; }
	ControlTemplate* GetItemsPanelTemplate() const { return m_itemsPanelTemplate; }

	ItemList* GetItems() const { return m_itemList; }

private:

	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

	void Items_ListChanged(ListChangedEventArgs* e);

private:
	RefPtr<ControlTemplate>	m_itemsPanelTemplate;
	ItemsPresenter*			m_visualItemsPresenter;	///< VisualTree ���� ItemsPresenter
	RefPtr<Panel>			m_hostPanel;			///< �A�C�e���R���g���[���̒ǉ���
	RefPtr<ItemList>		m_itemList;
};


} // namespace GUI
} // namespace Lumino
