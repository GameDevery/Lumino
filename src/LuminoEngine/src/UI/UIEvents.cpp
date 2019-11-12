﻿
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include "UIManager.hpp"
#include "UIEventArgsPool.hpp"

namespace ln {

//==============================================================================
// UIEvents

const UIEventType	UIEvents::GotFocusEvent = 1;
const UIEventType	UIEvents::LostFocusEvent = 2;
const UIEventType	UIEvents::MouseMoveEvent = 3;
const UIEventType	UIEvents::MouseDownEvent = 4;
const UIEventType	UIEvents::MouseUpEvent = 5;
const UIEventType	UIEvents::MouseWheelEvent = 6;
const UIEventType	UIEvents::MouseEnterEvent = 7;
const UIEventType	UIEvents::MouseLeaveEvent = 8;
const UIEventType	UIEvents::KeyDownEvent = 9;
const UIEventType	UIEvents::KeyUpEvent = 10;
const UIEventType	UIEvents::TextInputEvent = 11;
const UIEventType	UIEvents::CheckedEvent = 12;
const UIEventType	UIEvents::UncheckedEvent = 13;
const UIEventType	UIEvents::ScrollDragStartedEvent = 14;
const UIEventType	UIEvents::ScrollDragDeltaEvent = 15;
const UIEventType	UIEvents::ScrollDragCompletedEvent = 16;
const UIEventType	UIEvents::ScrollDragCanceledEvent = 17;
const UIEventType	UIEvents::ScrollEvent = 18;
const UIEventType	UIEvents::DragEnterEvent = 19;
const UIEventType	UIEvents::DragDropEvent = 20;

const UIEventType	UIEvents::Selected = 21;
const UIEventType	UIEvents::Unselected = 22;

const UIEventType	UIEvents::Click = 23;

const UIEventType   UIEvents::SelectionChanged = 24;

const UIEventType   UIEvents::Timer = 25;

const UIEventType	UIEvents::ExecuteCommandEvent = 201;
const UIEventType	UIEvents::CanExecuteCommandEvent = 202;
const UIEventType	UIEvents::CanExecuteChangedEvent = 203;

const UIEventType   UIEvents::NotifyPropertyChanged = 301;

const UIEventType	UIEvents::RequestVisualUpdateEvent = 101;
const UIEventType	UIEvents::RequestVisualRedrawEvent = 102;

//==============================================================================
// UIEventArgs

LN_OBJECT_IMPLEMENT(UIEventArgs, Object) {}

Ref<UIEventArgs> UIEventArgs::create(UIElement* sender, UIEventType type, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        return Ref<UIEventArgs>(pool->create<UIEventArgs>(sender, type), false);
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIEventArgs::UIEventArgs()
    : m_sender(nullptr)
    , m_type(0)
{
}

UIEventArgs::~UIEventArgs()
{
}

void UIEventArgs::init(UIElement* sender, UIEventType type)
{
    Object::init();
    m_sender = sender;
    m_type = type;
}

//==============================================================================
// UIMouseEventArgs

LN_OBJECT_IMPLEMENT(UIMouseEventArgs, UIEventArgs) {}

Ref<UIMouseEventArgs> UIMouseEventArgs::create(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIMouseEventArgs> ptr(pool->create<UIMouseEventArgs>(sender, type, button, x, y, clickCount), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIMouseEventArgs::UIMouseEventArgs()
    : m_button(MouseButtons::None)
    , m_position(0, 0)
    , m_clickCount(0)
{
}

UIMouseEventArgs::~UIMouseEventArgs()
{
}

void UIMouseEventArgs::init(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount)
{
    UIEventArgs::init(sender, type);
    m_button = button;
    m_position.x = x;
    m_position.y = y;
    m_clickCount = clickCount;
}

Point UIMouseEventArgs::getPosition(UIElement* relativeTo) const
{
    auto inv = Matrix::makeInverse(relativeTo->m_combinedFinalRenderTransform);
    auto pos = Vector3::transformCoord(Vector3(m_position.x, m_position.y, .0f), inv);
    return Point(pos.x, pos.y);
    //const Rect& rc = relativeTo->finalGlobalRect();
    //return Point(m_position.x - rc.x, m_position.y - rc.y);
}

//Point UIMouseEventArgs::getPosition(RenderView* relativeTo) const
//{
//	auto pt = relativeTo->actualScreenOffset();
//	return Point(m_position.x - pt.x, m_position.y - pt.y);
//}

//==============================================================================
// UIKeyEventArgs

LN_OBJECT_IMPLEMENT(UIKeyEventArgs, UIEventArgs) {}

//------------------------------------------------------------------------------
Ref<UIKeyEventArgs> UIKeyEventArgs::create(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIKeyEventArgs> ptr(pool->create<UIKeyEventArgs>(sender, type, keyCode, modifierKeys, charCode), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

//------------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs()
    : m_keyCode(Keys::Unknown)
    , m_modifierKeys(ModifierKeys::None)
    , m_charCode(0)
{
}

//------------------------------------------------------------------------------
UIKeyEventArgs::~UIKeyEventArgs()
{
}

//------------------------------------------------------------------------------
void UIKeyEventArgs::init(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode)
{
    UIEventArgs::init(sender, type);
    m_keyCode = keyCode;
    m_modifierKeys = modifierKeys;
    m_charCode = charCode;
}

//==============================================================================
// UIMouseWheelEventArgs

LN_OBJECT_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs) {}

//------------------------------------------------------------------------------
Ref<UIMouseWheelEventArgs> UIMouseWheelEventArgs::create(UIElement* sender, UIEventType type, int delta, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIMouseWheelEventArgs> ptr(pool->create<UIMouseWheelEventArgs>(sender, type, delta), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::UIMouseWheelEventArgs()
    : m_delta(0)
{
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::~UIMouseWheelEventArgs()
{
}

//------------------------------------------------------------------------------
void UIMouseWheelEventArgs::init(UIElement* sender, UIEventType type, int delta)
{
    UIEventArgs::init(sender, type);
    m_delta = delta;
}

//==============================================================================
// UIDragDeltaEventArgs

LN_OBJECT_IMPLEMENT(UIDragDeltaEventArgs, UIEventArgs) {}

Ref<UIDragDeltaEventArgs> UIDragDeltaEventArgs::create(UIElement* sender, UIEventType type, float offsetX, float offsetY, bool caching)
{
	if (caching)
	{
		auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
		Ref<UIDragDeltaEventArgs> ptr(pool->create<UIDragDeltaEventArgs>(sender, type, offsetX, offsetY), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

UIDragDeltaEventArgs::UIDragDeltaEventArgs()
{
}

UIDragDeltaEventArgs::~UIDragDeltaEventArgs()
{
}

void UIDragDeltaEventArgs::init(UIElement* sender, UIEventType type, float offsetX, float offsetY)
{
	UIEventArgs::init(sender, type);
	m_offsetX = offsetX;
	m_offsetY = offsetY;
}


//==============================================================================
// UIScrollEventArgs

LN_OBJECT_IMPLEMENT(UIScrollEventArgs, UIEventArgs) {}

Ref<UIScrollEventArgs> UIScrollEventArgs::create(UIElement* sender, UIEventType type, float newValue, ScrollEventType scrollType, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIScrollEventArgs> ptr(pool->create<UIScrollEventArgs>(sender, type, newValue, scrollType), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIScrollEventArgs::UIScrollEventArgs()
{
}

UIScrollEventArgs::~UIScrollEventArgs()
{
}

void UIScrollEventArgs::init(UIElement* sender, UIEventType type, float newValue, ScrollEventType scrollType)
{
    UIEventArgs::init(sender, type);
    m_newValue = newValue;
    m_scrollType = scrollType;
}

//==============================================================================
// UIDragDropEventArgs

LN_OBJECT_IMPLEMENT(UIDragDropEventArgs, UIEventArgs) {}

Ref<UIDragDropEventArgs> UIDragDropEventArgs::create(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIDragDropEventArgs> ptr(pool->create<UIDragDropEventArgs>(sender, type, data, effect), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIDragDropEventArgs::UIDragDropEventArgs()
    : m_data(nullptr)
    , m_effect(DragDropEffects::None)
{
}

UIDragDropEventArgs::~UIDragDropEventArgs()
{
}

void UIDragDropEventArgs::init(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect)
{
    UIEventArgs::init(sender, type);
    if (LN_REQUIRE(data)) return;
    m_data = data;
    m_effect = effect;
}

//==============================================================================
// UIClickEventArgs

LN_OBJECT_IMPLEMENT(UIClickEventArgs, UIEventArgs) {}

Ref<UIClickEventArgs> UIClickEventArgs::create(UIElement* sender, UIEventType type, int clickCount, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIClickEventArgs> ptr(pool->create<UIClickEventArgs>(sender, type, clickCount), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIClickEventArgs::UIClickEventArgs()
    : m_clickCount(0)
{
}

UIClickEventArgs::~UIClickEventArgs()
{
}

void UIClickEventArgs::init(UIElement* sender, UIEventType type, int clickCount)
{
    UIEventArgs::init(sender, type);
    m_clickCount = clickCount;
}


//==============================================================================
// UISelectionChangedEventArgs

LN_OBJECT_IMPLEMENT(UISelectionChangedEventArgs, UIEventArgs) {}

Ref<UISelectionChangedEventArgs> UISelectionChangedEventArgs::create(UIElement* sender, UIEventType type, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UISelectionChangedEventArgs> ptr(pool->create<UISelectionChangedEventArgs>(sender, type), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UISelectionChangedEventArgs::UISelectionChangedEventArgs()
{
}

void UISelectionChangedEventArgs::init(UIElement* sender, UIEventType type)
{
    UIEventArgs::init(sender, type);
}

//==============================================================================
// UICommandEventArgs

LN_OBJECT_IMPLEMENT(UICommandEventArgs, UIEventArgs) {}

Ref<UICommandEventArgs> UICommandEventArgs::create(UIElement* sender, UIEventType type, UICommand* command, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UICommandEventArgs> ptr(pool->create<UICommandEventArgs>(sender, type, command), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UICommandEventArgs::UICommandEventArgs()
    : m_command(nullptr)
    , m_canExecute(true)
{
}

void UICommandEventArgs::init(UIElement* sender, UIEventType type, UICommand* command)
{
    UIEventArgs::init(sender, type);
    m_command = command;
    m_canExecute = true;
}

//==============================================================================
// UINotifyPropertyChangedEventArgs

LN_OBJECT_IMPLEMENT(UINotifyPropertyChangedEventArgs, UIEventArgs) {}

Ref<UINotifyPropertyChangedEventArgs> UINotifyPropertyChangedEventArgs::create(UIElement* sender, UIEventType type, const StringRef& propertyName, bool caching)
{
    if (caching) {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UINotifyPropertyChangedEventArgs> ptr(pool->create<UINotifyPropertyChangedEventArgs>(sender, type, propertyName), false);
        return ptr;
    }
    else {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

Ref<UINotifyPropertyChangedEventArgs> UINotifyPropertyChangedEventArgs::create(UIElement* sender, UIEventType type, UICollectionChangedAction action, int startIndex, int count, bool caching)
{
    if (caching) {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UINotifyPropertyChangedEventArgs> ptr(pool->create<UINotifyPropertyChangedEventArgs>(sender, type, action, startIndex, count), false);
        return ptr;
    }
    else {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UINotifyPropertyChangedEventArgs::UINotifyPropertyChangedEventArgs()
    : m_action(UICollectionChangedAction::Reset)
{
}

void UINotifyPropertyChangedEventArgs::init(UIElement* sender, UIEventType type, const StringRef& propertyName)
{
    UIEventArgs::init(sender, type);
    m_action = UICollectionChangedAction::Reset;
    m_name = propertyName;
    m_startIndex = 0;
    m_count = 0;
}

void UINotifyPropertyChangedEventArgs::init(UIElement* sender, UIEventType type, UICollectionChangedAction action, int startIndex, int count)
{
    UIEventArgs::init(sender, type);
    m_action = action;
    m_name = String::Empty;
    m_startIndex = startIndex;
    m_count = count;
}

//==============================================================================
// UITimerEventArgs

LN_OBJECT_IMPLEMENT(UITimerEventArgs, UIEventArgs) {}

Ref<UITimerEventArgs> UITimerEventArgs::create(UIElement* sender, UIEventType type, float elapsedSeconds, bool caching)
{
	if (caching)
	{
		auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
		Ref<UITimerEventArgs> ptr(pool->create<UITimerEventArgs>(sender, type, elapsedSeconds), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

UITimerEventArgs::UITimerEventArgs()
	: m_elapsedSeconds(0.0f)
{
}

void UITimerEventArgs::init(UIElement* sender, UIEventType type, float elapsedSeconds)
{
	UIEventArgs::init(sender, type);
	m_elapsedSeconds = elapsedSeconds;
}

} // namespace ln

