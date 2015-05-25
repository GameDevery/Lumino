
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Animation
{

/// �l��⊮����N���X�̃x�[�X�N���X
class AnimationCurve
	: public RefObject
{
public:
	AnimationCurve();
	virtual ~AnimationCurve();

public:
	/// �l�̎��
	virtual ValueType GetValueType() const = 0;

	/// ���Ԃ̐ݒ� (mTime �����̂܂܁AmCurrentFramePos �����[�v�l���Őݒ肷��)
	virtual void SetTime(double time) = 0;

	/// �I�[�t���[���ʒu�̎擾
	virtual double GetLastFrameTime() const = 0;

};

/// FloatAnimationCurve �̃L�[�t���[��
struct FloatKeyFrame
{
public:
	double				FrameTime;		///< �t���[���ʒu
	float				Value;			///< �l
	float				Velocity;		///< ���x
	float				Accel;			///< �����x
	float				LeftSlope;		///< ���� (�I��) ���x
	float				RightSlope;		///< �E�� (�J�n) ���x
	InterpolationMode	Mode;			///< ���̃L�[�Ƃ̕�ԕ��@ (�����l�͐��`���)

public:
	FloatKeyFrame()
		: FrameTime(0.0f)
		, Value(0.0f)
		, Velocity(0.0f)
		, Accel(0.0f)
		, LeftSlope(0.0f)
		, RightSlope(0.0f)
		, Mode(InterpolationMode_Linear)
	{}
};

/// float �p
class FloatAnimationCurve
	: public AnimationCurve
{
public:
	FloatAnimationCurve();
	virtual ~FloatAnimationCurve();

public:

	/// �L�[�t���[���ǉ� (�I�[���O�ɒǉ������ꍇ�̓\�[�g���s��)
	void AddKeyFrame(const FloatKeyFrame& keyFrame);

	/// �L�[�t���[���ǉ� (���`��Ԃ̃L�[�t���[��)
	void AddKeyFrame(double frame_pos, float value);

	/// ��Ԍ��ʂ̎擾 (SetTime() �ōX�V�����)
	float GetValue() const { return m_value; }

public:
	// override AnimationCurve
	virtual ValueType GetValueType() const { return ValueType_Float; }
	virtual void SetTime(double time);
	virtual double GetLastFrameTime() const;

private:
	typedef ArrayList<FloatKeyFrame>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	float			m_defaultValue;
	float			m_value;
};

/// VMD�pBezier
class VMDBezierTable
{
public:
	/// ������
	void Initialize(float pt_x1, float pt_y1, float pt_x2, float pt_y2);

	/// t = 0.0�`1.0
	float GetInterValue(float t);

private:
	float GetYVal(float x, float x1, float y1, float x2, float y2);

private:
	static const int	YVAL_DIV_NUM = 16;
	float	            m_yValue[YVAL_DIV_NUM + 1];
	bool	            m_isLinear;
};


/// VMD�p
class VMDBezierSQTTransformAnimation
	: public AnimationCurve
{
public:
	struct KeyFrame
	{
		double			Time;				// �t���[���ʒu
		Vector3			Position;			// �ʒu
		Quaternion		Rotation;			// ��](�N�H�[�^�j�I��)

		VMDBezierTable	PosXInterBezier;	// X���ړ����
		VMDBezierTable	PosYInterBezier;	// Y���ړ����
		VMDBezierTable	PosZInterBezier;	// Z���ړ����
		VMDBezierTable	RotInterBezier;	    // ��]���
	};

public:
	VMDBezierSQTTransformAnimation();
	virtual ~VMDBezierSQTTransformAnimation();

public:

	/// �L�[�t���[���ǉ�
	void AddKeyFrame(
		double framePos, const Vector3& pos, const Quaternion& rot,
		char* interpolation_x,
		char* interpolation_y,
		char* interpolation_z,
		char* interpolation_rot);

	/// �L�[�t���[���̃\�[�g
	void SortKeyFrame();

	/// ��Ԍ��ʂ̎擾 (SetTime() �ōX�V�����)
	const SQTTransform& GetValue() const { return m_transform; }

public:
	virtual ValueType GetValueType() const { return ValueType_SQTTransform; }
	virtual void SetTime(double time);
	virtual double GetLastFrameTime() const;

private:
	typedef ArrayList<KeyFrame>	KeyFrameList;
	KeyFrameList	m_keyFrameList;
	SQTTransform	m_transform;
};

} // namespace Animation
} // namespace Lumino
