
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Thumb.h"
#include "Track.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{

/// Scroll �C�x���g�̌����ƂȂ��������\���܂��B
LN_ENUM(ScrollEventType)
{
	ThumbTrack = 0,		///< �܂݂��h���b�O����Ă���
	EndScroll,			///< �܂݂̃h���b�O���I������
	SmallDecrement,		///< �X�N���[���o�[�̃f�N�������g�{�^�����N���b�N���ꂽ
	SmallIncrement,		///< �X�N���[���o�[�̃C���N�������g�{�^�����N���b�N���ꂽ
	LargeDecrement,		///< �X�N���[���o�[�� PageUp �̈悪�N���b�N���ꂽ
	LargeIncrement,		///< �X�N���[���o�[�� PageDown �̈悪�N���b�N���ꂽ
};
LN_ENUM_DECLARE(ScrollEventType);

/**
	@brief		ScrollBar �̃X�N���[���C�x���g�̈����ł��B
*/
class ScrollEventArgs
	: public RoutedEventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	ScrollEventArgs(float newValue, ScrollEventType type) { NewValue = newValue; Type = type; }
	virtual ~ScrollEventArgs() {}

public:
	float			NewValue;		///< �V�����l
	ScrollEventType	Type;			///< �X�N���[���C�x���g�̌���
};

/**
	@brief
*/
class ScrollBar
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollBar);
public:
	static const String PART_TrackKeyName;

	LN_PROPERTY(float,					ValueProperty);
	LN_PROPERTY(Orientation,			OrientationProperty);

	LN_ROUTED_EVENT(ScrollEventArgs,	ScrollEvent);


	// TODO: RoutedCommand �� ID �ł͂Ȃ� Command �̃|�C���^�𒼐ڌ��J���������ǂ������B
	// button->SetCommand(Window.CloseCommand) �Ƃ��������B

public:
	static ScrollBar* Create(GUIManager* manager);

	ScrollBar(GUIManager* manager);
	virtual ~ScrollBar();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/// ScrollBar �̕������w�肵�܂��B
// TODO:	void SetOrientation(Orientation orientation) { m_orientation = orientation; OnPropertyChanged(OrientationProperty->GetName(), m_orientation); }

	/// ScrollBar �̕������擾���܂��B�K��l�� Orientation::Horizontal �ł��B
	Orientation GetOrientation() const { return m_orientation; }

	// TODO:����� RangeBase �Ɉڂ�����

	/// �X�N���[���ʒu�ɑ΂���l��ݒ肵�܂��B
	void SetValue(float value) { m_value = value; m_track->SetValue(value); }	// TODO: TemplateBinding �̂ق����悢���H

	/// �X�N���[���ʒu�ɑ΂���l���擾���܂��B�K��l�� 0 �ł��B
	float GetValue() const { return m_value; }

	/// �w��\�ȍŏ��l��ݒ肵�܂��B
	void SetMinimum(float value) { m_minimum = value; }

	/// �w��\�ȍŏ��l���擾���܂��B�K��l�� 0 �ł��B
	float GetMinimum() const { return m_minimum; }

	/// �w��\�ȍő�l��ݒ肵�܂��B
	void SetMaximum(float value) { m_maximum = value; }

	/// �w��\�ȍő�l���擾���܂��B�K��l�� 1 �ł��B
	float GetMaximum() const { return m_maximum; }

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */

	RoutedEventSlot<ScrollEventArgs>	Scroll;
	
	///// ScrollEvent �Ƀn���h����o�^���܂��B
	//void AddScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent += handler; }

	///// ScrollEvent �ɓo�^���ꂽ�n���h�����������܂��B
	//void RemoveScrollEventHandler(Delegate01<ScrollEventArgs*> handler) { m_scrollEvent -= handler; }

	/** @} */


	void SetTrackExtent(float value) { m_track->SetMaximum(value); }
	void SetTrackViewportSize(float value) { m_track->SetViewportSize(value); }




protected:
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual void OnScroll(ScrollEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }

private:
	void UpdateValue(float horizontalDragDelta, float verticalDragDelta);
	void ChangeValue(float newValue/*, bool defer*/);
	void Handler_Thumb_DragStarted(DragEventArgs* e);
	void Handler_Thumb_DragDelta(DragEventArgs* e);

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	//Event01<ScrollEventArgs*>	m_scrollEvent;

	Track*				m_track;	///< VisualTree ���� Track

	float				m_dragStartValue;	///< �h���b�O�J�n���� m_value
	bool				m_isStandalone;		///< Value �̑���� this �ōs�����AScrollViewer �o�R�ōs�����B

	friend class ScrollViewer;
};


} // namespace GUI
} // namespace Lumino
