
#pragma once
#include "Common.h"
#include "AnimationCurve.h"

namespace Lumino
{
namespace Animation
{

/// �A�j���[�V�����������I�u�W�F�N�g��ێ�����I�u�W�F�N�g�̃C���^�[�t�F�C�X
class IAnimationTargetElement
{
public:
	virtual ~IAnimationTargetElement() {}

	virtual int GetAnimationTargetAttributeCount() const = 0;
	virtual IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) = 0;

	/// (������Ȃ���� NULL ��Ԃ�)
	//virtual IAnimationTargetAttribute* FindAnimationTargetAttributes(const String& relativePath) const = 0;
};

/// Animator
class Animator
	: public RefObject
{
public:

public:
	Animator();
	~Animator();

public:

	/// �쐬
	void Create(IAnimationTargetElement* element);

	/// �Đ����ł��邩���m�F����
	bool IsPlaying() const;

	/// �Đ�
	void Play(const TCHAR* name/*, PlayMode mode = PlayMode_StopSameLayer*/);

	///// �u�����h (�A�j���[�V�����̍Đ��ɂ͉e�����Ȃ��B��~���̃A�j���[�V���������̊֐��ɂ���čĐ��J�n����邱�Ƃ͂Ȃ�)
	//void Blend(const lnKeyChar* animName, lnFloat targetWeight, lnFloat fadeLength);

	///// �N���X�t�F�[�h
	//void CrossFade(const lnKeyChar* animName, lnFloat fadeLength, PlayMode mode = StopSameLayer);

	///// �O�̃A�j���[�V�������I��������A�Đ����J�n����
	//void PlayQueued(const lnKeyChar* animName, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer);

	///// �O�̃A�j���[�V�������I������Ƃ��A�N���X�t�F�[�h�ōĐ����J�n����
	//void CrossFadeQueued(const lnKeyChar* animName, lnFloat fadeLength, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer);

	///// �����C���[���̃A�j���[�V�����Đ����x�̓���
	//void SyncLayer(int layer);

	/// �A�j���[�V������i�߂�
	void AdvanceTime(double elapsedTime);

public:

	/// �A�j���[�V�����N���b�v�̒ǉ�
	void AddAnimationClip(AnimationClip* animationClip, int layer = 0);

	/// �A�j���[�V�����N���b�v�̍폜
	void RemoveAnimationClip(AnimationClip* animationClip);

	/// AnimationTargetEntity �̌��� (������Ȃ���� NULL)
	AnimationTargetAttributeEntity* FindAnimationTargetAttributeEntity(const String& name) const;

	/// AnimationState �̌���
	AnimationState* FindAnimationState(const TCHAR* clipName);

	/// AnimationApplyTarget �̒ǉ� (�ł��邾�� AnimationController �쐬����ɐݒ肷�邱�ƁB�d���֎~)
	//void addAnimationApplyTarget(IAnimationApplyTarget* target);

private:
	typedef ArrayList<AnimationTargetAttributeEntity>	AnimationTargetAttributeEntityList;
	typedef SortedArray<String, AnimationState*>		AnimationStateList;

	IAnimationTargetElement*			m_element;
	AnimationTargetAttributeEntityList	m_animationTargetAttributeEntityList;
	AnimationStateList					m_animationStateList;

	///// �ǉ��ς� AnimationApplyTarget �̌���
	//AnimationApplyTargetState* findAnimationApplyTargetState(const lnKeyChar* naem);

	//typedef std::vector<AnimationState*>			AnimationStateArray;
	//typedef std::vector<AnimationApplyTargetState>	AnimationApplyTargetStateArray;

	//AnimationStateArray				mAnimationStateArray;
	//AnimationApplyTargetStateArray	mAnimationApplyTargetStateArray;
};

} // namespace Animation
} // namespace Lumino
