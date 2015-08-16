
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/VisualState.h>
#include <Lumino/GUI/VisualTreeHelper.h>
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{

//=============================================================================
// FloatEasing
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatEasing::FloatEasing()
	:/* m_targetName()
	, m_targetProperty(NULL)
	, */m_targetValue(0)
	, m_easingMode(Animation::EasingMode::Linear)
	//, m_duration(0)
	, m_easingFunction()
{
	SetEasingMode(Animation::EasingMode::Linear);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatEasing::~FloatEasing()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FloatEasing::SetEasingMode(Animation::EasingMode easingMode)
{
	m_easingMode = easingMode;
	m_easingFunction = AnimationUtilities::SelectEasingFunction<float>(m_easingMode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool FloatEasing::Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time)
{
	float value = 0;

	// �o�ߎ��� 0 �̏ꍇ�͂��̂܂܃Z�b�g�ŗǂ��B0���Z�΍�̈Ӗ������߂āB
	// �܂��A���Ԃ����ɏI�[�𒴂��Ă�����A��r�֐��������ꍇ�����l�Z�b�g�B
	if (m_duration == 0 || m_duration <= time || m_easingFunction == nullptr)
	{
		value = m_targetValue;
	}
	// ���Ԃ� 0 �ȑO�̏ꍇ�͏����l�B
	else if (time <= 0)
	{
		value = Variant::Cast<float>(startValue);
	}
	// ��Ԃŋ��߂�
	else
	{
		float b = Variant::Cast<float>(startValue);
		value = m_easingFunction(time, b, m_targetValue - b, m_duration);
	}

	targetElement->SetTypedPropertyValue<float>(targetProp, value);
	return (time < m_duration);
}

//=============================================================================
// VisualState
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::VisualState(GUIManager* manager)
	: m_manager(manager)
{
	m_storyboard.Attach(LN_NEW Storyboard(m_manager));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::VisualState(GUIManager* manager, const String& name)
	: m_manager(manager)
	, m_name(name)
{
	m_storyboard.Attach(LN_NEW Storyboard(m_manager));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualState::~VisualState()
{
}

//=============================================================================
// VisualStateGroup
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateGroup::VisualStateGroup()
	: m_name()
	, m_currentState(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateGroup::VisualStateGroup(const String& name)
	: m_name(name)
	, m_currentState(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateGroup::~VisualStateGroup()
{
}

//=============================================================================
// VisualStateGroup
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateManager::VisualStateManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualStateManager::~VisualStateManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualStateManager::GoToState(Control* control, const String& stateName)
{
	LN_CHECK_ARGS_RETURN(control != NULL);
	if (stateName.IsEmpty()) { return; }

	VisualStateGroupList* groupList = control->GetVisualStateGroups();
	for (auto* group : *groupList)
	{
		VisualStateList* stateList = group->GetVisualStateList();
		VisualStateList::iterator itr = std::find_if(stateList->begin(), stateList->end(), [stateName](VisualState* state) { return state->GetName() == stateName; });
		if (itr != stateList->end())
		{
			// �Đ����̃A�j�����~�߂�
			VisualState* current = group->GetCurrentState();
			if (current != NULL) {
				current->GetStoryboard()->Stop(control);
			}

			group->SetCurrentState(*itr);				// �J�����g�ύX
			(*itr)->GetStoryboard()->Begin(control);	// �A�j���[�V�����J�n
			return;	// �����I��
		}
	}
}


//=============================================================================
// Storyboard
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Storyboard::Storyboard(GUIManager* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Storyboard::~Storyboard()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Storyboard::AddTimeline(AnimationTimeline* timeline)
{
	RefPtr<AnimationTimeline> t(timeline, true);
	m_animationTimelineList.Add(t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Storyboard::Begin(UIElement* target)
{
	// TODO: AnimationClock �͕p�ɂɍ쐬�����̂ŃL���b�V��������
	RefPtr<AnimationClock> clock(LN_NEW AnimationClock(m_manager, this, target, &m_animationTimelineList));

	// TODO: Manager �ɂ��o�^���ăA�j�����Ă��炤
	GUIHelper::UIElement_GetAnimationClockList(target)->Add(clock);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Storyboard::Stop(UIElement* target)
{
	Array< RefPtr<AnimationClock> >* list = GUIHelper::UIElement_GetAnimationClockList(target);
	Array< RefPtr<AnimationClock> >::iterator itr = list->begin();
	for (; itr != list->end();)
	{
		if ((*itr)->m_sourceStoryboard == this)
		{
			(*itr)->StopInternal();
			itr = list->erase(itr);
		}
		else {
			++itr;
		}
	}
}


//=============================================================================
// VisualStatus
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String VisualStatus::Normal = _T("Normal");
const String VisualStatus::MouseOver = _T("MouseOver");

} // namespace GUI
} // namespace Lumino

