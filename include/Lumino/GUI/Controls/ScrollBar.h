
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ScrollBar
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollBar);
public:
	static const EventID	ScrollEvent;

	// TODO: RoutedCommand �� ID �ł͂Ȃ� Command �̃|�C���^�𒼐ڌ��J���������ǂ������B
	// button->SetCommand(Window.CloseCommand) �Ƃ��������B

public:
	ScrollBar(GUIManager* manager);
	virtual ~ScrollBar();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/// ScrollBar �̕������w�肵�܂��B
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/// ScrollBar �̕������擾���܂��B�K��l�� Orientation::Horizontal �ł��B
	Orientation GetOrientation() const { return m_orientation; }

	// TODO:����� RangeBase �Ɉڂ�����

	/// �X�N���[���ʒu�ɑ΂���l��ݒ肵�܂��B
	void SetValue(float value) { m_value = value; }

	/// �X�N���[���ʒu�ɑ΂���l���擾���܂��B�K��l�� 0 �ł��B
	float GetValue() const { return m_value; }

	/// �w��\�ȍŏ��l��ݒ肵�܂��B
	void SetMinimum(float value) { m_minimum = value; }

	/// �w��\�ȍŏ��l���擾���܂��B�K��l�� 0 �ł��B
	float GetMinimum() const { return m_minimum; }

	/// �w��\�ȍő�l��ݒ肵�܂��B
	void SetMaximum(float value) { m_maximum = value; }

	/// �w��\�ȍő�l���擾���܂��B�K��l�� 1 �ł��B
	float GetMaximum() const { return m_maximum; }

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */
	
	/// ScrollEvent �Ƀn���h����o�^���܂��B
	void AddScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent += handler; }

	/// ScrollEvent �ɓo�^���ꂽ�n���h�����������܂��B
	void RemoveScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent -= handler; }

	/** @} */

protected:
	virtual void OnScroll(ScrollEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	Event01<ScrollEventArgs*>	m_scrollEvent;

};


} // namespace GUI
} // namespace Lumino
