﻿
#include "Internal.h"
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UITreeView.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UITreeViewItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITreeViewItem, UIHeaderedItemsControl)

//------------------------------------------------------------------------------
UITreeViewItem::UITreeViewItem()
{
}

//------------------------------------------------------------------------------
UITreeViewItem::~UITreeViewItem()
{
}

//------------------------------------------------------------------------------
void UITreeViewItem::initialize()
{
	UIHeaderedItemsControl::initialize();

	setHContentAlignment(HAlignment::Stretch);

	auto panel = newObject<UIStackPanel>();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	panel->setVisibility(UIVisibility::Collapsed);	// default close
	setLayoutPanel(panel);
	
	goToVisualState(UIVisualStates::NormalState);

	m_expander = newObject<UIToggleButton>();
	m_expander->setSize(Size(16, 16));	// TODO:
	m_expander->setStyleSubControlName(_T("UITreeViewItem"), _T("ExpanderButton"));
	m_expander->connectOnChecked(createDelegate(this, &UITreeViewItem::expander_OnChecked));
	m_expander->connectOnUnchecked(createDelegate(this, &UITreeViewItem::expander_OnUnchecked));
	addVisualChild(m_expander);

	// TODO:
	//setBackground(Brush::Blue);
}

////------------------------------------------------------------------------------
//void UITreeViewItem::setHeader(UIElement* header)
//{
//	removeVisualChild(m_header);
//
//	m_header = header;
//	m_header->setBackground(Brush::Green);
//	m_header->setHeight(16);	// TODO:
//
//	if (m_header != nullptr)
//	{
//		addVisualChild(m_header);
//	}
//}

//------------------------------------------------------------------------------
void UITreeViewItem::setExpanded(bool expand)
{
	m_expander->setChecked(expand);
}

//------------------------------------------------------------------------------
bool UITreeViewItem::isExpanded() const
{
	return m_expander->isChecked();
}

//------------------------------------------------------------------------------
UITreeViewItem* UITreeViewItem::addTextItem(const String& text)
{
	auto textBlock = newObject<UITextBlock>();
	textBlock->setText(text);
	return addItem(textBlock);
}

//------------------------------------------------------------------------------
UITreeViewItem* UITreeViewItem::addItem(UIElement* item)
{
	if (LN_CHECK_ARG(item != nullptr)) return nullptr;
	auto treeItem = newObject<UITreeViewItem>();
	treeItem->setHeader(item);
	addChild(treeItem);
	return treeItem;
}

//------------------------------------------------------------------------------
Size UITreeViewItem::measureOverride(const Size& constraint)
{
	if (getItems()->isEmpty())
	{
		m_expander->setVisibility(UIVisibility::Hidden);
	}
	else
	{
		m_expander->setVisibility(UIVisibility::Visible);
	}

	//// TODO: type
	//auto* parent = dynamic_cast<UITreeViewItem*>(getLogicalParent());
	//if (parent != nullptr)
	//{
	//	writeCoreFlag(detail::UICoreFlags_LayoutVisible, parent->isExpanded());
	//	printf("%d\n", parent->isExpanded());
	//}

	//Size desiredSize(0, 0);	// TODO: Branch の余白は後で考える http://doc.qt.io/qt-4.8/stylesheet-examples.html#customizing-qtreeview

	Size desiredSize = UIHeaderedItemsControl::measureOverride(constraint);

	// ベースの領域に対して、ExpanderButton の領域を追加する
	m_expander->measureLayout(constraint);
	Size expanderSize = m_expander->getDesiredSize();
	desiredSize.width += expanderSize.width;
	return desiredSize;

	//// ヘッダの領域を計測する
	//m_header->measureLayout(constraint);
	//Size headerSize = m_header->getDesiredSize();

	//// 子アイテムの領域を計測する
	//UILayoutPanel* itemsPanel = getLayoutPanel();
	//itemsPanel->measureLayout(constraint);
	//Size panelSize = itemsPanel->getDesiredSize();

	//// 下方向に結合する
	//desiredSize.height += headerSize.height;
	//desiredSize.height += panelSize.height;
	//desiredSize.width = expanderSize.width + std::max(headerSize.width, panelSize.width);

	//Size thisSize = ln::detail::LayoutHelper::measureElement(this, constraint);

	//return Size::max(desiredSize, thisSize);
}

