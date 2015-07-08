
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

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */
	
	/// ScrollEvent �Ƀn���h����o�^���܂��B
	void AddScrollEventHandler(Delegate02<CoreObject*, ScrollEventArgs*> handler) { m_scrollEvent += handler; }

	/// ScrollEvent �ɓo�^���ꂽ�n���h�����������܂��B
	void RemoveScrollEventHandler(Delegate02<CoreObject*, ScrollEventArgs*> handler) { m_scrollEvent -= handler; }

	/** @} */

protected:
	virtual void OnScroll(ScrollEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }

private:
	Orientation	m_orientation;
	Event02<CoreObject*, ScrollEventArgs*>	m_scrollEvent;

};


} // namespace GUI
} // namespace Lumino
