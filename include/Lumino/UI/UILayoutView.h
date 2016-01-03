
#pragma once
#include "UIInjectedInputReceiver.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	UILayoutView �̓l�C�e�B�u�E�B���h�E�� UI �V�X�e���̐ڍ��_�ƂȂ�A
				UI ���C�A�E�g�̃��[�g�v�f��ێ����܂��B
*/
class UILayoutView
	: public RefObject
	, public IUIInjectedInputReceiver
{
public:

	/** ���̗v�f���֘A�t�����Ă��� UIContext ���擾���܂��B*/
	UIContext* GetOwnerContext() { return m_ownerContext; }

LN_INTERNAL_ACCESS:
	void UpdateLayout(const SizeF& viewSize);
	//void Render();
	const SizeF& GetViewPixelSize() const { return m_viewPixelSize; }
	bool UpdateMouseHover(const PointF& mousePos);
	void CaptureMouse(UIElement* element);
	void ReleaseMouseCapture(UIElement* element);


	// Implements IUIInjectedInputReceiver
	//virtual bool InjectViewportSizeChanged(int width, int height) override;
	virtual bool InjectMouseMove(float clientX, float clientY) override;
	virtual bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY) override;
	virtual bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY) override;
	virtual bool InjectMouseWheel(int delta, float clientX, float clientY) override;
	virtual bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl) override;
	virtual bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl) override;
	virtual bool InjectTextInput(TCHAR ch) override;

private:
	friend class UIContext;

	struct MouseClickTracker
	{
		double		LastTime = 0.0;
		int			ClickCount = 0;
		UIElement*	HoverElement = nullptr;
	};

	UILayoutView();
	virtual ~UILayoutView();
	void Initialize(UIContext* ownerContext, Platform::Window* ownerNativeWindow);

	Platform::Window*	m_ownerNativeWindow;

	UIContext*			m_ownerContext;
	UIElement*			m_rootElement;
	UIElement*			m_mouseHoverElement;
	UIElement*			m_capturedElement;

	PointF				m_mousePosition;
	MouseClickTracker	m_mouseClickTrackers[MouseButton::TERMINATOR];

	SizeF				m_viewPixelSize;
};

LN_NAMESPACE_END
