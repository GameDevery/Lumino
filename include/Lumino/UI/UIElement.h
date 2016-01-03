
#pragma once
#include "Common.h"
#include "UITypeInfo.h"
#include "UIEvent.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	
*/
class UIElement
	: public tr::ReflectionObject
{
	LN_UI_TYPEINFO_DECLARE();

public:
	LN_ROUTED_EVENT(MouseEventArgs,		MouseEnterEvent);				/**< MouseEnter ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseLeaveEvent);				/**< MouseLeave ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseMoveEvent);				/**< MouseMove ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseDownEvent);				/**< MouseDown ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseUpEvent);					/**< MouseUp ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		KeyDownEvent);					/**< KeyDown ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		KeyUpEvent);					/**< KeyUp ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(KeyEventArgs,		TextInputEvent);				/**< TextInput ���[�e�B���O�C�x���g�̎��ʎq */

	//-------------------------------------------------------------------------
	/** @name RoutedEvents */
	/** @{ */

	UIEvent<UIMouseEventArgs>		MouseEnter;		/**< �}�E�X�|�C���^�����̗v�f�̋��E���ɓ������Ƃ��ɔ������܂��B*/
	UIEvent<UIMouseEventArgs>		MouseLeave;		/**< �}�E�X�|�C���^�����̗v�f�̋��E����o���Ƃ��ɔ������܂��B*/
	UIEvent<UIMouseEventArgs>		MouseMove;		/**< �}�E�X�|�C���^�����̗v�f��ňړ�����Ɣ������܂��B */
	UIEvent<UIMouseEventArgs>		MouseDown;		/**< �}�E�X�|�C���^�����̗v�f��ɂ���Ƃ��ɔC�ӂ̃}�E�X�{�^�����������Ɣ������܂��B*/
	UIEvent<UIMouseEventArgs>		MouseUp;		/**< �}�E�X�|�C���^�����̗v�f��ɂ���Ƃ��ɔC�ӂ̃}�E�X�{�^�����������Ɣ������܂��B*/
	UIEvent<UIKeyEventArgs>			KeyDown;		/**< ���̗v�f���t�H�[�J�X�������Ă��鎞�ɃL�[�{�[�h�̃L�[���������Ɣ������܂��B*/
	UIEvent<UIKeyEventArgs>			KeyUp;			/**< ���̗v�f���t�H�[�J�X�������Ă��鎞�ɃL�[�{�[�h�̃L�[���������Ɣ������܂��B*/
	UIEvent<UIKeyEventArgs>			TextInput;		/**< �L�[����ɂ�蕶�������͂����Ɣ������܂��B*/

	/** @} */

public:

	/** �v�f�̎��ʖ����擾���܂��B*/
	const String& GetKeyName() const { return m_keyName; }

	/** �_����̐e�v�f���擾���܂��B*/
	UIElement* GetParent() const { return m_parent; }

	/** ���̗v�f���t�H�[�J�X�𓾂邱�Ƃ��ł��邩���m�F���܂��B*/
	virtual bool IsFocusable() const { return false; }

	/** ���C�A�E�g�����̑���p�X�̎��s���ɂ��̗v�f���v�Z�����T�C�Y���擾���܂��B���̒l�͎q�v�f���e�v�f�֗v������A�q�v�f���g�̍Œ�T�C�Y�ł��B*/
	const SizeF& GetDesiredSize() const { return m_desiredSize; }

	/** ���̗v�f�̍ŏI�I�ȕ`��T�C�Y���擾���܂��B���̒l�� Arrange() �Ŋm�肵�܂��B*/
	const SizeF& GetRenderSize() const { return m_finalLocalRect.GetSize(); }

	/** ���̗v�f�ւ̃t�H�[�J�X�̎擾�����݂܂��B*/
	void Focus();

	/** �}�E�X�L���v�`����ݒ肵�܂��B*/
	void CaptureMouse();

	/** �}�E�X�L���v�`�����������܂��B*/
	void ReleaseMouseCapture();

	/** ���̗v�f���̎q�r�W���A���v�f�̐����擾���܂��B*/
	virtual int GetVisualChildrenCount() const;

	/** ���̗v�f���̎w�肵���C���f�b�N�X�ɂ���q�r�W���A���v�f���擾���܂��B*/
	virtual UIElement* GetVisualChild(int index) const;

	/** Z�I�[�_�[��A�N�e�B�u��Ԃ��l���������ŁA�q�r�W���A���v�f���擾���܂��B���ɂ���v�f����A��O�ɂ���v�f����ɂȂ�܂��B*/
	virtual UIElement* GetVisualChildOrderd(int index) const;

	/** ���̗v�f���֘A�t�����Ă��� UILayoutView ���擾���܂��B*/
	UILayoutView* GetLayoutView() const;

	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	void RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e);
	
