
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UIPanel.h>	// TODO: stackpanel �ɂ��邩��
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// UIListBoxItem
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIListBoxItem, UIContentControl)

//------------------------------------------------------------------------------
UIListBoxItem::UIListBoxItem()
{
}

//------------------------------------------------------------------------------
UIListBoxItem::~UIListBoxItem()
{
}

//------------------------------------------------------------------------------
void UIListBoxItem::Initialize(ln::detail::UIManager* manager)
{
	UIContentControl::Initialize(manager);
}

//==============================================================================
// UIListBox
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIListBox, UIItemsControl)

//------------------------------------------------------------------------------
UIListBoxPtr UIListBox::Create()
{
	auto ptr = UIListBoxPtr::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIListBox::UIListBox()
{
}

//------------------------------------------------------------------------------
UIListBox::~UIListBox()
{
}

//------------------------------------------------------------------------------
void UIListBox::Initialize(ln::detail::UIManager* manager)
{
	UIItemsControl::Initialize(manager);

	auto panel = RefPtr<UIPanel>::MakeRef();
	panel->Initialize(manager);
	SetItemsHostPanel(panel);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::AddTextItem(const String& text)
{
	auto textBlock = RefPtr<UITextBlock>::MakeRef();
	textBlock->Initialize(GetManager());
	textBlock->SetText(text);
	return AddItem(textBlock);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::AddItem(UIElement* item)
{
	LN_CHECK_ARG(item != nullptr);

	// �󂯎���� item �� UIListBoxItem �Ń��b�v���āAUIListBoxItem �����X�g�ɓ����
	auto listItem = RefPtr<UIListBoxItem>::MakeRef();
	listItem->Initialize(GetManager());
	listItem->SetContent(item);
	GetItems()->Add(listItem);
	return listItem;
}

} // namespace tr
LN_NAMESPACE_END
