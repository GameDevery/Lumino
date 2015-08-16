
#pragma once
#include "Common.h"
#include "../Animation/AnimationUtilities.h"
#include "../Variant.h"
#include "AnimationClock.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
	<FloatTimeline From="0" To="60" Duration="500"	���Ԃ̓~���b
		TargetName="textBlock"
		TargetProperty="FontSize" />
*/
class FloatTimeline		// TODO: ����Ȃ�����
	: public CoreObject
{
public:
	FloatTimeline();
	virtual ~FloatTimeline();

protected:
	//Animation::FloatAnimationCurve	m_curve;
};

class FloatEasing
	: public AnimationTimeline
{
public:
	FloatEasing();
	virtual ~FloatEasing();

	//void SetTargetName(const String& name) { m_targetName = name; }
	//void SetTargetProperty(const Property* prop) { m_targetProperty = prop; }
	void SetTargetValue(float value) { m_targetValue = value; }
	void SetEasingMode(Animation::EasingMode easingMode);
	void SetDuration(float duration) { m_duration = duration; }

protected:
	virtual bool Apply(UIElement* targetElement, Property* targetProp, const Variant& startValue, float time);

private:
	//String					m_targetName;
	//const Property*			m_targetProperty;
	float					m_targetValue;
	Animation::EasingMode	m_easingMode;
	//float					m_duration;
	AnimationUtilities::FloatEasingFunction	m_easingFunction;

};

/**
	@brief		
*/
class Storyboard
	: public CoreObject
{
public:
	Storyboard(GUIManager* manager);
	virtual ~Storyboard();

	void AddTimeline(AnimationTimeline* timeline);

	//Array< RefPtr<AnimationTimeline*> > GetChildren();
	
	/**
		@brief	�w�肵���v�f�ɃA�j���[�V������K�p���ĊJ�n���܂��B
	*/
	void Begin(UIElement* target);

	/**
		@brief	�w�肵���v�f�Ɋ��蓖�Ă��Ă���A���� Storyboard �p�ɍ쐬���ꂽ AnimationClock ���~���܂��B
	*/
	void Stop(UIElement* target);

private:
	GUIManager*	m_manager;
	Array< RefPtr<AnimationTimeline> >	m_animationTimelineList;
};

/**
	@brief		
*/
class VisualState
	: public CoreObject
{
	//[RuntimeNamePropertyAttribute("Name")]
	//[ContentPropertyAttribute("Storyboard")]
public:
	VisualState(GUIManager* manager);
	VisualState(GUIManager* manager, const String& name);
	virtual ~VisualState();

	const String& GetName() const { return m_name; }
	Storyboard* GetStoryboard() const { return m_storyboard; }

protected:
	GUIManager* m_manager;
	String	m_name;
	RefPtr<Storyboard>	m_storyboard;
};

typedef GenericVariantList<VisualState*>	VisualStateList;
	
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
	VisualStateGroup(const String& groupName);
	virtual ~VisualStateGroup();

	/// ���̃O���[�v���Ō��݃A�N�e�B�u�ł���A�R���g���[���ɓK�p����Ă��� VisualState ���擾���܂��B
	/// (��~����Ƃ��Ɏg�p����)
	VisualState* GetCurrentState() const { return m_currentState; }

	VisualStateList* GetVisualStateList() { return &m_status; }

	void AddState(VisualState* state) { m_status.Add(state); }

private:
	void SetCurrentState(VisualState* state) { m_currentState = state; }

private:
	String			m_name;
	VisualStateList	m_status;	// TODO: * �ɁB
	VisualState*	m_currentState;
	friend class VisualStateManager;
};

typedef GenericVariantList<VisualStateGroup*>	VisualStateGroupList;
	
/**
	@brief		
	@note		�EVisualStateGroups �͕����� UIElement ���狤�L�����B�� �Ƃ������AStyle �����L�����B
				�EWPF �� VisualStateManager.GotoState() �� static ���\�b�h�B
*/
class VisualStateManager
	: public CoreObject
{
public:
	VisualStateManager();
	virtual ~VisualStateManager();
	
	/// UI�v�f�̏�Ԃ�؂�ւ��܂��B
	static void GoToState(Control* control, const String& stateName);

protected:

};



class VisualStatus
{
public:
	static const String	Normal;		/* "Normal" ��Ԃ̖��O�ł��B*/
	static const String	MouseOver;	/* "MouseOver" ��Ԃ̖��O�ł��B*/

};
	

} // namespace GUI
} // namespace Lumino
