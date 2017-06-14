
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIButtonBase
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIButtonBase, UIControl);

//------------------------------------------------------------------------------
UIButtonBase::UIButtonBase()
	: m_clickMode(ClickMode::release)
	, m_isPressed(false)
{
}

//------------------------------------------------------------------------------
UIButtonBase::~UIButtonBase()
{
}

//------------------------------------------------------------------------------
void UIButtonBase::initialize()
{
	UIControl::initialize();

	HContentAlignment = HAlignment::Center;
	VContentAlignment = VAlignment::Center;

	// TODO: UIControl::initialize() の中でも作ってるから、そっちが無駄になる。
	// UIControl では何も作らなくてもいいかも。null の場合、UILayoutPanel と同じレイアウトにするとか。
	setLayoutPanel(newObject<UIStackPanel>());
}

//------------------------------------------------------------------------------
void UIButtonBase::setText(const StringRef& text)
{
	auto textBlock = UITextBlockPtr::makeRef();
	textBlock->initialize();
	textBlock->setText(text);
	addChild(textBlock);
}

//------------------------------------------------------------------------------
EventConnection UIButtonBase::connectOnGotFocus(UIEventHandler handler)
{
	return m_onClick.connect(handler);
}

//------------------------------------------------------------------------------
void UIButtonBase::onClick(UIEventArgs* e)
{
	m_onClick.raise(e);
	//raiseEvent(ClickEvent, this, UIEventArgs::create(this));
}

//------------------------------------------------------------------------------
void UIButtonBase::onMouseDown(UIMouseEventArgs* e)
{
	if (m_clickMode == ClickMode::release)
	{
		m_isPressed = true;
		focus();
		captureMouse();
		goToVisualState(UIVisualStates::PressedState);
		e->handled = true;
	}
	else if (m_clickMode == ClickMode::Press)
	{
		onClick(e);
		e->handled = true;
	}

	UIControl::onMouseDown(e);
}

//------------------------------------------------------------------------------
void UIButtonBase::onMouseUp(UIMouseEventArgs* e)
{
	if (m_clickMode == ClickMode::release)
	{
		if (m_isPressed)
		{
			m_isPressed = false;
			releaseMouseCapture();
			goToVisualState(UIVisualStates::MouseOverState);
			onClick(e);
			e->handled = true;
		}
	}

	UIControl::onMouseUp(e);
}

//==============================================================================
// UIButton
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIButton, UIButtonBase);

//------------------------------------------------------------------------------
RefPtr<UIButton> UIButton::create()
{
	return newObject<UIButton>();
}

//------------------------------------------------------------------------------
RefPtr<UIButton> UIButton::create(const StringRef& text, float width, float height)
{
	return newObject<UIButton>(text, width, height);
}

//------------------------------------------------------------------------------
UIButton::UIButton()
{
}

//------------------------------------------------------------------------------
UIButton::~UIButton()
{
}

//------------------------------------------------------------------------------
void UIButton::initialize()
{
	UIButtonBase::initialize();
}

//------------------------------------------------------------------------------
void UIButton::initialize(const StringRef& text, float width, float height)
{
	UIButtonBase::initialize();
	setText(text);
	setWidth(width);
	setHeight(height);
}

//==============================================================================
// UIToggleButton
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIToggleButton, UIButtonBase);

const String UIToggleButton::CheckedState = _T("Checked");
const String UIToggleButton::UncheckedState = _T("Unchecked");

//------------------------------------------------------------------------------
RefPtr<UIToggleButton> UIToggleButton::create()
{
	return newObject<UIToggleButton>();
}

//------------------------------------------------------------------------------
UIToggleButton::UIToggleButton()
	: m_isChecked(false)
{
}

//------------------------------------------------------------------------------
UIToggleButton::~UIToggleButton()
{
}

//------------------------------------------------------------------------------
void UIToggleButton::initialize()
{
	UIButtonBase::initialize();

	auto* vsm = getVisualStateManager();
	vsm->registerVisualState(UIVisualStates::CommonGroup, CheckedState);
	vsm->registerVisualState(UIVisualStates::CommonGroup, UncheckedState);
}

//------------------------------------------------------------------------------
void UIToggleButton::onClick(UIEventArgs* e)
{
	m_isChecked = !m_isChecked;

	if (m_isChecked)
	{
		goToVisualState(CheckedState);
	}
	else
	{
		goToVisualState(UncheckedState);
	}

	UIButtonBase::onClick(e);
}


LN_NAMESPACE_END
