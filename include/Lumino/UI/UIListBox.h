
#pragma once
#include "UIItemsControl.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class UIListBoxItem;
class UIListBox;
using UIListBoxItemPtr = RefPtr<UIListBoxItem>;
using UIListBoxPtr = RefPtr<UIListBox>;

/**
	@brief
	@note		ListBoxItem �̘_���I�Ȗ�ڂ́A�e�� ListBox �Ƀ}�E�X�����`���邱�ƁB
				ListBox �� ListBoxItem ����̒ʒm���󂯎������A�����̐ݒ� (�����I���\����) ���l�����A
				���ꂼ��� Item �̏�Ԃ��X�V����B
*/
class UIListBoxItem
	: public UIContentControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static const String NormalState;
	static const String MouseOverState;

LN_PROTECTED_INTERNAL_ACCESS:
	UIListBoxItem();
	virtual ~UIListBoxItem();
	void Initialize(ln::detail::UIManager* manager);

protected:
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
};

/**
	@brief
*/
class UIListBox
	: public UIItemsControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UIListBoxPtr Create();

public:
	UIListBoxItemPtr AddTextItem(const String& text);
	UIListBoxItemPtr AddItem(UIElement* item);

LN_PROTECTED_INTERNAL_ACCESS:
	UIListBox();
	virtual ~UIListBox();
	void Initialize(ln::detail::UIManager* manager);

private:
};

} // namespace tr
LN_NAMESPACE_END
