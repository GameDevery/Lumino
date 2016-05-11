
#include "../Internal.h"
#include <Lumino/Reflection/ReflectionEventArgs.h>
#include <Lumino/Animation/AnimatableObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// AnimatableObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(AnimatableObject, Object);

//------------------------------------------------------------------------------
AnimatableObject::AnimatableObject()
{
}

//------------------------------------------------------------------------------
AnimatableObject::~AnimatableObject()
{
}

//------------------------------------------------------------------------------
void AnimatableObject::OnPropertyChanged(tr::PropertyChangedEventArgs* e)
{
	Object::OnPropertyChanged(e);

	// Animation �ɂ��ݒ�ȊO�̏ꍇ�͈�x�A�j���[�V�������~����
	if (e->cause != tr::PropertySetSource::ByAnimation)
	{
		DeactivatePropertyAnimation(e->changedProperty);
	}
}

//------------------------------------------------------------------------------
void AnimatableObject::DeactivatePropertyAnimation(const tr::Property* targetProperty)
{
	// �Đ����̃A�j���̒��ɓ����^�[�Q�b�g�̓����v���p�e�B���A�j���[�V�������Ă�����̂�����Β�~����
	for (auto& clock : m_playingAnimationClockList)
	{
		for (auto& playingCurveLineInst : clock->m_instanceList)
		{
			if (playingCurveLineInst->targetObject == this &&
				playingCurveLineInst->targetProperty == targetProperty)
			{
				playingCurveLineInst->isActive = false;
			}
		}
	}
}

LN_NAMESPACE_END

