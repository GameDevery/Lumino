
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/AnimationClock.h>
#include <Lumino/GUI/VisualTreeHelper.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// AnimationTimeline
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationTimeline::AnimationTimeline()
	: m_targetName()
	, m_targetProperty()
	, m_duration(0)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationTimeline::~AnimationTimeline()
{

}


//=============================================================================
// AnimationClock
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClock::AnimationClock(Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines)
	: m_sourceStoryboard(sourceStoryboard)
	, m_timeLineInstanceList()
{
	for (AnimationTimeline* timeline : *timelines)
	{
		// �^�C�����C���̓K�p���I������
		UIElement* target;
		if (timeline->GetTargetName().IsEmpty()) {
			target = owner;
		}
		else {
			target = VisualTreeHelper::FindChildByName(owner, timeline->GetTargetName());
		}

		// �^�C�����C���̓K�p��v���p�e�B����������
		if (target != NULL)
		{
			// TODO:TypeInfo��static�֐��ɂ�����
			Property* prop = GetTypeInfo(target)->FindProperty(timeline->GetTargetProperty());
			if (prop != NULL)
			{
				TimeLineInstance tli;
				tli.OwnerTimeLine = timeline;
				tli.TargetElement = target;
				tli.TargetProperty = prop;
				tli.StartValue = target->GetPropertyValue(prop);
				m_timeLineInstanceList.Add(tli);
			}
		}
	}

	// �����l�Z�b�g�̈Ӗ������߂āA�����ł܂��� 0 �^�C���Œl���Z�b�g���Ă����B
	SetTime(0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationClock::~AnimationClock()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::SetTime(float time)
{
	for (TimeLineInstance& tli : m_timeLineInstanceList)
	{
		tli.OwnerTimeLine->Apply(tli.TargetElement, tli.TargetProperty, tli.StartValue, time);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::StopInternal()
{
	// TODO: Manager ����O��
}

} // namespace GUI
} // namespace Lumino

