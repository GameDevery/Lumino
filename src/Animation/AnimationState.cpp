
#include "../Internal.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "AnimationState.h"

namespace Lumino
{
namespace Animation
{

//=============================================================================
// AnimationState
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationState::AnimationState(AnimationClip* clip)
	: m_clip(NULL)
	, m_animationTargetList()
	, m_currentLocalTime(0.0)
	, m_state(PlayState_Stopped)
	, m_addingBlendWeight(1.0f)
{
	LN_REFOBJ_SET(m_clip, clip);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationState::~AnimationState()
{
	ClearTargetList();
	LN_SAFE_RELEASE(m_clip);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& AnimationState::GetName() const
{
	return m_clip->GetName();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationState::Refresh(Animator* animator)
{
	LN_VERIFY(animator != NULL) { return; }

	ClearTargetList();

	// Curve �̓K�p��� element ����T���A������� t �Ɏ����Ă���
	LN_FOREACH(const AnimationClip::AnimationCurveEntry& e, m_clip->GetAnimationCurveEntryList())
	{
		AnimationTargetAttributeEntity* target = animator->FindAnimationTargetAttributeEntity(e.RelativePath);
		if (target != NULL)
		{
			AnimationTarget t;
			t.Curve = e.Curve;
			t.Target = target;
			LN_SAFE_ADDREF(t.Curve);
			m_animationTargetList.Add(t);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationState::SetPlayState(PlayState state)
{
	// �V�����Đ����J�n����ꍇ�͎��Ԃ����Z�b�g����
	if (m_state == PlayState_Stopped && state == PlayState_Playing)
	{
		m_state = state;
		SetLocalTime(0.0);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationState::AdvanceTime(double elapsedTime)
{
	if (m_state == PlayState_Playing)
	{
		m_currentLocalTime += elapsedTime;
		SetLocalTime(m_currentLocalTime);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationState::ClearTargetList()
{
	LN_FOREACH(AnimationTarget& target, m_animationTargetList)
	{
		target.Curve->Release();
	}
	m_animationTargetList.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AnimationState::SetLocalTime(double time)
{
	m_currentLocalTime = time;
	LN_FOREACH(AnimationTarget& target, m_animationTargetList)
	{


		//if (target.Target->Target->GetAnimationTargetName() == _T("�Z���^�[")) {
		//	printf("a");
		//}

		// ���Ԃ��Z�b�g���Ēl�𐶐�����
		target.Curve->SetTime(time);

		// �l�̌^�ɉ����ău�����h�����������A�o�͂���
		target.Target->Type = target.Curve->GetValueType();
		switch (target.Target->Type)
		{
		case ValueType_Float:
		{
			*((float*)target.Target->Buffer) = static_cast<FloatAnimationCurve*>(target.Curve)->GetValue();
			break;
		}
		case ValueType_Vector3:
		{
			LN_THROW(0, NotImplementedException);
			break;
		}
		case ValueType_Quaternion:
		{
			LN_THROW(0, NotImplementedException);
			break;
		}
		case ValueType_SQTTransform:
		{
			if (m_addingBlendWeight != 1.0f)
			{
				SQTTransform v = static_cast<VMDBezierSQTTransformAnimation*>(target.Curve)->GetValue();
				SQTTransform* t = (SQTTransform*)target.Target->Buffer;
				t->Scale += v.Scale * m_addingBlendWeight;
				t->Rotation *= Quaternion::Slerp(Quaternion::Identity, v.Rotation, m_addingBlendWeight);
				t->Translation += v.Translation * m_addingBlendWeight;
			}
			else
			{
				*((SQTTransform*)target.Target->Buffer) = static_cast<VMDBezierSQTTransformAnimation*>(target.Curve)->GetValue();
			}
			target.Target->Modified = true;	// �l���Z�b�g����
			break;
		}
		//case ValueType_Event:
		//	// �l�^�ł͂Ȃ��̂ŉ������Ȃ�
		//	break;
		default:
			break;
		}
	}
}

} // namespace Animation
} // namespace Lumino
