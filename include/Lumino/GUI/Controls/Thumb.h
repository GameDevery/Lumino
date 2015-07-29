
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		�}�E�X�h���b�O���֌W����C�x���g�����ł��B
*/
class DragEventArgs
	: public RoutedEventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	DragEventArgs(float xOffset, float yOffset) { XOffset = xOffset; YOffset = yOffset; }
	virtual ~DragEventArgs() {}

public:
	float XOffset;		///< �h���b�O�J�n�_����̐�������
	float YOffset;		///< �h���b�O�J�n�_����̐�������
};

/**
	@brief
	@note		Thumb �̓}�E�X�h���b�O�ɂ��ړ��ʂ�ʒm���邽�߂̃R���g���[���B
				Thumb ���̂̈ʒu���ړ�������̂ł͂Ȃ��_�ɒ��ӁB
				ScrollBar �͂������AGridSpritter �� Thumb �̃T�u�N���X�B
*/
class Thumb
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Thumb);
public:
	static const Property*		IsDraggingProperty;		///< �h���b�O���ł��邩�������l

	static const RoutedEvent*	DragStartedEvent;		///< �h���b�O�J�n�C�x���g
	static const RoutedEvent*	DragDeltaEvent;			///< �h���b�O���̈ړ��C�x���g
	static const RoutedEvent*	DragCompletedEvent;		///< �h���b�O�I���C�x���g
	static const RoutedEvent*	DragCanceledEvent;		///< �h���b�O�L�����Z���C�x���g

	// TODO:��
	Event01<DragEventArgs*>	DragStarted;
	Event01<DragEventArgs*>	DragDelta;
	Event01<DragEventArgs*>	DragCompleted;
	Event01<DragEventArgs*>	DragCanceled;

public:
	static Thumb* Create(GUIManager* manager);

	Thumb(GUIManager* manager);
	virtual ~Thumb();

	/// �h���b�O���ł��邩���m�F����
	bool IsDragging() const { return m_isDragging; }

	/// �h���b�O���L�����Z������
	void CancelDrag();

protected:
	virtual void OnDragStarted(DragEventArgs* e)	{ if (!e->Handled) { RaiseEvent(DragStartedEvent, this, e); } }
	virtual void OnDragDelta(DragEventArgs* e)		{ if (!e->Handled) { RaiseEvent(DragDeltaEvent, this, e); } }
	virtual void OnDragCompleted(DragEventArgs* e)	{ if (!e->Handled) { RaiseEvent(DragCompletedEvent, this, e); } }
	virtual void OnDragCanceled(DragEventArgs* e)	{ if (!e->Handled) { RaiseEvent(DragCanceledEvent, this, e); } }

private:
	void Handler_MouseMove(MouseEventArgs* e);
	void Handler_MouseDown(MouseEventArgs* e);
	void Handler_MouseUp(MouseEventArgs* e);

private:
	PointF	m_lastScreenPosition;
	bool	m_isDragging;
};

/**
	@brief		Thumb �̊O��
*/
class ThumbChrome
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ThumbChrome);

public:
	static const Property*	RenderMouseOverProperty;
	static const Property*	RenderPressedProperty;

public:
	static ThumbChrome* Create(GUIManager* manager);

	ThumbChrome(GUIManager* manager);
	virtual ~ThumbChrome();

	void SetRenderMouseOver(bool enabled) { m_renderMouseOver = enabled; }
	bool GetRenderMouseOver() const { return m_renderMouseOver; }
	void SetRenderPressed(bool enabled) { m_renderPressed = enabled; }
	bool GetRenderPressed() const { return m_renderPressed; }

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	//virtual Size MeasureOverride(Size availableSize);	// �K�v�Ȃ�����
	//virtual Size ArrangeOverride(Size finalSize);		// �K�v�Ȃ�����
	virtual void OnRender(Graphics::Painter* painter);

private:
	RefPtr<Graphics::TextureBrush>	m_bgFrameBrush;
	RefPtr<Graphics::TextureBrush>	m_bgInnerBrush;

	bool	m_renderMouseOver;
	bool	m_renderPressed;
};

} // namespace GUI
} // namespace Lumino
