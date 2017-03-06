
#pragma once
#include "UIElement.h"
#include "UIButton.h"

LN_NAMESPACE_BEGIN


/**
	@brief	�}�E�X�h���b�O���֌W����C�x���g�����ł��B
*/
class UIDragDeltaEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIDragDeltaEventArgs() = default;
	virtual ~UIDragDeltaEventArgs() = default;

public:
	float horizontalChange;		/**< �h���b�O�J�n�_����̐������� */
	float verticalChange;		/**< �h���b�O�J�n�_����̐������� */
};

/**
	@brief		
*/
class UIThumb
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragStartedEvent);		/**< �}�E�X�h���b�O���J�n�����Ƃ��ɔ�������C�x���g��\���܂��B*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragDeltaEvent);			/**< �}�E�X�h���b�O���ɔ�������C�x���g��\���܂��B*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCompletedEvent);		/**< �}�E�X�h���b�O�����������Ƃ��ɔ�������C�x���g��\���܂��B*/
    LN_ROUTED_EVENT2(UIDragDeltaEventArgs, DragCanceledEvent);		/**< �}�E�X�h���b�O�𒆒f�����Ƃ��ɔ�������C�x���g��\���܂��B*/

public:
	static RefPtr<UIThumb> Create();

LN_CONSTRUCT_ACCESS:
	UIThumb();
	virtual ~UIThumb();
	void Initialize(detail::UIManager* manager);

protected:
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
	virtual void OnDragStarted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragStartedEventId, this, e); } }
	virtual void OnDragDelta(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragDeltaEventId, this, e); } }
	virtual void OnDragCompleted(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCompletedEventId, this, e); } }
	virtual void OnDragCanceled(UIDragDeltaEventArgs* e) { if (!e->handled) { RaiseEvent(DragCanceledEventId, this, e); } }

private:
	PointF	m_lastScreenPosition;
	bool	m_isDragging;

	UIEvent<UIDragDeltaEventArgs>        DragStarted;
	UIEvent<UIDragDeltaEventArgs>        DragDelta;
	UIEvent<UIDragDeltaEventArgs>        DragCompleted;
	UIEvent<UIDragDeltaEventArgs>        DragCanceled;
};

/**
	@brief		
*/
class UITrack
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UITrack> Create();
public:

	/** Track �̕������w�肵�܂��B*/
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/** Track �̕������擾���܂��B�K��l�� Orientation::Horizontal �ł��B*/
	Orientation GetOrientation() const { return m_orientation; }

	/** �X�N���[���ʒu�ɑ΂���l��ݒ肵�܂��B*/
	void SetValue(float value) { m_value = value; }

	/** �X�N���[���ʒu�ɑ΂���l���擾���܂��B�K��l�� 0 �ł��B*/
	float GetValue() const { return m_value; }

	/** �w��\�ȍŏ��l��ݒ肵�܂��B*/
	void SetMinimum(float value) { m_minimum = value; }

	/** �w��\�ȍŏ��l���擾���܂��B�K��l�� 0 �ł��B*/
	float GetMinimum() const { return m_minimum; }

	/** �w��\�ȍő�l��ݒ肵�܂��B*/
	void SetMaximum(float value) { m_maximum = value; }

	/** �w��\�ȍő�l���擾���܂��B�K��l�� 1 �ł��B*/
	float GetMaximum() const { return m_maximum; }

	/** �X�N���[���\�ȃR���e���c�̒��ŕ\������镔���̃T�C�Y��ݒ肵�܂��B*/
	void SetViewportSize(float value) { m_viewportSize = value; }

	/** �X�N���[���\�ȃR���e���c�̒��ŕ\������镔���̃T�C�Y���擾���܂��B*/
	float GetViewportSize() const { return m_viewportSize; }

	/** ������l�ɕϊ����܂��B*/
	float ValueFromDistance(float horizontal, float vertical);

LN_CONSTRUCT_ACCESS:

	UITrack();
	virtual ~UITrack();
	void Initialize(detail::UIManager* manager);

protected:
	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
	void CoerceLength(float& componentLength, float trackLength);
	void CalcScrollBarComponentsSize(
		float trackLength,
		float viewportSize,
		float* outDecreaseButtonLength,
		float* outThumbLength,
		float* outIncreaseButtonLength);

	Orientation			m_orientation;
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	float				m_density;
	float				m_viewportSize;
	RefPtr<UIButton>	m_pageUpButton;
	RefPtr<UIThumb>		m_thumb;
	RefPtr<UIButton>	m_pageDownButton;

};

/** Scroll �C�x���g�̌����ƂȂ��������\���܂��B*/
enum class ScrollEventType
{
	ThumbTrack,			/** �܂݂��h���b�O����Ă��� */
	EndScroll,			/** �܂݂̃h���b�O���I������ */
	SmallDecrement,		/** �X�N���[���o�[�̃f�N�������g�{�^�����N���b�N���ꂽ */
	SmallIncrement,		/** �X�N���[���o�[�̃C���N�������g�{�^�����N���b�N���ꂽ */
	LargeDecrement,		/** �X�N���[���o�[�� PageUp �̈悪�N���b�N���ꂽ */
	LargeIncrement,		/** �X�N���[���o�[�� PageDown �̈悪�N���b�N���ꂽ */
};

/**
	@brief        ScrollBar �̃X�N���[���C�x���g�̈����ł��B
*/
class UIScrollEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIScrollEventArgs(float newValue_, ScrollEventType type_) { newValue = newValue_; type = type_; }
	virtual ~UIScrollEventArgs() = default;

public:
	float			newValue;	/** �V�����l */
	ScrollEventType	type;		/** �X�N���[���C�x���g�̌��� */
};

