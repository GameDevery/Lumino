
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIElement.h>
#include "UIManager.h"
#include "EventArgsPool.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// UILayoutView
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UILayoutView::UILayoutView()
	: m_ownerNativeWindow(nullptr)
	, m_ownerContext(nullptr)
	, m_rootElement(nullptr)
	, m_mouseHoverElement(nullptr)
	, m_capturedElement(nullptr)
	, m_mouseClickTrackers{}
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UILayoutView::~UILayoutView()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UILayoutView::Initialize(UIContext* ownerContext, PlatformWindow* ownerNativeWindow)
{
	m_ownerContext = ownerContext;
	m_ownerNativeWindow = ownerNativeWindow;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UILayoutView::UpdateLayout(const SizeF& viewSize)
{
	m_viewPixelSize = viewSize;

	if (m_rootElement != NULL)
	{
		m_rootElement->ApplyTemplate();
		m_rootElement->UpdateLayout();
		m_rootElement->UpdateTransformHierarchy();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::UpdateMouseHover(const PointF& mousePos)
{
	detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();
	UIElement* old = m_mouseHoverElement;

	// TODO:IME���̃C�x���g����������
	//if ( m_pIme != NULL )
	//{
	//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
	//	{
	//		goto EXIT;
	//	}
	//}

	// �ʏ�̃E�B���h�E�̃C�x���g����������
	if (m_rootElement != NULL)
	{
		m_mouseHoverElement = m_rootElement->CheckMouseHoverElement(mousePos);
		if (m_mouseHoverElement != nullptr) {
			goto EXIT;
		}
	}

	m_mouseHoverElement = nullptr;

EXIT:
	// �V�����ꂼ��� Element �� MouseLeave�AMouseEnter �C�x���g�𑗂�
	if (m_mouseHoverElement != old)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(MouseButton::None, 0, mousePos.X, mousePos.Y, 0), false);
		if (old != nullptr) {
			old->OnEvent(detail::UIInternalEventType::MouseLeave, args);
		}

		// ���� MouseLeave �C�x���g�ƂŎg���܂킵�Ă���̂Ń��Z�b�g��Y�ꂸ��
		args->handled = false;

		if (m_mouseHoverElement != nullptr) {
			return m_mouseHoverElement->OnEvent(detail::UIInternalEventType::MouseEnter, args);
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UILayoutView::CaptureMouse(UIElement* element)
{
	m_capturedElement = element;
	m_ownerNativeWindow->CaptureMouse();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UILayoutView::ReleaseMouseCapture(UIElement* element)
{
	if (m_capturedElement == element)
	{
		m_capturedElement = NULL;
		m_ownerNativeWindow->ReleaseMouseCapture();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectMouseMove(float clientX, float clientY)
{
	detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// �L���v�`�����̃R���g���[��������΂�����ɑ���
	if (m_capturedElement != nullptr)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(MouseButton::None, 0, clientX, clientY, 0), false);
		return m_capturedElement->OnEvent(detail::UIInternalEventType::MouseMove, args);
	}
	UpdateMouseHover(PointF(clientX, clientY));
	if (m_mouseHoverElement == NULL) { return false; }

	RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(MouseButton::None, 0, clientX, clientY, 0), false);
	return m_mouseHoverElement->OnEvent(detail::UIInternalEventType::MouseMove, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectMouseButtonDown(MouseButton button, float clientX, float clientY)
{
	detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// �}�E�X�N���b�N�񐔂̏���
	MouseClickTracker& tracker = m_mouseClickTrackers[button];
	tracker.ClickCount++;

	double curTime = 0.001 * Environment::GetTickCount();
	float elapsed = (float)(curTime - tracker.LastTime);
	if (elapsed > detail::UIManager::MouseButtonClickTimeout ||
		m_mouseHoverElement != tracker.HoverElement ||
		tracker.ClickCount > 3)
	{
		tracker.ClickCount = 1;
		tracker.HoverElement = m_mouseHoverElement;
	}
	tracker.LastTime = curTime;

	// �L���v�`�����̃R���g���[��������΂�����ɑ���
	if (m_capturedElement != NULL)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(button, 0, clientX, clientY, tracker.ClickCount), false);
		return m_capturedElement->OnEvent(detail::UIInternalEventType::MouseButtonDown, args);
	}
	if (m_mouseHoverElement == NULL) { return false; }

	RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(button, 0, clientX, clientY, tracker.ClickCount), false);
	return m_mouseHoverElement->OnEvent(detail::UIInternalEventType::MouseButtonDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectMouseButtonUp(MouseButton button, float clientX, float clientY)
{
	detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// �L���v�`������UI�v�f������΂�����ɑ���
	if (m_capturedElement != nullptr)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(button, 0, clientX, clientY, 0), false);
		return m_capturedElement->OnEvent(detail::UIInternalEventType::MouseButtonUp, args);
	}
	// �}�E�X�ʒu��UI�v�f������΂�����ɑ���
	if (m_mouseHoverElement != nullptr)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(button, 0, clientX, clientY, 0), false);
		return m_mouseHoverElement->OnEvent(detail::UIInternalEventType::MouseButtonUp, args);
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectMouseWheel(int delta, float clientX, float clientY)
{
	detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// �L���v�`������UI�v�f������΂�����ɑ���
	if (m_capturedElement != NULL)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(MouseButton::None, delta, clientX, clientY, 0), false);
		return m_capturedElement->OnEvent(detail::UIInternalEventType::MouseWheel, args);
	}
	// �}�E�X�ʒu��UI�v�f������΂�����ɑ���
	if (m_mouseHoverElement != nullptr)
	{
		RefPtr<UIMouseEventArgs> args(pool->Create<UIMouseEventArgs>(MouseButton::None, delta, clientX, clientY, 0), false);
		return m_mouseHoverElement->OnEvent(detail::UIInternalEventType::MouseWheel, args);
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	// �t�H�[�J�X�������Ă���UI�v�f�ɑ���
	if (m_ownerContext->SetFocusElement() != nullptr)
	{
		detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();
		RefPtr<UIKeyEventArgs> args(pool->Create<UIKeyEventArgs>(keyCode, isAlt, isShift, isControl), false);
		return m_ownerContext->SetFocusElement()->OnEvent(detail::UIInternalEventType::KeyDown, args);
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	// �t�H�[�J�X�������Ă���UI�v�f�ɑ���
	if (m_ownerContext->SetFocusElement() != nullptr)
	{
		detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();
		RefPtr<UIKeyEventArgs> args(pool->Create<UIKeyEventArgs>(keyCode, isAlt, isShift, isControl), false);
		return m_ownerContext->SetFocusElement()->OnEvent(detail::UIInternalEventType::KeyUp, args);
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UILayoutView::InjectTextInput(TCHAR ch)
{
	// �t�H�[�J�X�������Ă���UI�v�f�ɑ���
	if (m_ownerContext->SetFocusElement() != nullptr)
	{
		detail::EventArgsPool* pool = m_ownerContext->GetManager()->GetEventArgsPool();
		RefPtr<UIKeyEventArgs> args(pool->Create<UIKeyEventArgs>(Key::Unknown, false, false, false), false);
		args->charCode = ch;
		return m_ownerContext->SetFocusElement()->OnEvent(detail::UIInternalEventType::TextInput, args);
	}
	return false;
}

LN_NAMESPACE_END
