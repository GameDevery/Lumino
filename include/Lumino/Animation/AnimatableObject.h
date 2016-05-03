
#pragma once
#include "Common.h"
#include "AnimationClock.h"

LN_NAMESPACE_BEGIN

/**
	@brief	�v���p�e�B�A�j���[�V������K�p�ł���N���X�̃x�[�X�N���X�B
*/
class AnimatableObject
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	AnimatableObject();
	virtual ~AnimatableObject();

protected:
	virtual void OnPropertyChanged(tr::PropertyChangedEventArgs* e) override;

private:
	void DeactivatePropertyAnimation(const tr::Property* targetProperty);

	friend class detail::AnimationManager;
	friend class AnimationClock;
	Array<RefPtr<AnimationClock>>	m_playingAnimationClockList;
};

LN_NAMESPACE_END

