﻿#pragma once
#include "../UIItemsElement.hpp"

namespace ln {
class UIPopup;
class UILayoutPanel2;

class UIComboBoxItem
    : public UICollectionItem
{
public:
    virtual const String& elementName() const  override { static String name = u"UIComboBoxItem"; return name; }
    // TODO: group


    bool onHitTest(const Point& frameClientPosition) override
    {
        bool r = UICollectionItem::onHitTest(frameClientPosition);
        if (r) {
            printf("");
        }
        return r;
    }

LN_CONSTRUCT_ACCESS:
    UIComboBoxItem();
    void init();

private:
};

class UIComboBox
	: public UIItemsControl
{
public:

protected:
    virtual const String& elementName() const  override { static String name = u"UIComboBox"; return name; }
    virtual void onAddChild(UIElement* child) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual void onUpdateLayout(UILayoutContext* layoutContext);
	virtual void onRender(UIRenderingContext* context) override;

	virtual void onItemClick(UICollectionItem* item, UIClickEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
    UIComboBox();
	void init();

private:
    Ref<UIPopup> m_popup;
	Ref<UILayoutPanel2> m_itemsHost;
};

} // namespace ln
