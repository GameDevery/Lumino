
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
	virtual ~VisualStateGroup();
	
	/// ���̃O���[�v���Ō��݃A�N�e�B�u�ł���A�R���g���[���ɓK�p����Ă��� VisualState ���擾���܂��B
	VisualState* GetCurrentState();

protected:

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
*/
class Storyboard
	: public CoreObject
{
public:
	Storyboard();
	virtual ~Storyboard();
	
	void Begin(UIElement* target);

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
	virtual ~AnimationTimeline();

protected:
	double	m_duration;		///< �Đ����� (�~���b)
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

	

} // namespace GUI
} // namespace Lumino
