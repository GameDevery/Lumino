
#pragma once
#include "UIItemsControl.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
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
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	virtual void onRender(DrawingContext* g) override
	{
		UIControl::onRender(g);
	}
LN_PROTECTED_INTERNAL_ACCESS:
	UIListBoxItem();
	virtual ~UIListBoxItem();
	void initialize();
};

/**
	@brief
*/
class UIListBox
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static const String NormalState;

public:
	static UIListBoxPtr create();

public:
	UIListBoxItemPtr addTextItem(const String& text);
	UIListBoxItemPtr addItem(UIElement* item);

LN_PROTECTED_INTERNAL_ACCESS:
	UIListBox();
	virtual ~UIListBox();
	void initialize();

private:
};

LN_NAMESPACE_END
