﻿#pragma once
#include "UIItemsElement.hpp"

namespace ln {

class UITabItem
	: public UICollectionItem
{
protected:
	virtual const String& elementName() const  override { static String name = u"UITabItem"; return name; }

LN_CONSTRUCT_ACCESS:
	UITabItem();
	void init();
};

class UITabBar
	: public UIItemsControl
{
public:
	void addTab(UITabItem* item);

LN_CONSTRUCT_ACCESS:
	UITabBar();
	void init();

protected:
    //virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    //virtual Size measureOverride(const Size& constraint) override;
    //virtual Size arrangeOverride(const Size& finalSize) override;
    //virtual void onRoutedEvent(UIEventArgs* e) override;

private:
};

} // namespace ln
