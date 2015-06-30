
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

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
	static PropertyID	IsDraggingProperty;		///< �h���b�O���ł��邩�������l

	static EventID		DragStartedEvent;		///< �h���b�O�J�n�C�x���g
	static EventID		DragCompletedEvent;		///< �h���b�O�I���C�x���g
	static EventID		DragDeltaEvent;			///< �h���b�O���̈ړ��C�x���g

public:
	Thumb(GUIManager* manager);
	virtual ~Thumb();

	/// �h���b�O���ł��邩���m�F����
	bool IsDragging() const { return m_isDragging; }

protected:
	virtual void OnMouseMove(MouseEventArgs* e);
	virtual void OnMouseLeftButtonDown(MouseEventArgs* e);
	virtual void OnMouseLeftButtonUp(MouseEventArgs* e);

private:
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
	static PropertyID	RenderMouseOverProperty;
	static PropertyID	RenderPressedProperty;

public:
	ThumbChrome(GUIManager* manager);
	virtual ~ThumbChrome();

	void SetRenderMouseOver(bool enabled) { m_renderMouseOver = enabled; }
	bool GetRenderMouseOver() const { return m_renderMouseOver; }
	void SetRenderPressed(bool enabled) { m_renderPressed = enabled; }
	bool GetRenderPressed() const { return m_renderPressed; }

protected:
	virtual Size MeasureOverride(Size availableSize);	// �K�v�Ȃ�����
	virtual Size ArrangeOverride(Size finalSize);		// �K�v�Ȃ�����
	virtual void OnRender();

private:
	bool	m_renderMouseOver;
	bool	m_renderPressed;
};

} // namespace GUI
} // namespace Lumino
