/*
	[2015/5/18]
	Static �Ȓ��_�o�b�t�@�̕`��́ADrawPrimitiveUP �������Ȃ��Ƃ� 4 �{�͑����B
	���[����񃂃f���ł� 4ms �������Ă��̂� 0ms �ȉ��ɂȂ����B(���_�� 37000)

	����A�S Lock ����� Static �ȕ��� 50ms �Ƃ����������B
	���� Lock ���ƁALock �������_���ɔ�Ⴗ��B
	�����x�Ȃ� 0ms�A������ 18500���� 25ms �O��ɂȂ�B

	Dynamic �Ȓ��_�o�b�t�@�̕`��́ALock ���Ȃ���� 1ms ���炢�B
	Lock ����� 5�`10ms�B���� Lock �ł��S Lock �ł��ς��Ȃ������B


	�L�͂�p�ɂɏ���������Ȃ�A���ϓI�Ɍ��� DrawPrimitiveUP ����Ԃ悳�����ł͂��邪�A
	��� 4ms ��������̂̓Q�[���Ƃ��Ă͍l�����́B


	���ۂɎg����̂͂قƂ�Ǖ\��A�j���[�V�����ŁA���������̑ΏۂƂȂ钸�_���͂�������1000���x�B
	�i���܂Ɋ͑����ۂ��Ək�����ĉB���Ȃ�Ă��Ƃ���Ă郂�f�������邪�j
	Static �� 1000�� Lock �͖� 2ms �������B





	�E���b�V���R���e�i�̕K�v��
	�}�e���A���ł͘d������Ȃ��f�[�^�̃O���[�v���Ɏg���B�Ⴆ�΁A
	�E���_�t�H�[�}�b�g���قȂ�
	�Econstant buffer ���g�p�����X�L�j���O�̂��߂̃{�[���̃O���[�v��
	�E���_�J���[�ɓ������܂܂����̂͌ォ��`�悵����




	[2015/5/17]

	IAnimationTargetElement
		virtual IAnimationTargetAttribute[] GetAnimationTargetAttributes() = 0;
		�A�j���[�V������K�p�����A�̃I�u�W�F�N�g�̐e�v�f�B
		�E3D ���f���Ȃ烂�f�����̂��́B
		�EGUI �Ȃ� Control�B

	IAnimationTargetAttribute
		virtual GetName() = 0;
		virtual SetValue() = 0;
		��Ԓl��K�p����I�u�W�F�N�g�B
		�E3D ���f���Ȃ�m�[�h�A���[�t�B
		�EGUI �Ȃ�v���p�e�B�B


	AnimationController
		AnimationClip[]

	AnimationClip
		Name
		{ AnimationCurve, TargetName }[]
		AnimationCurve���O���[�v������B
		�E3D ���f���̃A�j���[�V����1���ɑ�������B(Name �� "����"�A"����" ��)
		�EGUI �ł͏��1���ɑ�������B("�{�^������"�A"��A�N�e�B�u" ��)
		AnimationCurve �́A���ꂪ���ɑ΂���A�j���[�V�����Ȃ̂��������L�[�����������ĊǗ�����B
		�E3D ���f���ł̓{�[�����B
		�EGUI �ł̓v���p�e�B���B
		�g���񂵂��ł���B

	AnimationCurve
		KeyFrame[]
		���ۂɒl�̕�Ԃ��s���N���X�B���ꂾ���B
		�g���񂵂��ł���B


	���C���[
		[Unity] https://psm.playstation.net/static/general/all/unity_for_psm/ja/Documentation/Manual/MecanimFAQ.html
		���C���[�̏��Ԃ͉����ɉe�����܂����H
			�͂��B���C���[�͈�ԏォ�牺�܂ŏ��ɕ]������܂��B
			 Override �ƃZ�b�g���ꂽ���C���[�͏�ɑO�̏��Ԃ̃��C���[���㏑�����܂��i�}�X�N������΁A���̃}�X�N�ɂ��ƂÂ��܂��j�B 
		
	
		�E�����U��Ȃ���ړ�����
			// ������
			animCtrl->GetAnimationState("swing")->SetLayer(1);	// �f�t�H���g�� 0
			// �t���[���X�V
			animCtrl->Play("run", WrapMode_Loop);	// wrap �� state->SetWrapMode(WrapMode_Loop) �ł��炩���ߐݒ肵�Ă��悢
			animCtrl->Play("swing");
		
			���̃��C���[�̃A�j���[�V�����͏㏑������̂ŁA�Ⴆ�� "run" �A�j���� "�E�r" �{�[���͏㏑�������B
			�����g�́A"swing" �A�j���ɃL�[�t���[�����Ђ��Ȃ����1�x���K�p����Ȃ��̂ŁA"run" �̃A�j�������������邱�ƂɂȂ�B
		
		�E�����C���[�̃A�j���[�V�����͉��Z�u�����h�����B
		  ����͎�Ƀt�F�C�X�A�j���[�V�����p�ɂȂ肻���B
	
		�E�t�B���^�����O
			�Ⴆ�΁A����{�[���ȉ��ɂ̂ݓK�p�������A�j���[�V����������Ƃ���B
			����� Animation ���W���[�������ł͉����ł��Ȃ��BIAnimationTargetAttribute �͊K�w�\���������Ȃ����߁B
			�Ȃ̂ŁAModeling ���W���[���� MMDAnimationController �݂����Ȋg�����s�����ƂɂȂ�B

*/

