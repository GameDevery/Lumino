
#include "../Internal.h"
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// InputDriver
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputDriver::InputDriver()
	: m_keyStatus{}
	, m_mouseStatus{}
	, m_mousePoint()
	, m_mouseWheel(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputDriver::~InputDriver()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputDriver::UpdateFrame()
{
	m_keyStatus.fill(false);
	m_mouseStatus.fill(false);
	m_mousePoint.X = 0;
	m_mousePoint.Y = 0;
	m_mouseWheel = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputDriver::OnEvent(const Platform::EventArgs& e)
{
	switch (e.Type)
	{
	case Platform::EventType_MouseDown:
		m_mouseStatus[e.Mouse.Button] = true;
		break;
	case Platform::EventType_MouseUp:
		m_mouseStatus[e.Mouse.Button] = false;
		break;
	case Platform::EventType_MouseMove:
		m_mousePoint.X = e.Mouse.X;
		m_mousePoint.Y = e.Mouse.Y;
		break;
	case Platform::EventType_MouseWheel:
		m_mouseWheel = e.Mouse.WheelDelta;
		break;
	case Platform::EventType_KeyDown:
		m_keyStatus[(int)e.Key.KeyCode] = true;
		break;
	case Platform::EventType_KeyUp:
		m_keyStatus[(int)e.Key.KeyCode] = false;
		break;
	default:
		break;
	}
}

LN_NAMESPACE_END