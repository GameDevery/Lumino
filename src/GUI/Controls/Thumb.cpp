
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Thumb.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// DragEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Lumino::GUI::DragEventArgs, RoutedEventArgs);

//=============================================================================
// Thumb
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Thumb, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Thumb);

// Register property
LN_PROPERTY_IMPLEMENT(Thumb, bool, IsDraggingProperty, "IsDragging", m_isDragging, false, NULL);

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragStartedEvent, "DragStarted", DragStarted);
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragDeltaEvent, "DragDelta", DragDelta);
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragCompletedEvent, "DragCompleted", DragCompleted);
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragCanceledEvent, "DragCanceled", DragCanceled);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbPtr Thumb::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::Thumb(GUIManager* manager)
	: Control(manager)
	, m_isDragging(false)
{
	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(Thumb, MouseEventArgs, UIElement::MouseMoveEvent, Handler_MouseMove);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Thumb, MouseEventArgs, UIElement::MouseDownEvent, Handler_MouseDown);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Thumb, MouseEventArgs, UIElement::MouseUpEvent, Handler_MouseUp);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::~Thumb()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::CancelDrag()
{
	if (m_isDragging)
	{
		m_isDragging = false;
		ReleaseMouseCapture();

		// �h���b�O�L�����Z���C�x���g
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			m_lastScreenPosition.X,
			m_lastScreenPosition.Y));
		OnDragDelta(args);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::Handler_MouseMove(MouseEventArgs* e)
{
	if (m_isDragging)
	{
		// �h���b�O���C�x���g
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			e->X - m_lastScreenPosition.X,
			e->Y - m_lastScreenPosition.Y));
		OnDragDelta(args);

		e->Handled = true;
		return;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::Handler_MouseDown(MouseEventArgs* e)
{
	if (!m_isDragging)
	{
		m_lastScreenPosition.Set(e->X, e->Y);
		m_isDragging = true;
		CaptureMouse();

		// �h���b�O�J�n�C�x���g
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			e->X - m_lastScreenPosition.X,
			e->Y - m_lastScreenPosition.Y));
		OnDragStarted(args);

		e->Handled = true;
		return;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::Handler_MouseUp(MouseEventArgs* e)
{
	if (m_isDragging)
	{
		m_isDragging = false;
		ReleaseMouseCapture();

		// �h���b�O�I���C�x���g
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			e->X - m_lastScreenPosition.X,
			e->Y - m_lastScreenPosition.Y));
		OnDragCompleted(args);

		e->Handled = true;
		return;
	}
}

//=============================================================================
// ThumbChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ThumbChrome, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(ThumbChrome);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChromePtr ThumbChrome::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChrome::ThumbChrome(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChrome::~ThumbChrome()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ThumbChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_bgFrameBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ThumbChromeBackgroundFrameBrush")));
	m_bgInnerBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ThumbChromeBackgroundInnerBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ThumbChrome::OnRender(Graphics::Painter* painter)
{
	RectF rect = m_finalLocalRect;

	// �g
	painter->SetBrush(m_bgFrameBrush);
	painter->DrawFrameRectangle(rect, 16);

	// ����
	rect.Inflate(-16, -16);
	painter->SetBrush(m_bgInnerBrush);
	painter->DrawRectangle(rect);
}


} // namespace GUI
} // namespace Lumino