#include "../Internal.h"
#include "AnimationClip.h"
#include "AnimationState.h"
#include "Animator.h"

namespace Lumino
{
namespace Animation
{

//=============================================================================
// Animator
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Animator::Animator()
	: m_element(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Animator::~Animator()
{
	LN_FOREACH(AnimationState* s, m_animationStateList)
	{
		s->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::Create(IAnimationTargetElement* element)
{
	m_element = element;

	int count = m_element->GetAnimationTargetAttributeCount();
	m_animationTargetAttributeEntityList.Resize(count);
	for (int i = 0; i < count; i++)
	{
		m_animationTargetAttributeEntityList[i].Target = m_element->GetAnimationTargetAttribute(i);
		m_animationTargetAttributeEntityList[i].Type = ValueType_Float;	// �Ƃ肠����������
		memset(m_animationTargetAttributeEntityList[i].Buffer, 0, sizeof(m_animationTargetAttributeEntityList[i].Buffer));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Animator::IsPlaying() const
{
	LN_THROW(0, NotImplementedException);
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::Play(const TCHAR* name/*, PlayMode mode*/)
{
	AnimationState* state = FindAnimationState(name);
	if (state != NULL)
	{
		// �Đ�����Ԃɂ���
		state->SetPlayState(PlayState_Playing);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::AdvanceTime(double elapsedTime)
{
	// �ꎞ�o�b�t�@���N���A���� (TODO: Valiant �^�݂����Ȃ̂�����āA�e�l���ɓK�؂ȃN���A������������������ǂ���������Ȃ��c)
	LN_FOREACH(AnimationTargetAttributeEntity& e, m_animationTargetAttributeEntityList)
	{
		memset(e.Buffer, 0, sizeof(e.Buffer));
		e.Modified = false;
	}

	// TODO: ���C���[�̍l��
	LN_FOREACH(AnimationState* state, m_animationStateList)
	{
		state->AdvanceTime(elapsedTime);
	}

	LN_FOREACH(AnimationTargetAttributeEntity& e, m_animationTargetAttributeEntityList)
	{
		//if (e.Target->GetAnimationTargetName() == _T("�E�r")) {
		//	printf("b");
		//}
		// ��A�̏����̒��Ŗ{���ɒl���Z�b�g���ꂽ���̂����ʒm����
		if (e.Modified) {
			e.Target->SetAnimationTargetValue(e.Type, e.Buffer);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::AddAnimationClip(AnimationClip* animationClip, int layer)
{
	LN_VERIFY(animationClip != NULL) { return; }

	AnimationState* state = LN_NEW AnimationState(animationClip);
	m_animationStateList.Add(animationClip->GetName(), state);
	state->Refresh(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::RemoveAnimationClip(AnimationClip* animationClip)
{
	LN_VERIFY(animationClip != NULL) { return; }
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationTargetAttributeEntity* Animator::FindAnimationTargetAttributeEntity(const String& name) const
{
	LN_FOREACH(AnimationTargetAttributeEntity& e, m_animationTargetAttributeEntityList)
	{
		if (e.Target->GetAnimationTargetName() == name) {
			return &e;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationState* Animator::FindAnimationState(const TCHAR* clipName)
{
	LN_VERIFY(clipName != NULL) { return NULL; }
	LN_FOREACH(AnimationState* state, m_animationStateList)
	{
		if (state->GetName() == clipName) {
			return state;
		}
	}
	return NULL;
}


} // namespace Animation
} // namespace Lumino
