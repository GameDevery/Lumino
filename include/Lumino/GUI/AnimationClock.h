
#pragma once
#include "Common.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief		
	@note		
*/
class AnimationTimeline
	: public CoreObject
{
public:
	AnimationTimeline();
	virtual ~AnimationTimeline();

	/// �A�j���[�V����������I�u�W�F�N�g�̖��O (x;key �Ŏw�肳�ꂽ���O)
	/// �w�肵�Ă��Ȃ��ꍇ�͘_���v�f���^�[�Q�b�g�ƂȂ�B
	void SetTargetName(const String& name) { m_targetName = name; }

	const String& GetTargetName() const { return m_targetName; }

	/// �A�j���[�V����������v���p�e�B�̖��O
	void SetTargetProperty(const String& name) { m_targetProperty = name; }	// WPF �ł͓Y�t�v���p�e�B�Ō^�� PropertyPath

	const String& GetTargetProperty() const { return m_targetProperty; }

	void SetDuration(float duration) { m_duration = duration; }

protected:
	virtual void Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time) = 0;

protected:
	String	m_targetName;		///< �^�[�Q�b�g�� UI �v�f���B�r�W���A���c���[����Ώۗv�f����������Ƃ��Ɏg�p����B
	String	m_targetProperty;	///< �^�[�Q�b�g�v���p�e�B��
	double	m_duration;			///< �Đ����� (�~���b)

	friend class AnimationClock;
};
	

/**
	@brief		
	@note		���̃C���X�^���X�� UIElement ���ێ�����B
*/
class AnimationClock
	: public CoreObject
{
public:
	AnimationClock(Storyboard* sourceStoryboard, UIElement* owner, Array< RefPtr<AnimationTimeline> >* timelines);
	virtual ~AnimationClock();
	
	// TODO: internal
	void SetTime(float time);
	void AdvanceTime(float elapsedTime);

private:
	void StopInternal();

private:
	struct TimeLineInstance
	{
		RefPtr<AnimationTimeline>	OwnerTimeLine;
		UIElement*	TargetElement;		// TODO: �A�j�����s���� Target ���f�X�g���N�g���ꂽ���̑Ή�
		Property*	TargetProperty;
		Variant		StartValue;
	};

	Storyboard*	m_sourceStoryboard;		// ��~����Ƃ��̃L�[�ɂ������̂ŎQ�ƃJ�E���g���グ��K�v�͂Ȃ�
	Array<TimeLineInstance>	m_timeLineInstanceList;

	friend class Storyboard;
};
	
} // namespace GUI
} // namespace Lumino
