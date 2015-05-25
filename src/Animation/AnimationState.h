
#pragma once
#include "Common.h"
#include "AnimationCurve.h"

namespace Lumino
{
namespace Animation
{

/// �A�j���[�V�����̒l��ݒ肷��I�u�W�F�N�g�̃C���^�[�t�F�C�X
class IAnimationTargetAttribute
{
public:
	virtual ~IAnimationTargetAttribute() {}

	virtual const String& GetAnimationTargetName() const = 0;
	virtual void SetAnimationTargetValue(ValueType type, const void* value) = 0;

};

/// IAnimationTargetAttribute �Ɉꎞ�o�b�t�@�������������Ȃ������̂ŁA���b�v����\���̂�p�ӂ���
struct AnimationTargetAttributeEntity
{
	IAnimationTargetAttribute*	Target;
	ValueType					Type;			///< Buffer �̒l�̌^
	byte_t						Buffer[64];		///< IAnimationTargetAttribute �ɐݒ肷��O�̌v�Z�̈ꎞ�o�b�t�@
	bool						Modified;		///< 1 �ȏ�l���ݒ肳�ꂽ��
};

/// �f�[�^���f���ł��� AnimationClip �ɑ΂���C���X�^���X
class AnimationState
	: public RefObject
{
public:
	AnimationState(AnimationClip* clip);
	~AnimationState();

public:
	/// ���O�̎擾
	const String& GetName() const;

	/// AnimationTarget �̍Ċ��蓖�Ă��s��
	void Refresh(Animator* animator);

	/// �Đ���Ԃ̐ݒ�
	void SetPlayState(PlayState state);

	/// �Đ���Ԃ̎擾
	PlayState GetPlayState() const { return m_state; }

	/// ���� AnimationState �̓����C���[���̃u�����h��
	void SetAddingBlendWeight(float weight) { m_addingBlendWeight = weight; }

	/// ���Ԃ�i�߂�
	void AdvanceTime(double elapsedTime);

private:
	void ClearTargetList();
	void SetLocalTime(double time);

private:
	struct AnimationTarget
	{
		AnimationCurve*					Curve;		// ���̃A�j���[�V������
		AnimationTargetAttributeEntity*	Target;		// TargetState.Value �ɒl���Z�b�g����
	};

	typedef ArrayList<AnimationTarget>	AnimationTargetList;

	AnimationClip*			m_clip;
	AnimationTargetList		m_animationTargetList;
	double					m_currentLocalTime;
	PlayState				m_state;
	float					m_addingBlendWeight;
};

} // namespace Animation
} // namespace Lumino
