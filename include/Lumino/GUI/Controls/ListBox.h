
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Panel.h"
#include "../ContentControl.h"
#include "../ItemsControl.h"
#include "../Decorator.h"

namespace Lumino
{
namespace GUI
{
class ListBox;

/**
	@brief
*/
class ListBoxItem
	: public ItemsControlItem
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ListBoxItem);

public:
	static ListBoxItem* Create(GUIManager* manager);

	ListBoxItem(GUIManager* manager);
	virtual ~ListBoxItem();

private:
	void Handler_MouseEnter(MouseEventArgs* e);
	void Handler_MouseLeave(MouseEventArgs* e);
};


/**
	@brief
*/
class ListBoxItemList	// TODO: ����Ȃ�����
	: public GenericVariantList<ListBoxItem*>
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:

	ListBoxItemList(ListBox* owner) : m_owner(owner) {}
	virtual ~ListBoxItemList() {}

protected:
	virtual void OnItemAdded(ListBoxItem* item);
	virtual void OnItemRemoved(ListBoxItem* item);

private:
	ListBox*	m_owner;
};

/**
	@brief		ListBox �R���g���[���̊O�ςł��B
*/
class ListBoxChrome
	: public Decorator
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ListBoxChrome);
public:
	//static const String	IsMouseOverProperty;	///< IsMouseOver �v���p�e�B�̎��ʎq
	//static const String	FrameWidthProperty;		///< FrameWidth �v���p�e�B�̎��ʎq

public:
	static ListBoxChromePtr Create();

	ListBoxChrome(GUIManager* manager);
	virtual ~ListBoxChrome();

public:
	// Property
	//void SetMouseOver(bool value) { m_isMouseOver = value; }
	//bool IsMouseOver() const { return m_isMouseOver; }

	///// �{�^���C���[�W�̊O�g�̕���ݒ肵�܂��B
	//void SetFrameWidth(float width) { m_frameWidth = width; }

	///// �{�^���C���[�W�̊O�g�̕����擾���܂��B
	//float GetFrameWidth() const { return m_frameWidth; }

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void OnRender(Graphics::Painter* painter);

private:
	RefPtr<Graphics::TextureBrush>	m_frameBrush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;
	bool							m_isMouseOver;
};



/**
	@brief		
*/
class ListBox
	: public ItemsControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ListBox);
public:

	/**
		@brief	ListBox ���쐬���܂��B	
	*/
	static ListBoxPtr Create();


public:

	/**
		@brief	�w�肵������������X�g�{�b�N�X�̍��ڂƂ��Ēǉ����܂��B	
	*/
	ListBoxItemPtr AddTextItem(const String& text);
	ListBoxItemPtr AddItem(const UIElement& item);

	

	/**
		@brief	�w�肵������������X�g�{�b�N�X�̍��ڂƂ��Ēǉ����܂��B	
	*/
	//void SetItemGroup(int index, const Variant& groupKey);



	// TODO: ������ protected �ɂ���ׂ�
	ListBox(GUIManager* manager);
	virtual ~ListBox();

	//ListBoxItemList* GetListBoxItems() { return m_listBoxItems; }

	// ���[�e�B���e�B
	void InsertListBoxItem(int index, UIElement* element);

	//ItemsPanel
protected:
#if 0
	// �q�v�f�����v�f�� override
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);
	virtual bool OnEvent(EventType type, RoutedEventArgs* args);
	virtual void Render();
#endif

	// ListBoxItemList callback
	friend class ListBoxItemList;
	void OnListBoxItemAdded(ListBoxItem* item);
	void OnListBoxItemRemoved(ListBoxItem* item);

	//virtual void Render();
	//virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	//virtual bool OnEvent(EventType type, RoutedEventArgs* args);
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);

protected:
	//RefPtr<ListBoxItemList>	m_listBoxItems;		///< logical children
	RefPtr<Panel>			m_itemsPanel;		///< ListBoxItem ���z�u����� Panel


	// ListBox �̃X�^�C���ƃe���v���[�g
	// https://msdn.microsoft.com/ja-jp/library/ms754242%28v=vs.110%29.aspx
};


} // namespace GUI
} // namespace Lumino
