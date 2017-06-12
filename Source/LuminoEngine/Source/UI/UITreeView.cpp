
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UITreeView.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// UITreeViewItem
//==============================================================================
	LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITreeViewItem, UIControl)

//------------------------------------------------------------------------------
UITreeViewItem::UITreeViewItem()
	: m_header(nullptr)
{
}

//------------------------------------------------------------------------------
UITreeViewItem::~UITreeViewItem()
{
}

//------------------------------------------------------------------------------
void UITreeViewItem::initialize()
{
	UIControl::initialize();
	SetHContentAlignment(HAlignment::Left);
	SetHAlignment(HAlignment::Stretch);
	GoToVisualState(UIVisualStates::NormalState);
}

//------------------------------------------------------------------------------
void UITreeViewItem::SetHeader(UIElement* header)
{
	RemoveVisualChild(m_header);

	m_header = header;
	m_header->SetBackground(Brush::Black);
	m_header->SetHeight(16);	// TODO:

	if (m_header != nullptr)
		AddVisualChild(m_header);
}

//------------------------------------------------------------------------------
Size UITreeViewItem::MeasureOverride(const Size& constraint)
{
	Size desiredSize(16, 0);	// TODO: Branch �̗]���͌�ōl���� http://doc.qt.io/qt-4.8/stylesheet-examples.html#customizing-qtreeview

	// Measure Header
	m_header->MeasureLayout(constraint);
	Size headerSize = m_header->GetDesiredSize();

	// Measure Items
	UILayoutPanel* itemsPanel = GetLayoutPanel();
	itemsPanel->MeasureLayout(constraint);
	Size panelSize = itemsPanel->GetDesiredSize();

	// �������Ɍ�������
	desiredSize.height += headerSize.height;
	desiredSize.height += panelSize.height;
	desiredSize.width = std::max(headerSize.width, panelSize.width);

	Size thisSize = ln::detail::LayoutHelper::MeasureElement(this, constraint);

	return Size::Max(desiredSize, thisSize);

	// ��GetLayoutPanel() �œ����� UILayoutPanel �� Measure �������ōs���̂� 
}

//------------------------------------------------------------------------------
Size UITreeViewItem::ArrangeOverride(const Size& finalSize)
{
	Size expanderSize(16, 16);	// TODO: Branch �̗]���͌�ōl����

	// Header
	Size headerSize = m_header->GetDesiredSize();
	Rect headerRect(expanderSize.width, 0, finalSize.width - expanderSize.width, std::max(expanderSize.height, headerSize.height));
	m_header->ArrangeLayout(headerRect);
	
	// Items
	Rect itemsRect(expanderSize.width, headerRect.height, finalSize.width - expanderSize.width, finalSize.height - headerRect.height);
	GetLayoutPanel()->ArrangeLayout(itemsRect);

	return finalSize;
}


//==============================================================================
// UITreeView
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITreeView, UIControl)

//------------------------------------------------------------------------------
UITreeViewPtr UITreeView::create()
{
	auto ptr = UITreeViewPtr::makeRef();
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
	SetHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::makeRef();
	panel->initialize();
	panel->SetHAlignment(HAlignment::Stretch);
	panel->SetVAlignment(VAlignment::Stretch);
	SetLayoutPanel(panel);
	GoToVisualState(UIVisualStates::NormalState);
}

////------------------------------------------------------------------------------
//UITreeViewItemPtr UITreeView::AddTextItem(const String& text)
//{
//	auto textBlock = RefPtr<UITextBlock>::MakeRef();
//	textBlock->initialize(GetManager());
//	textBlock->SetText(text);
//	return AddItem(textBlock);
//}
//
////------------------------------------------------------------------------------
//UITreeViewItemPtr UITreeView::AddItem(UIElement* item)
//{
//
//	// �󂯎���� item �� UITreeViewItem �Ń��b�v���āAUITreeViewItem �����X�g�ɓ����
//	auto listItem = RefPtr<UITreeViewItem>::MakeRef();
//	listItem->initialize(GetManager());
//	listItem->SetContent(item);
//	GetItems()->Add(listItem);
//	return listItem;
//}

} // namespace tr
LN_NAMESPACE_END
