
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/AnimationClock.h>
#include <Lumino/GUI/VisualTreeHelper.h>
#include "GUIHelper.h"

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
AnimationClock::AnimationClock(GUIManager* manager, Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines)
	: m_manager(manager)
	, m_sourceStoryboard(sourceStoryboard)
	, m_timeLineInstanceList()
	, m_currentTime(0)
	, m_isFinished(false)
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
				tli.StartValue = target->GetPropertyValue(prop);	// ���݂̃v���p�e�B�l���J�n�l�Ƃ���
				m_timeLineInstanceList.Add(tli);
			}
		}
	}

	// �����l�Z�b�g�̈Ӗ������߂āA�����ł܂��� 0 �^�C���Œl���Z�b�g���Ă����B
	SetTime(0);

	GUIHelper::GUIManager_AddAnimationClock(m_manager, this);
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
	// �Ƃ肠���� true �ɂ��Ă����āA�^�C�����C����1�ł����s���������� false �ɂ���
	m_isFinished = true;

	for (TimeLineInstance& tli : m_timeLineInstanceList)
	{
		bool r = tli.OwnerTimeLine->Apply(tli.TargetElement, tli.TargetProperty, tli.StartValue, time);
		if (r) {
			m_isFinished = false;
		}
	}
	
	// �I�������� Manager �̎��ԊǗ�����O���c�̂����A
	// ���̊֐��̒��ł͂܂��C�e���[�g���ł���Bm_isFinished �t���O�� Manager �Ō��āA������ŊO���Ă��炤�B
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::AdvanceTime(float elapsedTime)
{
	m_currentTime += elapsedTime;
	SetTime(m_currentTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationClock::StopInternal()
{
	// Manager ����O��
	GUIHelper::GUIManager_RemoveAnimationClock(m_manager, this);
}

} // namespace GUI
} // namespace Lumino