protected:
	UIElement();
	virtual ~UIElement();

	/** �w�肵���v�f�����̗v�f�Ƀr�W���A���q�v�f�Ƃ��Ēǉ����܂��B*/
	//void AddVisualChild(UIElement* element);

	/** �w�肵���v�f�����̗v�f�̃r�W���A���c���[����폜���܂��B*/
	//void RemoveVisualChild(UIElement* element);

	/** �t�H���g�ɂ�郌�C�A�E�g���𖳌������܂��B���̃��C�A�E�g�X�V�p�X�ŁA�t�H���g��񂪍č\�z����܂��B*/
	void InvalidateFont() { m_invalidateFlags |= detail::InvalidateFlags::Font; }
	
	/**
		@brief		���̗v�f��\�����邽�߂ɕK�v�ȃT�C�Y���v�����܂��B
		@params[in]	constraint	: ���̗v�f��z�u�ł���̈�̍ő�T�C�Y�B�ʏ�͐e�v�f�̃T�C�Y���n����܂��B
		@return		���̗v�f�̃��C�A�E�g�̍ۂɕK�v�ƂȂ�Œ���̃T�C�Y�B���̗v�f�̃T�C�Y�ƁA�S�Ă̎q�v�f�̃T�C�Y�Ɋ�Â����肵�܂��BInf �ł����Ă͂Ȃ�܂���B
		@details	constraint �́AScrollViewer ���̃R���e���c�ƂȂ����ꍇ�� Infinity ���n����邱�Ƃ�����܂��B
	*/
	virtual SizeF MeasureOverride(const SizeF& constraint);

	/**
		@brief		�q�v�f�̔z�u���m�肵�A���̗v�f�̍ŏI�T�C�Y��Ԃ��܂��B
		@param[in]	finalSize	: �e�v�f�����̗v�f�ɑ΂��Ċ��蓖�Ă��̈�̃T�C�Y�B
		@return		�v�f�̍ŏI�T�C�Y�B�v�f�̕`�掞�ɂ��̃T�C�Y���g�p���܂��B
		@details	�h���N���X�� finalSize �����傫���T�C�Y��Ԃ��ƁA�`�掞�Ɍ��؂ꂪ�������܂��B
	*/
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

	/** ���̗v�f�̃��C�A�E�g�̍X�V�������������ɌĂяo����܂��B*/
	virtual void OnLayoutUpdated();

	/**
		@brief	���̗v�f�̕`����s���܂��B
	*/
	//virtual void OnRender(RenderingContext* painter);

	virtual void OnMouseMove(UIMouseEventArgs* e);
	virtual void OnMouseDown(UIMouseEventArgs* e);
	virtual void OnMouseUp(UIMouseEventArgs* e);
	virtual void OnMouseEnter(UIMouseEventArgs* e);
	virtual void OnMouseLeave(UIMouseEventArgs* e);
	virtual void OnKeyDown(UIKeyEventArgs* e);
	virtual void OnKeyUp(UIKeyEventArgs* e);
	virtual void OnTextInput(UIKeyEventArgs* e);

LN_INTERNAL_ACCESS:
	UIElement* CheckMouseHoverElement(const PointF& globalPt);
	void ActivateInternal(UIElement* child);
	virtual bool OnEvent(detail::UIInternalEventType type, UIEventArgs* args);
	void ApplyTemplate();
	void ApplyTemplateHierarchy();
	void UpdateLayout();
	void UpdateTransformHierarchy();

private:
	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	void RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e);

	UILayoutView*			m_ownerLayoutView;
	String					m_keyName;
	UIElement*				m_parent;
	SizeF					m_desiredSize;			// MeasureLayout() �Ō��肳��邱�̃R���g���[���̗v���T�C�Y
	RectF					m_finalLocalRect;		// �`��Ɏg�p����ŏI���E��` (�O���[�o�����W�n=RootFrame �̃��[�J�����W�n)
	RectF					m_finalGlobalRect;

	// Property
	SizeF							m_size;
	ThicknessF						m_margin;
	float							m_opacity;
	//ToneF							m_tone;
	HorizontalAlignment				m_horizontalAlignment;
	VerticalAlignment				m_verticalAlignment;

	//RefPtr<Style>					m_style;
	float							m_combinedOpacity;
	//Array< RefPtr<AnimationClock> >	m_animationClockList;
	detail::InvalidateFlags			m_invalidateFlags;
	bool							m_isEnabled;
	bool							m_isMouseOver;
};

LN_NAMESPACE_END
