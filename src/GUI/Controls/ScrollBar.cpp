/*
	�EWPF
		ScrollBar
			Grid
				RepeatButton
				Track
				RepeatButton

		Track
			RepeatButton	��̗]��
			Thumb
				ScrollChrome
			RepeatButton	���̗]��

	�܂݂̃T�C�Y�� Track.ArrangeOverride �Ō��߂Ă����B

	ScrollBar �̃X�^�C���ƃe���v���[�g
	https://msdn.microsoft.com/ja-jp/library/ms742173%28v=vs.110%29.aspx?f=255&MSPPError=-2147217396
	PART_Track ������B


	Track.Value �� RangeBase.Value �� TemplateBinding ���Ă���B
*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollBar.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// ScrollBar
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollBar, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollBar);

LN_DEFINE_PROPERTY_ENUM_2(ScrollBar, Orientation, OrientationProperty, "Orientation", Orientation::Horizontal, &ScrollBar::SetOrientation, &ScrollBar::GetOrientation);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar* ScrollBar::Create(GUIManager* manager)
{
	auto obj = RefPtr<ScrollBar>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::ScrollBar(GUIManager* manager)
	: Control(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::~ScrollBar()
{
}

} // namespace GUI
} // namespace Lumino