//------------------------------------------------------------------------------
Size UITreeViewItem::arrangeOverride(const Size& finalSize)
{
	auto& padding = getPadding();

	// Expander
	Size expanderSize = m_expander->getDesiredSize();

	// Header
	Rect headerRect(expanderSize.width, 0, finalSize.width - expanderSize.width, 0);
	UIElement* header = getHeader();
	if (header != nullptr)
	{
		Size headerSize = header->getDesiredSize();
		headerRect.height = headerSize.height;
		//headerRect = headerRect.makeDeflate(getPadding());	// padding は Header へのみ影響させる
		//header->arrangeLayout(headerRect);
		Rect arrangeRect(headerRect.x + padding.left, headerRect.y + padding.top, headerRect.getSize());
		header->arrangeLayout(arrangeRect);
		//headerRect = Rect(headerRect.x + padding.left, headerRect.y + padding.top, headerRect.getSize());
		//header->arrangeLayout(headerRect);

		headerRect.height += padding.getHeight();
	}

	Thickness renderFrame(expanderSize.width, 0, 0, finalSize.height - headerRect.height);
	//renderFrame.right -= padding.getWidth() - renderFrame.left;
	//renderFrame.bottom -= padding.getHeight();
	setRenderFrameThickness(renderFrame);

	// Items
	Rect itemsRect(headerRect.x, headerRect.height, headerRect.width, finalSize.height - headerRect.height);
	getLayoutPanel()->arrangeLayout(itemsRect);




	// Expander
	Rect expanderRect(0, 0, expanderSize);
	expanderRect.y = (headerRect.height - expanderSize.height) / 2;
	m_expander->arrangeLayout(expanderRect);


	//UIHeaderedItemsControl::arrangeOverride(finalSize);

	//Size expanderSize = m_expander->getDesiredSize();

	//// Expander
	//m_expander->arrangeLayout(Rect(0, 0, expanderSize));

	////// Header
	////Size headerSize = m_header->getDesiredSize();
	////Rect headerRect(expanderSize.width, 0, finalSize.width - expanderSize.width, std::max(expanderSize.height, headerSize.height));
	////m_header->arrangeLayout(headerRect);
	////
	////// Items
	////Rect itemsRect(expanderSize.width, headerRect.height, finalSize.width - expanderSize.width, finalSize.height - headerRect.height);
	////getLayoutPanel()->arrangeLayout(itemsRect);

	return finalSize;
}

void UITreeViewItem::expander_OnChecked(UIEventArgs* e)
{
	getLayoutPanel()->setVisibility(UIVisibility::Visible);
}

void UITreeViewItem::expander_OnUnchecked(UIEventArgs* e)
{
	getLayoutPanel()->setVisibility(UIVisibility::Collapsed);
}

//==============================================================================
// UITreeView
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITreeView, UIControl)

//------------------------------------------------------------------------------
Ref<UITreeView> UITreeView::create()
{
	auto ptr = Ref<UITreeView>::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UITreeView::UITreeView()
{
}

//------------------------------------------------------------------------------
UITreeView::~UITreeView()
{
}

//------------------------------------------------------------------------------
void UITreeView::initialize()
{
	UIControl::initialize();
	setHContentAlignment(HAlignment::Stretch);

	auto panel = newObject<UIStackPanel>();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	setLayoutPanel(panel);
	goToVisualState(UIVisualStates::NormalState);
}

//------------------------------------------------------------------------------
UITreeViewItem* UITreeView::addTextItem(const String& text)
{
	auto textBlock = newObject<UITextBlock>();
	textBlock->setText(text);
	return addItem(textBlock);
}

//------------------------------------------------------------------------------
UITreeViewItem* UITreeView::addItem(UIElement* item)
{
	if (LN_CHECK_ARG(item != nullptr)) return nullptr;
	auto treeItem = newObject<UITreeViewItem>();
	treeItem->setHeader(item);
	addChild(treeItem);
	return treeItem;
}

LN_NAMESPACE_END
