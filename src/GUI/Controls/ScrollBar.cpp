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
	��
	�ł��A�����܂ł��Ȃ��Ă����C������B
	ScrollBar �� Slider �� "PART_Track" �Ƃ��Ďq�v�f�� Track �����邱�Ƃ�O��Ƃ��Ă���B
	(null �͉\�����A���ۂ���� ScrollBar ����邱�Ƃ�����̂��E�E�E�H)
	�ł���΁ATemplateBinding ���Ȃ��Ă� ScrollBar �����璼�ڒl�� set ���Ă�����Ȃɖ��Ȃ��B
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
// ScrollEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Lumino::GUI::ScrollEventArgs, EventArgs);

//=============================================================================
// ScrollBar
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollBar, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollBar);

const String ScrollBar::PART_TrackKeyName(_T("PART_Track"));

LN_DEFINE_PROPERTY_2(ScrollBar, float, ValueProperty, "Value", 0.0f, &ScrollBar::SetValue, &ScrollBar::GetValue);
LN_DEFINE_PROPERTY_ENUM_2(ScrollBar, Orientation, OrientationProperty, "Orientation", Orientation::Horizontal, &ScrollBar::SetOrientation, &ScrollBar::GetOrientation);

LN_DEFINE_ROUTED_EVENT(ScrollBar, ScrollEventArgs, ScrollEvent, "DragStarted", Scroll);

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
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_orientation(Orientation::Horizontal)
	, m_scrollEvent()
	, m_track(NULL)
	, m_dragStartValue(0.0f)
{	
	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(ScrollBar, DragEventArgs, Thumb::DragStartedEvent, Handler_Thumb_DragStarted);
	LN_REGISTER_ROUTED_EVENT_HANDLER(ScrollBar, DragEventArgs, Thumb::DragDeltaEvent, Handler_Thumb_DragDelta);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::~ScrollBar()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::PollingTemplateChildCreated(UIElement* newElement)
{
	// PART �̃L�[�Ɉ�v���閼�O�����v�f�����������烁���o�ɕێ����Ă���
	if (newElement->GetKeyName() == PART_TrackKeyName) {
		m_track = dynamic_cast<Track*>(newElement);
	}

	Control::PollingTemplateChildCreated(newElement);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::UpdateValue(float horizontalDragDelta, float verticalDragDelta)
{
	float valueDelta = m_track->ValueFromDistance(horizontalDragDelta, verticalDragDelta);

	float newValue = m_dragStartValue/*GetValue()*/ + valueDelta;
	ChangeValue(newValue);
	RefPtr<ScrollEventArgs> args(m_manager->GetEventArgsPool()->Create<ScrollEventArgs>(newValue, ScrollEventType::ThumbTrack));
	RaiseEvent(ScrollEvent, this, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::ChangeValue(float newValue/*, bool defer*/)
{
	if (m_isStandalone)
	{
		// �~ Track �� TemplateBinding�������̂ŁB
		// TODO: ����ς� SetValue() �̒��ł��悤�ɂ����ق����ǂ�����
		SetPropertyValue(ValueProperty, newValue);

		// TODO: OnValueChannged �Ƃ�����āA������ōs���ׂ��B
		m_track->SetValue(newValue);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::Handler_Thumb_DragStarted(DragEventArgs* e)
{
	m_dragStartValue = m_value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::Handler_Thumb_DragDelta(DragEventArgs* e)
{
	UpdateValue(e->XOffset, e->YOffset);
}

} // namespace GUI
} // namespace Lumino

