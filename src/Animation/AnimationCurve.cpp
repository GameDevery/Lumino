
#include "../Internal.h"
#include "AnimationCurve.h"

namespace Lumino
{
namespace Animation
{

//=============================================================================
// AnimationCurve
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationCurve::AnimationCurve()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationCurve::~AnimationCurve()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void AnimationCurve::SetTime(double time)
//{
//}


//=============================================================================
// VMDBezierTable
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatAnimationCurve::FloatAnimationCurve()
	: m_defaultValue(0.0f)
	, m_value(0.0f)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FloatAnimationCurve::~FloatAnimationCurve()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FloatAnimationCurve::AddKeyFrame(const FloatKeyFrame& keyFrame)
{
	// ���̂܂ܒǉ��ł���
	if (m_keyFrameList.IsEmpty() || m_keyFrameList.GetLast().FrameTime <= keyFrame.FrameTime)
	{
		m_keyFrameList.Add(keyFrame);
	}
	// �ǉ���̃\�[�g���K�v
	else
	{
		// ��r�p�t�@���N�^
		struct compare
		{
			bool operator()(const FloatKeyFrame& l, const FloatKeyFrame& r)
			{
				return l.FrameTime < r.FrameTime;
			}
		};

		m_keyFrameList.Add(keyFrame);
		std::stable_sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FloatAnimationCurve::AddKeyFrame(double frame_pos, float value)
{
	FloatKeyFrame key;
	key.FrameTime = frame_pos;
	key.Value = value;
	AddKeyFrame(key);
}

static int _cmpKey(const void* a_, const void* b_)
{
	FloatKeyFrame* l = (FloatKeyFrame*)a_;
	FloatKeyFrame* r = (FloatKeyFrame*)b_;

	if ((l)->FrameTime < (r)->FrameTime)
		return -1;
	else if ((l)->FrameTime >= (r)->FrameTime && (l)->FrameTime < ((r + 1))->FrameTime)
		return 0;
	else
		return 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FloatAnimationCurve::SetTime(double time)
{
	if (!m_keyFrameList.IsEmpty())
	{
		// time_ ���ŏ��̃t���[���ʒu���O�̏ꍇ�̓f�t�H���g�l
		if (time < m_keyFrameList.GetFront().FrameTime)
		{
			m_value = m_defaultValue;
		}
		// �L�[���ЂƂ����̏ꍇ�͂��̃L�[�̒l
		else if (m_keyFrameList.GetCount() == 1)
		{
			m_value = m_keyFrameList.GetFront().Value;
		}
		// time_ ���I�[�ȍ~�̏ꍇ�͏I�[�̒l
		else if (time >= m_keyFrameList.GetLast().FrameTime)
		{
			m_value = m_keyFrameList.GetLast().Value;
		}
		// �ȏ�ȊO�̏ꍇ�͕�Ԃ���
		else
		{
			const FloatKeyFrame* key0 = (FloatKeyFrame*)bsearch(&time, &(m_keyFrameList[0]), m_keyFrameList.GetCount(), sizeof(FloatKeyFrame), _cmpKey);
			const FloatKeyFrame* key1 = key0 + 1;

			float p0 = key0->Value;
			float p1 = key1->Value;
			float t0 = static_cast< float >(key0->FrameTime);
			float t1 = static_cast< float >(key1->FrameTime);
			float t = static_cast< float >(time - t0) / (t1 - t0);

			switch (key0->Mode)
			{
				// ��Ԗ���
			case InterpolationMode_None:
			{
				m_value = p0;
				break;
			}
				// ���`
			case InterpolationMode_Linear:
			{
				m_value = p0 + (p1 - p0) * t;
				break;
			}
				// �������x
			case InterpolationMode_QuadAccel:
			{
				m_value = Math::QuadAccel(p0, key0->Velocity, key0->Accel, static_cast< float >(time - key0->FrameTime));
				break;
			}
				// �O�����
			case InterpolationMode_Hermite:
			{
				m_value = Math::Hermite(
					p0, p1,
					key0->RightSlope,
					key1->LeftSlope,
					t);
				break;
			}
				// Catmull-Rom
			case InterpolationMode_CatmullRom:
			{
				// ���[�v�Đ��� time ���I�[�𒴂��Ă���ꍇ�A
				// ���̎��_��key �̒l�� ���[�v�J�n�ʒu�̂ЂƂO�̃L�[���w���Ă���

				const FloatKeyFrame& begin = m_keyFrameList.GetFront();
				const FloatKeyFrame& end = m_keyFrameList.GetLast();

				// ���̕�Ԃɂ́Abegin �̂ЂƂO�� end �̂ЂƂ�̒l���K�v�B
				// ���ꂼ�ꂪ�n�_�A�I�_�̏ꍇ�̓��[�v����悤�ɕ�Ԃ���
				m_value = Math::CatmullRom(
					((key0->FrameTime == begin.FrameTime) ? end.Value : (key0 - 1)->Value),
					p0,
					p1,
					((key1->FrameTime == end.FrameTime) ? begin.Value : (key0 + 2)->Value),
					t);
				break;
			}
			}
		}
	}
	else
	{
		m_value = m_defaultValue;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
double FloatAnimationCurve::GetLastFrameTime() const
{
	if (m_keyFrameList.IsEmpty()) return 0;
	return m_keyFrameList.GetLast().FrameTime;
}


//=============================================================================
// VMDBezierTable
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
	void VMDBezierTable::Initialize(float fPointX1, float fPointY1, float fPointX2, float fPointY2)
{
	if (fPointX1 == fPointY1 && fPointX2 == fPointY2)
	{
		m_isLinear = true;
	}
	else
	{
		fPointX1 = (fPointX1 / 127.0f) * 3.0f;
		fPointY1 = (fPointY1 / 127.0f) * 3.0f;

		fPointX2 = (fPointX2 / 127.0f) * 3.0f;
		fPointY2 = (fPointY2 / 127.0f) * 3.0f;

		// �������̂��߃e�[�u�������Ă���
		m_yValue[0] = 0.0f;
		m_yValue[YVAL_DIV_NUM] = 1.0f;

		float	fAddX = 1.0f / (float)YVAL_DIV_NUM;

		for (int i = 1; i < YVAL_DIV_NUM; i++)
		{
			m_yValue[i] = GetYVal(fAddX * i, fPointX1, fPointY1, fPointX2, fPointY2);
		}

		m_isLinear = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
	float VMDBezierTable::GetInterValue(float fX)
{
	if (m_isLinear)	return fX;	// ���`���

	// �e�[�u������`��Ԃ���
	fX *= (float)YVAL_DIV_NUM;

	int		iIdx = (int)fX;

	fX -= iIdx;

	return m_yValue[iIdx] * (1.0f - fX) + m_yValue[iIdx + 1] * fX;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float VMDBezierTable::GetYVal(float fX, float fX1, float fY1, float fX2, float fY2)
{
	float	fT = fX;
	float	fInvT = 1.0f - fT;

	for (int i = 0; i < 32; i++)
	{
		float	fTempX = fInvT*fInvT*fT*fX1 + fInvT*fT*fT*fX2 + fT*fT*fT;

		fTempX -= fX;
		if (fabsf(fTempX) < 0.0001f)
		{
			break;
		}
		else
		{
			fT -= fTempX * 0.5f;
			fInvT = 1.0f - fT;
		}
	}

	return fInvT*fInvT*fT*fY1 + fInvT*fT*fT*fY2 + fT*fT*fT;
}

//=============================================================================
// VMDBezierSQTTransformAnimation
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VMDBezierSQTTransformAnimation::VMDBezierSQTTransformAnimation()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VMDBezierSQTTransformAnimation::~VMDBezierSQTTransformAnimation()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VMDBezierSQTTransformAnimation::AddKeyFrame(
	double framePos, const Vector3& pos, const Quaternion& rot,
	char* interpolation_x,
	char* interpolation_y,
	char* interpolation_z,
	char* interpolation_rot)
{
	m_keyFrameList.Add(KeyFrame());
	KeyFrame& key = m_keyFrameList.GetLast();

	key.Time = framePos;
	key.Position = pos;
	key.Rotation = rot;
	key.Rotation.Normalize();

	key.PosXInterBezier.Initialize(interpolation_x[0], interpolation_x[4], interpolation_x[8], interpolation_x[12]);
	key.PosYInterBezier.Initialize(interpolation_y[0], interpolation_y[4], interpolation_y[8], interpolation_y[12]);
	key.PosZInterBezier.Initialize(interpolation_z[0], interpolation_z[4], interpolation_z[8], interpolation_z[12]);
	key.RotInterBezier.Initialize(interpolation_rot[0], interpolation_rot[4], interpolation_rot[8], interpolation_rot[12]);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VMDBezierSQTTransformAnimation::SortKeyFrame()
{
	struct
	{
		bool operator()(const KeyFrame& l, const KeyFrame& r) const
		{
			return l.Time < r.Time;
		}
	} compare;

	std::stable_sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
	//std::sort(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VMDBezierSQTTransformAnimation::SetTime(double time)
{
	// �t���[���� 1 ��
	if (m_keyFrameList.GetCount() == 1)
	{
		m_transform.Rotation = m_keyFrameList.GetFront().Rotation;
		m_transform.Translation = m_keyFrameList.GetFront().Position;
		return;
	}

	// �ŏ��̃t���[���ȑO�ł���΍ŏ��̃t���[���̒l��Ԃ�
	if (time <= m_keyFrameList.GetFront().Time)
	{
		m_transform.Rotation = m_keyFrameList.GetFront().Rotation;
		m_transform.Translation = m_keyFrameList.GetFront().Position;
		return;
	}

	// �Ō�̃t���[���ȍ~�ł���΍Ō�̃t���[���̒l��Ԃ�
	if (time >= m_keyFrameList.GetLast().Time)
	{
		m_transform.Rotation = m_keyFrameList.GetLast().Rotation;
		m_transform.Translation = m_keyFrameList.GetLast().Position;
		return;
	}

	// ����t���[���ʒu���O�̃L�[�t���[�����������邽�߂̊֐��I�u�W�F�N�g
	struct GreaterEqual
	{
		double FramePos;
		bool operator()(const KeyFrame& key) const
		{
			return FramePos <= key.Time;
		}
	} compare;
	compare.FramePos = time;

	// �L�[����
	KeyFrameList::iterator itr = std::find_if(m_keyFrameList.begin(), m_keyFrameList.end(), compare);
	//if ( itr == m_keyFrameList.end() ) // ���肦�Ȃ��͂�
	//{
	//    m_transform = LSQTTransform::IDENTITY;
	//}

	//lnU32   k0idx = itr - m_keyFrameList.begin();
	//lnU32   k1idx = k0idx + 1;

	int k1idx = itr - m_keyFrameList.begin();
	int k0idx = k1idx - 1;


	KeyFrame& k0 = m_keyFrameList[k0idx];
	KeyFrame& k1 = m_keyFrameList[k1idx];

	double t0 = k0.Time;
	double t1 = k1.Time;

	// float �� double �̌^�ϊ���}���邽�߁A�ȉ��� float �Ōv�Z���s��

	float rate = static_cast< float >((time - t0) / (t1 - t0));
	float inter;

	inter = k1.PosXInterBezier.GetInterValue(rate);
	m_transform.Translation.X = k0.Position.X * (1.0f - inter) + k1.Position.X * inter;

	inter = k1.PosYInterBezier.GetInterValue(rate);
	m_transform.Translation.Y = k0.Position.Y * (1.0f - inter) + k1.Position.Y * inter;

	inter = k1.PosZInterBezier.GetInterValue(rate);
	m_transform.Translation.Z = k0.Position.Z * (1.0f - inter) + k1.Position.Z * inter;

	inter = k1.RotInterBezier.GetInterValue(rate);
	m_transform.Rotation = Quaternion::Slerp(k0.Rotation, k1.Rotation, inter);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
double VMDBezierSQTTransformAnimation::GetLastFrameTime() const
{
	if (m_keyFrameList.IsEmpty()) return 0;
	return m_keyFrameList.GetLast().Time;
}

} // namespace Animation
} // namespace Lumino
