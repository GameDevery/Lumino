﻿
#include "Internal.hpp"
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs()
    : m_button(MouseButtons::None)
    , m_position(0, 0)
    , m_clickCount(0)
{
}

//------------------------------------------------------------------------------
UIMouseEventArgs::~UIMouseEventArgs()
{
}

//------------------------------------------------------------------------------
void UIMouseEventArgs::init(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount)
{
    UIEventArgs::init(sender, type);
    m_button = button;
    m_position.x = x;
    m_position.y = y;
    m_clickCount = clickCount;
}

//------------------------------------------------------------------------------
Point UIMouseEventArgs::getPosition(UIElement* relativeTo) const
{
    const Rect& rc = relativeTo->finalGlobalRect();
    return Point(m_position.x - rc.x, m_position.y - rc.y);
}

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

} // namespace ln
