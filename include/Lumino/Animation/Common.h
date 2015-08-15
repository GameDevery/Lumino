
#pragma once

namespace Lumino
{
namespace Animation
{
class AnimationCurve;
class AnimationClip;
class AnimationState;
class Animator;
class IAnimationTargetAttribute;
class IAnimationTargetElement;
struct AnimationTargetAttributeEntity;

/// AnimationCurve �̒l�̎��
enum ValueType
{
	ValueType_Float = 0,
	ValueType_Vector3,
	ValueType_Quaternion,
	ValueType_SQTTransform,
	//ValueType_Event,
};

/// �A�j���[�V�����̕�ԕ��@
enum InterpolationMode
{
	InterpolationMode_None = 0,		///< ��ԂȂ�
	InterpolationMode_Linear,		///< ���`���
	InterpolationMode_QuadAccel,	///< �������x
	InterpolationMode_Hermite,		///< �G���~�[�g�X�v���C�����
	InterpolationMode_CatmullRom,	///< Catmull-Rom ���
};

/// �A�j���[�V�����Đ��̏������@
enum WrapMode
{
	WrapMode_Once = 0,		///< �Ō�܂ōĐ����ꂽ��A��~����
	WrapMode_Loop,			///< �Ō�܂ōĐ����ꂽ��A�擪�ɖ߂��ă��[�v����
	//WrapMode_PingPong,		///< �Ō�܂ōĐ����ꂽ��A�t�����ɖ߂��ă��[�v����
	//WrapMode_Default,
	//WrapMode_ClampForever,
};

/// 
enum PlayMode
{
	StopSameLayer = 0,	///< �����C���[�̃A�j���[�V���������ׂĒ�~����
	StopAll,			///< �S�ẴA�j���[�V�������~����
};

/// 
enum PlayState
{
	PlayState_Stopped = 0,	///< ��~��
	PlayState_Playing,		///< �Đ���
	PlayState_Pausing,		///< �ꎞ��~��
};

/** �C�[�W���O�֐��̎�ނ�\���܂��B*/
LN_ENUM(EasingMode)
{
	Linear = 0,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
};
LN_ENUM_DECLARE(EasingMode);

} // namespace Animation
} // namespace Lumino
