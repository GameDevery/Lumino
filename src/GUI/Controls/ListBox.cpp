/*
	�EWPF
		ListBox
			Border	�g�y�єw�i
				ScrollViewer
					Grid
						Rectangle	����
						ScrollContentPresenter
							ItemsPresenter
								VirtualizingStackPanel
									ListBoxItem
									ListBoxItem
									ListBoxItem
										Border
											ContentPresenter
												<���낢��>
						ScrollBar	�c
						ScrollBar	��


	ListBoxItem �̘_���I�Ȗ�ڂ́A�e�� ListBox �Ƀ}�E�X�����`���邱�ƁB
	ListBox �� ListBoxItem ����̒ʒm���󂯎������A�����̐ݒ� (�����I���\����) ���l�����A
	���ꂼ��� Item �̏�Ԃ��X�V����B


	ItemsControl	�E�E�E Items, ItemsPanel
		MenuBase
		Selector
		StatusBar
		TreeView

	Selector		�E�E�E SelectedIndex
		ComboBox
		ListBox
		TabControl


	�܂��Ahttp://pro.art55.jp/?eid=1139229 �݂����ɃO���[�v�������ꍇ�́A

	ScrollViewer
		Grid
			ScrollContentPresenter
				ItemsPresenter
					StackPanel
						GroupItem
							..expander�Ƃ�...
							ContentPresenter
								ItemsPresenter
									ListBoxItem
									ListBoxItem
						GroupItem
						GroupItem
	�ƂȂ��Ă����B



	ItemsSource �� NULL �̏ꍇ�́A
	Items �ւ̒��ڃR���g���[��Add ��������B
	�\�[�g���E�E�E

	�Ƃ������AItemsSource �� NULL ���ǂ�����
	�����I�ȃ��[�h����������ς��悤�Ȃ��񂶁B


	WPF �̃A�C�e�������܂ł̗���

	�EItemContainerGenerator.PrepareGrouping() �ŁAowner �� ListBox.View (ItemsSource���ۂ�) ���擾�B
	�EItemContainerGenerator �� ���Ŏ擾���� View �� OnCollectionChanged ��o�^�B
	�E�R���N�V�����̕ύX�ʒm���󂯎��ƁAGenerator �� ListBox.GetContainerForItem() ���Ăяo���B
	�EListBox.GetContainerForItem() �� ListBoxItem ������ĕԂ��B
	�EitemContainerGenerator.ItemsChanged �C�x���g�𔭐�������B(���ʂ� CLR �C�x���g)
	�E���̃C�x���g�� Panel.OnItemsChanged �ŃL���b�`����B���̐�� �_���c���[�iPanel �� _uiElementCollection�j�ɒǉ������B

	�E�E�E���A�ŏ��̂������炱��ȕ��G�ɂ͂������Ȃ��E�E�E�E�E�B


	���Ȃ݂ɁATreeView �̏ꍇ�� TreeViewItem �� ItemsControl�B
	���̎d�g�݂��m�[�h�̐������l�X�Ƃ���邱�ƂɂȂ�B


	Panel �� Owner �������AOwner �� Item ������Ă��炤�̂͂����Ǝv���B
	���� Style��N���K�p����̂��B


	��Group

		ListBox
			�c
				ItemsPresenter
					StackPanel
						GroupItem
						GroupItem
						GroupItem
							StackPanel
								ContentPresenter (PART_Header)
									TextBlock
								ItemsPresenter (ItemsPresenter)
									VirtualizingStackPanel
										ListBoxItem
										ListBoxItem
										ListBoxItem
									

*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ListBox.h>
#include <Lumino/GUI/Controls/StackPanel.h>
#include <Lumino/GUI/TextBlock.h>
#include "../GUIHelper.h"

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ListBoxItem
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBoxItem, ItemsControlItem);
LN_UI_ELEMENT_SUBCLASS_IMPL(ListBoxItem);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItem* ListBoxItem::Create(GUIManager* manager)
{
	auto obj = RefPtr<ListBoxItem>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItem::ListBoxItem(GUIManager* manager)
	: ItemsControlItem(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItem::~ListBoxItem()
{
}

//=============================================================================
// ListBoxItemList
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBoxItemList, GenericVariantList<ListBoxItem*>);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxItemList::OnItemAdded(ListBoxItem* item)
{
	m_owner->OnListBoxItemAdded(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxItemList::OnItemRemoved(ListBoxItem* item)
{
	m_owner->OnListBoxItemRemoved(item);
}

//=============================================================================
// ListBoxChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBoxChrome, Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(ListBoxChrome);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxChromePtr ListBoxChrome::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxChrome::ListBoxChrome(GUIManager* manager)
	: Decorator(manager)
	, m_frameWidth(8)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxChrome::~ListBoxChrome()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_frameBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ListBoxNormalFrameBrush")));
	m_bgBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ListBoxNormalBackgroundBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBoxChrome::OnRender(Graphics::Painter* painter)
{
	RectF bgRect = m_finalLocalRect;
	RectF rect = m_finalLocalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter->SetBrush(m_bgBrush);
		painter->DrawRectangle(bgRect);
	}

	painter->SetBrush(m_frameBrush);
	painter->DrawFrameRectangle(rect, m_frameWidth);
}


//=============================================================================
// ListBox
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ListBox, ItemsControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ListBox);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxPtr ListBox::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBox::ListBox(GUIManager* manager)
	: ItemsControl(manager)
	//, m_listBoxItems(LN_NEW ListBoxItemList(this))
{
	//m_itemsPanel.Attach(LN_NEW StackPanel(manager));
	//AddVisualChild(m_itemsPanel);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBox::~ListBox()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ListBox::InsertTextItem(int index, const String& text)
//{
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ListBoxItemPtr ListBox::AddTextItem(const String& text)
{
	auto textBlock = GUIHelper::CreateUIElemenInstance<TextBlock>(m_manager);
	textBlock->SetText(text);
	auto item = GUIHelper::CreateUIElemenInstance<ListBoxItem>(m_manager);
	item->SetContent(textBlock);
	GetItems()->AddVariant(item);
	return item;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//ListBoxItemPtr ListBox::AddItem(const UIElement& item)
//{
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::InsertListBoxItem(int index, UIElement* element)
{
	RefPtr<ListBoxItem> item(ListBoxItem::Create(m_manager));
	item->SetContent(element);
	GetItems()->AddVariant(item);	// TODO: �C���f�b�N�X
	//m_listBoxItems->Insert(index, item);
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* ListBox::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_itemsPanel != NULL) {	// �q�v�f��D��
		UIElement* e = m_itemsPanel->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	Control::ApplyTemplateHierarchy(parent);
	if (m_itemsPanel != NULL) {
		m_itemsPanel->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::MeasureLayout(const SizeF& availableSize)
{
	if (m_itemsPanel != NULL) {
		m_itemsPanel->MeasureLayout(availableSize);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::MeasureLayout(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::ArrangeLayout(const RectF& finalLocalRect)
{
	if (m_itemsPanel != NULL) {
		m_itemsPanel->ArrangeLayout(finalLocalRect);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::ArrangeLayout(finalLocalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ListBox::OnEvent(EventType type, RoutedEventArgs* args)
{
	if (m_itemsPanel != NULL) {
		if (m_itemsPanel->OnEvent(type, args)) { return true; }
	}
	return Control::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::Render()
{
	Control::Render();
	if (m_itemsPanel != NULL) {
		m_itemsPanel->Render();
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::OnListBoxItemAdded(ListBoxItem* item)
{
	m_itemsPanel->GetChildren()->Add(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListBox::OnListBoxItemRemoved(ListBoxItem* item)
{
	LN_THROW(0, NotImplementedException);
	//m_itemsPanel->GetChildren()->Remove(item);
}

} // namespace GUI
} // namespace Lumino

