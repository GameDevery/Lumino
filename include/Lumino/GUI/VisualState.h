
#pragma once
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class VisualState
	: public CoreObject
{
	//[RuntimeNamePropertyAttribute("Name")]
	//[ContentPropertyAttribute("Storyboard")]
public:
	VisualState();
	VisualState(const String& name);
	virtual ~VisualState();

protected:
	String	m_name;
	Storyboard*	m_storyboard;
};
	
/**
	@brief		
	@details	VisualStateGroup �ɂ́A�����Ɏg�p�ł��Ȃ���Ԃ��܂߂܂��B ���Ƃ��΁ACheckBox �ɂ� 2 �� VisualStateGroup �I�u�W�F�N�g������܂��B
				����̃I�u�W�F�N�g�ɂ́ANormal�AMouseOver�APressed�A����� Disabled �̏�Ԃ��i�[����܂��B 
				��������̃I�u�W�F�N�g�ɂ́AChecked�AUnChecked�A����� Indeterminate �̏�Ԃ��i�[����܂��B 
				CheckBox �͓����� MouseOver �̏�Ԃ� UnChecked �̏�ԂɂȂ邱�Ƃ��ł��܂����A
				������ MouseOver �̏�Ԃ� Pressed �̏�ԂɂȂ邱�Ƃ͂ł��܂���B
				https://msdn.microsoft.com/ja-jp/library/system.windows.visualstategroup%28v=vs.110%29.aspx
*/
class VisualStateGroup
	: public CoreObject
{
public:
	VisualStateGroup();
	VisualStateGroup(const String& name);
	virtual ~VisualStateGroup();
	
	/// ���̃O���[�v���Ō��݃A�N�e�B�u�ł���A�R���g���[���ɓK�p����Ă��� VisualState ���擾���܂��B
	VisualState* GetCurrentState();

private:
	String	m_name;
};
	
/**
	@brief		
	@note		�EVisualStateManager �͕����� UIElement ���狤�L�����B
				�EWPF �� VisualStateManager.GotoState() �� static ���\�b�h�B
*/
class VisualStateManager
	: public CoreObject
{
public:
	VisualStateManager();
	virtual ~VisualStateManager();
	
	/// UI�v�f�̏�Ԃ�؂�ւ��܂��B
	void GoToState(UIElement* element, const String& stateName);

protected:

};


/**
	@brief		
	@note		1�̃^�[�Q�b�g�v���p�e�B��1�� AnimationTimeline ��K�p����B
*/
class AnimationClock
	: public CoreObject
{
public:
	AnimationClock(UIElement* targetElement, const String& targetPropertyName);
	virtual ~AnimationClock();
	
	void AdvanceTime(double elapsedTime);

protected:
};

/**
	@brief		
	@note		
*/
class AnimationTimeline
	: public CoreObject
{
public:
	AnimationTimeline();
	AnimationTimeline(const String& targetName);
	virtual ~AnimationTimeline();

	/// �A�j���[�V����������I�u�W�F�N�g�̖��O (x;key �Ŏw�肳�ꂽ���O)
	void SetTargetName(const String& name) { m_targetName = name; }

	/// �A�j���[�V����������v���p�e�B�̖��O
	void SetTargetProperty(const String& name) { m_targetProperty = name; }	// WPF �ł͓Y�t�v���p�e�B�Ō^�� PropertyPath

protected:
	double	m_duration;			///< �Đ����� (�~���b)
	String	m_targetName;		///< �^�[�Q�b�g�� UI �v�f���B�r�W���A���c���[����Ώۗv�f����������Ƃ��Ɏg�p����B
	String	m_targetProperty;	///< �^�[�Q�b�g�v���p�e�B��
};
	
/**
	@brief		
	<FloatTimeline From="0" To="60" Duration="500"	���Ԃ̓~���b
		TargetName="textBlock"
		TargetProperty="FontSize" />
*/
class FloatTimeline
	: public CoreObject
{
public:
	FloatTimeline();
	virtual ~FloatTimeline();

protected:
	//Animation::FloatAnimationCurve	m_curve;
};

/**
	@brief		
*/
class Storyboard
	: public CoreObject
{
public:
	Storyboard();
	virtual ~Storyboard();

	void AddTimeline(AnimationTimeline* timeline);

	//ArrayList< RefPtr<AnimationTimeline*> > GetChildren();
	
	void Begin(UIElement* target);

private:
	ArrayList< RefPtr<AnimationTimeline> >	m_animationTimelineList;
};


	

} // namespace GUI
} // namespace Lumino