/**
	@brief		
*/
class UIScrollBar
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIScrollBar> Create();
	// TODO:����� RangeBase �Ɉڂ�����

	/** �X�N���[���ʒu�ɑ΂���l��ݒ肵�܂��B*/
	void SetValue(float value);

	/** �X�N���[���ʒu�ɑ΂���l���擾���܂��B�K��l�� 0 �ł��B*/
	float GetValue() const;

	/** �w��\�ȍŏ��l��ݒ肵�܂��B*/
	void SetMinimum(float value);

	/** �w��\�ȍŏ��l���擾���܂��B�K��l�� 0 �ł��B*/
	float GetMinimum() const;

	/** �w��\�ȍő�l��ݒ肵�܂��B*/
	void SetMaximum(float value);

	/** �w��\�ȍő�l���擾���܂��B�K��l�� 1 �ł��B*/
	float GetMaximum() const;

	/** �X�N���[���\�ȃR���e���c�̒��ŕ\������镔���̃T�C�Y��ݒ肵�܂��B*/
	void SetViewportSize(float value);

	/** �X�N���[���\�ȃR���e���c�̒��ŕ\������镔���̃T�C�Y���擾���܂��B*/
	float GetViewportSize() const;

LN_CONSTRUCT_ACCESS:
	UIScrollBar();
	virtual ~UIScrollBar();
	void Initialize(detail::UIManager* manager);

protected:
	// UIElement interface
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;

private:
	void UpdateValue(float horizontalDragDelta, float verticalDragDelta);

	RefPtr<UITrack>    m_track;
	float            m_dragStartValue;
};

/**
	@brief        ScrollViewer �R���g���[�����̃��C���̃X�N���[���\�̈��\���܂��B
*/
class IScrollInfo
{
public:

	/// ��������ɃX�N���[���ł��邩�ǂ����������l��ݒ肵�܂��B
	/// @note    �X�N���[���o�[��\�����Ȃ��ݒ�ɂȂ��Ă�����A�����������݂��Ȃ��ꍇ�� false ���Z�b�g�����B
	///            IScrollInfo �̎����N���X�ŕs�v�Ȍv�Z���s��Ȃ��Ȃǂ̂��߂ɎQ�Ƃ���B
	virtual void SetCanHorizontallyScroll(bool enabled) = 0;

	/// ��������ɃX�N���[���ł��邩�ǂ����������l���擾���܂��B
	virtual bool CanHorizontallyScroll() const = 0;

	/// ��������ɃX�N���[���ł��邩�ǂ����������l��ݒ肵�܂��B
	virtual void SetCanVerticallyScroll(bool enabled) = 0;

	/// ��������ɃX�N���[���ł��邩�ǂ����������l���擾���܂��B
	virtual bool CanVerticallyScroll() const = 0;

	/// �G�N�X�e���g (�R���e���c�S��) �̉������擾���܂��B
	virtual float GetExtentWidth() const = 0;

	/// �G�N�X�e���g (�R���e���c�S��) �̏c�����擾���܂��B
	virtual float GetExtentHeight() const = 0;

	/// �R���e���c�ɑ΂�����ۂ̕\���̈�̉������擾���܂��B
	virtual float GetViewportWidth() const = 0;

	/// �R���e���c�ɑ΂�����ۂ̕\���̈�̏c�����擾���܂��B
	virtual float GetViewportHeight() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g��ݒ肵�܂��B
	virtual void SetHorizontalOffset(float offset) = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g���擾���܂��B
	virtual float GetHorizontalOffset() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g��ݒ肵�܂��B
	virtual void SetVerticalOffset(float offset) = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g���擾���܂��B
	virtual float GetVerticalOffset() const = 0;

	/// �X�N���[������𐧌䂷�� ScrollViewer �v�f��ݒ肵�܂��B
	//virtual void SetScrollOwner(ScrollViewer* owner) = 0;

	/// �R���e���c���� 1 �_���P�ʂ���ɃX�N���[�����܂��B
	virtual void LineUp() = 0;

	/// �R���e���c���� 1 �_���P�ʂ����ɃX�N���[�����܂��B
	virtual void LineDown() = 0;

	/// �R���e���c���� 1 �_���P�ʂ����ɃX�N���[�����܂��B
	virtual void LineLeft() = 0;

	/// �R���e���c���� 1 �_���P�ʂ��E�ɃX�N���[�����܂��B
	virtual void LineRight() = 0;

#if 0
	/// �R���e���c���� 1 �y�[�W����ɃX�N���[�����܂��B
	virtual void PageUp() = 0;

	/// �R���e���c���� 1 �y�[�W�����ɃX�N���[�����܂��B
	virtual void PageDown() = 0;

	/// �R���e���c���� 1 �y�[�W�����ɃX�N���[�����܂��B
	virtual void PageLeft() = 0;

	/// �R���e���c���� 1 �y�[�W���E�ɃX�N���[�����܂��B
	virtual void PageRight() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A��ɃX�N���[�����܂��B
	virtual void MouseWheelUp() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A���ɃX�N���[�����܂��B
	virtual void MouseWheelDown() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A���ɃX�N���[�����܂��B
	virtual void MouseWheelLeft() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A�E�ɃX�N���[�����܂��B
	virtual void MouseWheelRight() = 0;
#endif

	//Rect MakeVisible(UIElement visual, Rect rectangle);
};

/**
	@brief
*/
class UIScrollViewer
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIScrollViewer> Create();

LN_CONSTRUCT_ACCESS:
	UIScrollViewer();
	virtual ~UIScrollViewer();
	void Initialize(detail::UIManager* manager);

protected:
	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;

private:
	RefPtr<UIScrollBar>        m_verticalScrollBar;
	RefPtr<UIScrollBar>        m_horizontalScrollBar;
};


LN_NAMESPACE_END
