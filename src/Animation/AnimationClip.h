
#pragma once
#include "Common.h"
#include "AnimationCurve.h"

namespace Lumino
{
namespace Animation
{

class AnimationClip
	: public RefObject
{
public:
	struct AnimationCurveEntry
	{
		String					RelativePath;
		AnimationCurve*			Curve;
	};

	typedef ArrayList<AnimationCurveEntry>	AnimationCurveEntryList;

public:
	AnimationClip();
	virtual ~AnimationClip();

public:

	/// �A�j���[�V�����N���b�v���̐ݒ�
	void SetName(const String& name) { m_name = name; }

	/// �A�j���[�V�����N���b�v���擾
	const String& GetName() const { return m_name; }

	/// �I�[�t���[���̎���
	double GetLastFrameTime() { return m_lastFrameTime; }

	/// �f�t�H���g�̍Đ����@�̐ݒ�
	void SetDefaultWrapMode(WrapMode mode) { m_defaultrapMode = mode; }

	/// �f�t�H���g�̍Đ����@�̎擾
	WrapMode GetDefaultWrapMode() { return m_defaultrapMode; }

	/// �A�j���[�V�����̒ǉ� (3D���f�����[�V�����ł���΁A�{�[����, SQTTransformAnimation)
	void AddAnimationCurve(const String& relativePath, AnimationCurve* curve);

	/// ���Ԃ�ݒ肵�A�e AnimationCurve �̕�Ԓl���v�Z����
	//void SetTime(double oldTime, double newTime);

	/// AnimationCurve �Ƃ��̊��蓖�Đ�p�X���̃��X�g
	const AnimationCurveEntryList& GetAnimationCurveEntryList() const { return m_animationCurveEntryList; }

private:
	String						m_name;
	double						m_lastFrameTime;	///< Get �̂��тɌv�Z����Ǝ��Ԃ�����̂ŗ\�ߌv�Z���Ď����Ă���
	WrapMode					m_defaultrapMode;
	AnimationCurveEntryList		m_animationCurveEntryList;
};

} // namespace Animation
} // namespace Lumino
