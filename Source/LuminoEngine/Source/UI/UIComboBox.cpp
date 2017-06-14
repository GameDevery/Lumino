
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIScrollViewer.h>
#include <Lumino/UI/UIComboBox.h>
#include <Lumino/UI/UILayoutPanel.h>	// TODO: stackpanel �ɂ��邩��
#include <Lumino/UI/UILayoutView.h>
#include "UIManager.h"
#include "UIHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIPopup
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIPopup, UIContentControl)

//------------------------------------------------------------------------------
UIPopup::UIPopup()
{
}

//------------------------------------------------------------------------------
UIPopup::~UIPopup()
{
}

//------------------------------------------------------------------------------
void UIPopup::initialize()
{
	UIElement::initialize();
}

//------------------------------------------------------------------------------
void UIPopup::setContent(UIElement* element)
{
	if (m_content == element) return;

	m_content = element;

	// ���Ɏ����Ă���Ύ�菜���Ă���
	if (m_content != nullptr)
	{
		removeVisualChild(m_content);
		m_content = nullptr;
	}

	// �V�����ێ�����
	if (element != nullptr)
	{
		addVisualChild(element);
		m_content = element;
	}
}

//------------------------------------------------------------------------------
void UIPopup::open()
{
	UIElement* root = UIHelper::getLayoutRoot(this);
	if (root != nullptr)
	{
		UILayoutView* rootView = static_cast<UILayoutView*>(root);
		rootView->openPopup(this);
	}
}

//------------------------------------------------------------------------------
Size UIPopup::measureOverride(const Size& constraint)
{
	// Popup �͏�ɃT�C�Y 0 �ƂȂ�B
	// �܂��A�q�v�f�̃��C�A�E�g�͍s��Ȃ��B
	// �q�v�f�̃��C�A�E�g���s���͕̂ʓr�APopup ��p�̃��C�A�E�g�t�F�[�Y�B
	return Size();
}

//------------------------------------------------------------------------------
Size UIPopup::arrangeOverride(const Size& finalSize)
{
	return UIElement::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UIPopup::updateLayoutForInPlacePopup(const Size& viewSize)
{
	m_content->updateLayout(viewSize);
	//m_content->measureLayout(viewSize);

	////TODO: ���̂ւ��child�̈ʒu�����߂�

	//m_content->arrangeLayout(RectF(0, 0, viewSize));
	//m_content->updateTransformHierarchy(RectF(0, 0, viewSize));
}


//==============================================================================
// UIComboBoxItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIComboBoxItem, UIContentControl)

//------------------------------------------------------------------------------
UIComboBoxItem::UIComboBoxItem()
{
}

//------------------------------------------------------------------------------
UIComboBoxItem::~UIComboBoxItem()
{
}

//------------------------------------------------------------------------------
void UIComboBoxItem::initialize()
{
	UIContentControl::initialize();
	setHContentAlignment(HAlignment::Left);
	setHAlignment(HAlignment::Stretch);
	goToVisualState(UIVisualStates::NormalState);
}

//==============================================================================
// UIComboBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIComboBox, UIControl)


//------------------------------------------------------------------------------
UIComboBoxPtr UIComboBox::create()
{
	auto ptr = UIComboBoxPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIComboBox::UIComboBox()
{
}

//------------------------------------------------------------------------------
UIComboBox::~UIComboBox()
{
}

//------------------------------------------------------------------------------
void UIComboBox::initialize()
{
	UIControl::initialize();

	m_popup = newObject<UIPopup>();
	addVisualChild(m_popup);

	m_scrollViewer = newObject<UIScrollViewer>();
	m_scrollViewer->setWidth(100);	// TODO:
	m_scrollViewer->setHeight(30);
	m_scrollViewer->setBackground(Brush::Blue);
	m_popup->setContent(m_scrollViewer);

	setHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::makeRef();
	panel->initialize();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	setLayoutPanel(panel);
	goToVisualState(UIVisualStates::NormalState);
}

//------------------------------------------------------------------------------
//UIComboBoxItemPtr UIComboBox::addTextItem(const String& text)
//{
//	auto textBlock = RefPtr<UITextBlock>::MakeRef();
//	textBlock->initialize(getManager());
//	textBlock->SetText(text);
//	return AddItem(textBlock);
//}

//------------------------------------------------------------------------------
//UIComboBoxItemPtr UIComboBox::AddItem(UIElement* item)
//{
//	if (LN_CHECK_ARG(item != nullptr)) return nullptr;
//
//	// �󂯎���� item �� UIComboBoxItem �Ń��b�v���āAUIComboBoxItem �����X�g�ɓ����
//	auto listItem = RefPtr<UIComboBoxItem>::MakeRef();
//	listItem->initialize(getManager());
//	listItem->setContent(item);
//	getItems()->Add(listItem);
//	return listItem;
//}

//------------------------------------------------------------------------------
void UIComboBox::onMouseDown(UIMouseEventArgs* e)
{
	m_popup->open();
	UIControl::onMouseDown(e);
}

LN_NAMESPACE_END
