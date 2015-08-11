
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/Property.h>
#include "Common.h"
#include "../RoutedEvent.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "ControlTemplate.h"

#define LN_UI_ELEMENT_SUBCLASS_DECL(className) \
public: \
	static const String TypeID; \
	virtual const String& GetTypeID() const { return TypeID; } \
private: \
	friend class GUIManager;\
	friend class GUIHelper;\
	static className* internalCreateInstance(GUIManager* manager) \
	{ \
		auto obj = RefPtr<className>(LN_NEW className(manager)); \
		obj->InitializeComponent(); \
		obj.SafeAddRef(); \
		return obj; \
	}


#define LN_UI_ELEMENT_SUBCLASS_IMPL(className) \
	const String className::TypeID(_T(#className))

namespace Lumino
{
namespace GUI
{
class CanExecuteRoutedCommandEventArgs;
class ExecuteRoutedCommandEventArgs;
class RoutedCommandTypeContext;


/**
	@brief		
*/
class RoutedEventSlotBase
{
protected:
	RoutedEventSlotBase() {}
	virtual ~RoutedEventSlotBase() {}

private:
	friend class UIElement;
	virtual void Raise(RoutedEventArgs* e) = 0;
};


/**
	@brief		
	@details	RoutedEvent �� UIElement �y�т��̃T�u�N���X��������̂ݔ��������邱�Ƃ��o���܂��B
*/
template<class TArgs>
class RoutedEventSlot
	: public RoutedEventSlotBase
{
public:
	RoutedEventSlot() {}
	virtual ~RoutedEventSlot() {}

public:

	/**
		@brief	���[�e�B���O�C�x���g�̃n���h����ǉ����܂��B
	*/
	void AddHandler(const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	�w�肵���n���h���Ɉ�v����n���h�����A���̃X���b�g����폜���܂��B
	*/
	void RemoveHandler(const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Remove(handler);
	}
	
	/**
		@brief	���[�e�B���O�C�x���g�̃n���h����ǉ����܂��B
	*/
	void operator += (const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	�w�肵���n���h���Ɉ�v����n���h�����A���̃X���b�g����폜���܂��B
	*/
	void operator -= (const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Remove(handler);
	}

private:
	Array< Delegate01<TArgs*> > m_handlerList;

	virtual void Raise(RoutedEventArgs* e)
	{
		for (Delegate01<TArgs*>& d : m_handlerList)
		{
			d.Call(static_cast<TArgs*>(e));
		}
	}

	// Lumino::EventXX �̂悤�Ȏg�����Ɠ����ɁA
	// Property �ɂ������� �����o�ϐ��̂悤�ȈӖ������B
	// �����A�n���h���̃��X�g�B
	// RoutedEventSlot �́A������u���I�ȃn���h���v
};




/**
	@brief	GUI �v�f�̃x�[�X�N���X�ł��B
*/
class UIElement
	: public CoreObject
	, public IAddChild
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	LN_PROPERTY(SizeF,					SizeProperty);					/**< Size �v���p�e�B�̎��ʎq */
	LN_PROPERTY(ThicknessF,				MarginProperty);				/**< Margin �v���p�e�B�̎��ʎq */
	LN_PROPERTY(HorizontalAlignment,	HorizontalAlignmentProperty);	/**< HorizontalAlignment �v���p�e�B�̎��ʎq */
	LN_PROPERTY(VerticalAlignment,		VerticalAlignmentProperty);		/**< VerticalAlignment �v���p�e�B�̎��ʎq */
	LN_PROPERTY(bool,					IsHitTestProperty);				/**< IsHitTest �v���p�e�B�̎��ʎq */

	LN_ROUTED_EVENT(MouseEventArgs,		MouseEnterEvent);				/**< MouseEnter ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseLeaveEvent);				/**< MouseLeave ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseMoveEvent);				/**< MouseMove ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseDownEvent);				/**< MouseDown ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseUpEvent);					/**< MouseUp ���[�e�B���O�C�x���g�̎��ʎq */
	LN_ROUTED_EVENT(RoutedEventArgs,	CanExecuteRoutedCommandEvent);	/**< ���̃C�x���g�͓����p�ł��邽�߁A���[�U�[�̓n���h�����邱�Ƃ͂ł��Ȃ� */
	LN_ROUTED_EVENT(RoutedEventArgs,	ExecuteRoutedCommandEvent);		/**< ���̃C�x���g�͓����p�ł��邽�߁A���[�U�[�̓n���h�����邱�Ƃ͂ł��Ȃ� */

public:
	//-------------------------------------------------------------------------
	/** @name RoutedEvent */
	/** @{ */

	RoutedEventSlot<MouseEventArgs>		MouseEnter;		/**< �}�E�X�|�C���^�����̗v�f�̋��E���ɓ������Ƃ��ɔ������܂��B*/
	RoutedEventSlot<MouseEventArgs>		MouseLeave;		/**< �}�E�X�|�C���^�����̗v�f�̋��E����o���Ƃ��ɔ������܂��B*/
	RoutedEventSlot<MouseEventArgs>		MouseMove;		/**< �}�E�X�|�C���^�����̗v�f��ňړ�����Ɣ������܂��B */
	RoutedEventSlot<MouseEventArgs>		MouseDown;		/**< �}�E�X�|�C���^�����̗v�f��ɂ���Ƃ��ɔC�ӂ̃}�E�X�{�^�����������Ɣ������܂��B*/
	RoutedEventSlot<MouseEventArgs>		MouseUp;		/**< �}�E�X�|�C���^�����̗v�f��ɂ���Ƃ��ɔC�ӂ̃}�E�X�{�^�����������Ɣ������܂��B*/

	/** @} */
	//-------------------------------------------------------------------------
	/** @name Property */
	/** @{ */

	/** �v�f�̃T�C�Y��ݒ肵�܂��B�K��l�� NaN �ŁA���̒l�͎����T�C�Y�ύX���L���ł��邱�Ƃ������܂��B*/
	//void SetSize(const SizeF& size) { SetTypedPropertyValue(SizeProperty, size); }

	/** �v�f�̃T�C�Y���擾���܂��B*/
	//const SizeF& GetSize() const { return GetTypedPropertyValue<const SizeF&>(SizeProperty); }

	/** �v�f�̊O���̗]����ݒ肵�܂��B*/
	//void SetMargin(const ThicknessF& margin) { SetTypedPropertyValue(MarginProperty, margin); }

	/** �v�f�̊O���̗]�����擾���܂��B*/
	//const ThicknessF& GetMargin() const { return GetTypedPropertyValue<const ThicknessF&>(MarginProperty); }

	/** @} */

public:
	/** �_����̐e�v�f���擾���܂��B*/
	UIElement* GetParent() const { return m_parent; }

	/** �v�f�̎��ʖ����擾���܂��B*/
	const String& GetKeyName() const { return m_keyName; }

	/** ���C�A�E�g�����̑���p�X�̎��s���ɂ��̗v�f���v�Z�����T�C�Y���擾���܂��B���̒l�͎q�v�f���e�v�f�֗v������A�q�v�f���g�̍Œ�T�C�Y�ł��B*/
	const SizeF& GetDesiredSize() const { return m_desiredSize; }

	/** �}�E�X�L���v�`����ݒ肵�܂��B*/
	void CaptureMouse();

	/** �}�E�X�L���v�`�����������܂��B*/
	void ReleaseMouseCapture();

	/** ���̗v�f���̎q�r�W���A���v�f�̐����擾���܂��B*/
	virtual int GetVisualChildrenCount() const;

	/** ���̗v�f���̎w�肵���C���f�b�N�X�ɂ���q�r�W���A���v�f���擾���܂��B*/
	virtual UIElement* GetVisualChild(int index) const;

	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

protected:
	/** ���̗v�f�̃��C�A�E�g�̍X�V�������������ɌĂяo����܂��B*/
	virtual void OnLayoutUpdated();

	/** �w�肵���v�f�����̗v�f�Ƀr�W���A���q�v�f�Ƃ��Ēǉ����܂��B*/
	void AddVisualChild(UIElement* element);

	/** �w�肵���v�f�����̗v�f�̃r�W���A���c���[����폜���܂��B*/
	void RemoveVisualChild(UIElement* element);
	
	/**
		@brief		���̗v�f��\�����邽�߂ɕK�v�ȃT�C�Y���v�����܂��B
		@params[in]	constraint	: ���̗v�f��z�u�ł���̈�̍ő�T�C�Y�B�ʏ�͐e�v�f�̃T�C�Y���n����܂��B
		@return		���̗v�f�̃��C�A�E�g�̍ۂɕK�v�ƂȂ�Œ���̃T�C�Y�B���̗v�f�̃T�C�Y�ƁA�S�Ă̎q�v�f�̃T�C�Y�Ɋ�Â����肵�܂��B
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

protected:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

private:
	RoutedEventSlot<RoutedEventArgs>	CanExecuteRoutedCommand;
	RoutedEventSlot<RoutedEventArgs>	ExecuteRoutedCommand;

	ThicknessF		m_margin;























public:


	// Property

	//
	//void SetWidth(float width) { m_size.Width = width; }

	///// �v�f�̕����擾���܂��B
	//float GetWidth() { return m_size.Width; }

	///// �v�f�̍�����ݒ肵�܂��B�K��l�� NAN �ŁA�����I�ɃT�C�Y���v�Z���܂��B
	//void SetHeight(float width) { m_size.Width = width; }

	//


	void SetHorizontalAlignment(HorizontalAlignment value) { m_horizontalAlignment = value; }
	HorizontalAlignment GetHorizontalAlignment() const { return m_horizontalAlignment; }
	void SetVerticalAlignment(VerticalAlignment value) { m_verticalAlignment = value; }
	VerticalAlignment GetVerticalAlignment() const { return m_verticalAlignment; }

	///// �q�b�g�e�X�g�̗L����ݒ肵�܂��B
	//void SetHitTest(bool enabled) { SetPropertyValue(IsHitTestProperty, Variant(enabled)); }

	///// �q�b�g�e�X�g�̗L�����擾���܂��B
	//bool IsHitTest() const { return GetPropertyValue(IsHitTestProperty).GetBool(); }






public:

	/// ���̗v�f�̃��C�A�E�g�̍X�V�������������ɔ������܂��B���̃C�x���g�̓��[�e�B���O�C�x���g�ł͂���܂���B
	//Event01<RoutedEventArgs*>			LayoutUpdated;

private:

public:

	// �C�x���g�̈������� WPF �Ƃ͏����Ⴄ�B
	// WPF �� ButtonBase.Click �́A
	// public event RoutedEventHandler Click { add { AddHandler(ClickEvent, value); } remove { RemoveHandler(ClickEvent, value); } }
	// �̂悤�ɂȂ��Ă���B
	// �܂�Aevent Click �͒P�Ȃ邨����BClick(...) �̂悤�ɁA���ꂪ���ڌĂ΂�邱�Ƃ͂Ȃ��B
	// RaiseEvent(ClickEvent) ����Ă΂��B
	// RaiseEvent(ClickEvent) �� ButtonBase.OnClick() �ŌĂ�ł���B
	

	//void AddMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { AddHandler(MouseMoveEvent, handler); }
	//void RemoveMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { RemoveHandler(MouseMoveEvent, handler); }

	// ������2�� C_API ����g�p����
	virtual bool AddHandlerInternal(const String& key, void* nativeCFuncPtr) { LN_THROW(0, ArgumentException); return false; }
	virtual bool RemoveHandlerInternal(const String& key, void* nativeCFuncPtr) { LN_THROW(0, ArgumentException); return false; }


	/// rootLogicalParent : �e���v���[�g��K�p�����v�f�BTemplateBinding �̃\�[�X�I�u�W�F�N�g�B
	void SetTemplateBinding(const Property* thisProp, const String& srcPropPath/*, UIElement* rootLogicalParent*/);

	/// (�T�C�Y�̎����v�Z���L���ɂȂ��Ă���v�f�ɑ΂��Ă͌Ăяo���Ă����ʂ͂���܂���)
	void UpdateLayout();
	void UpdateTransformHierarchy();
	virtual void Render();


	/// ���݂̃e���v���[�g����r�W���A���c���[���č\�z���܂��B
	/// ���̊֐��͕K�v�ȃ^�C�~���O�Ń��C�A�E�g�V�X�e������Ăяo����܂��B�ʏ�A�����I�ɌĂяo���K�v�͂���܂���B
	///		�Ƃ������A�Ăяo������_���B�K�����[�g����ċA�I�ɍX�V���Ȃ��ƁA�����e���܂� ApplyTemplate() ���ĂȂ���Ԃł�����ĂԂ�
	///		���[�J�����\�[�X���������X�V����Ȃ��B
	///		TODO: ������������ASetParent �����u�ԂɃ��[�J�����\�[�X���X�V�����ق����ǂ������H
	///		��������΂��� ApplyTemplate() ���Ăяo���Ă��ǂ����c ���v�͖������B
	void ApplyTemplate();

	/// internal  �ʃR���g���[���̎q�ɒǉ����ꂽ�Ƃ����� true ���Z�b�g�����B
	/// �p�t�H�[�}���X�I�ȗ��R�����邪�A�{�� ApplyTemplate ����ׂ��^�C�~���O�ł͐e�v�f�̃��[�J�����\�[�X���m�肵�Ă��Ȃ����Ƃ����邽�߁B
	/// true ���Z�b�g���ꂽ��A�e�֓`�d���Ă����B
	void SetChildTemplateModified(bool modified) { m_childTemplateModified = modified; if (modified && m_parent != NULL) { m_parent->SetChildTemplateModified(true); } }
	void SetTemplateModified(bool modified) { m_templateModified = modified; if (modified && m_parent != NULL) { m_parent->SetChildTemplateModified(true); } }

public:


	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	


	/// ���݂̃e���v���[�g����r�W���A���c���[���č\�z���ꂽ��ɌĂяo����܂��B
	/// �h���N���X�� localResource �ɑ΂��ăL�[�l���烊�\�[�X���擾���邱�Ƃ��ł��܂��B
	virtual void OnApplyTemplate(CombinedLocalResource* localResource) {}
	virtual void OnRender(Graphics::Painter* painter) {}
	virtual bool OnEvent(EventType type, RoutedEventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }	// �_���v�f�̒ǉ��B�I�[�o�[���C�h����B
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

public:	// internal
	void SetParent(UIElement* parent) { m_parent = parent; }
	//virtual void AddChildToVisualTree(UIElement* element) { LN_THROW(0, InvalidOperationException); }

	// �R���X�g���N�^�̌�Ăяo���B
	void InitializeComponent();

	/// 
	void UpdateTemplateLogicalParentHierarchy(UIElement* logicalParent);

protected:
	virtual const String& GetTypeID() const = 0;

	// override
	virtual void OnPropertyChanged(PropertyChangedEventArgs* e);

	//friend class Decorator;
	//friend class Control;
	//friend class ContentControl;
	//friend class Panel;
public:	// internal
	virtual void ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/);
	void UpdateTemplateHierarchy();	// ������� modified �}�[�N����Ă���v�f�̃e���v���[�g���X�V����


	GUIManager* GetManager() const { return m_manager; }

private:
	friend class UIElementFactory;
	friend class ControlTemplate;
	friend class DataTemplate;
	void SetTemplateChild(UIElement* child);
	//virtual void AddVisualChild(UIElement* child);	///< AddChild() �͘_���c���[�ւ̒ǉ��AAddVisualChild() �̓r�W���A���c���[�ւ̒ǉ��B

	void UpdateLocalResource();
protected:
	virtual void PollingTemplateChildCreated(UIElement* newElement) {}

protected:
	//void RegisterRoutedEvent(RoutedEvent* ev) {
	//	m_routedEventList.Add(ev->GetName(), ev);
	//}
	//void RegisterRoutedEventHandler(EventID id, RoutedEventHandler* handler) {
	//	m_routedEventHandlerList.Add(id, handler);
	//}
	void AddRoutedCommandTypeContext(RoutedCommandTypeContext* c) {
		if (!m_routedCommandTypeContextList.Contains(c)) {	// �������͓̂o�^���Ȃ�
			m_routedCommandTypeContextList.Add(c);
		}
	}

public:
	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	void RaiseEvent(const RoutedEvent* ev, CoreObject* sender, RoutedEventArgs* e)
	{
		e->Sender = sender;
		RaiseEventInternal(ev, e);
	}

protected:
	// �w�肵���C�x���g�X���b�g�փC�x���g�𒼐ڔ��s����B���[�e�B���O�͍s��Ȃ�
	void EmitEventSlot(RoutedEventSlotBase& slot, RoutedEventArgs* e)
	{
		LN_VERIFY_RETURN(e != NULL);
		slot.Raise(e);
	}

private:
	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	void RaiseEventInternal(const RoutedEvent* ev, RoutedEventArgs* e)
	{
		CoreObject::RaiseEventInternal(ev, e);

		// bubble
		if (!e->Handled && m_parent != NULL)
		{
			m_parent->RaiseEventInternal(ev, e);
		}
	}


protected:
	// �T�u�N���X�͂ł��邾�� Event �Ƀn���h����o�^����̂ł͂Ȃ��AOn�`���I�[�o�[���C�h���邱�ƂŃC�x���g���n���h�����O����B
	// �n���h�����O������ e->Handled �� true �ɂ���B������ super ���Ăяo���B�������邱�ƂŁARaiseEvent() �ł̃C�x���g������
	// ���[�e�B���O���s��Ȃ��̂ŕ��׌y�����ł���B
	virtual void OnMouseMove(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }
	virtual void OnMouseDown(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseDownEvent, this, e); } }
	virtual void OnMouseUp(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseUpEvent, this, e); } }


	friend class GUIHelper;
	GUIManager*				m_manager;
	String					m_keyName;
	UIElement*				m_parent;				///< �e�v�f (�_���E�r�W���A���͊֌W�Ȃ��BRoutedEvent(Bubble) �̒ʒm��ƂȂ�)

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() �Ō��肳��邱�̃R���g���[���̍ŏI�v���T�C�Y
	RectF					m_finalLocalRect;			///< �`��Ɏg�p����ŏI���E��` (�O���[�o�����W�n=RootFrame �̃��[�J�����W�n)
	RectF					m_finalGlobalRect;


	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;

	// �폜�\��
	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;

	/// ����N���X�Ɋ܂܂�� RoutedCommand �̃��X�g�̃��X�g�B���̃��X�g�ɂ͊��N���X���珇�ɋl�܂��Ă���
	Array<RoutedCommandTypeContext*>	m_routedCommandTypeContextList;

	// Property
	SizeF				m_size;
	HorizontalAlignment	m_horizontalAlignment;
	VerticalAlignment	m_verticalAlignment;
	bool				m_isHitTest;


public:	// TODO: private �ɂ�����
	void Handler_CanExecuteRoutedCommandEvent(CanExecuteRoutedCommandEventArgs* e);
	void Handler_ExecuteRoutedCommandEvent(ExecuteRoutedCommandEventArgs* e);

private:
	void TemplateBindingSource_PropertyChanged(/*CoreObject* sender, */PropertyChangedEventArgs* e);


	struct TemplateBindingInfo
	{
		const Property*	ThisProp;
		String			SourcePropPath;
	};

	typedef Array<TemplateBindingInfo>	TemplateBindingInfoList;
	TemplateBindingInfoList		m_templateBindingInfoList;

	/// �e���v���[�g��K�p�����v�f�BTemplateBinding �̃\�[�X�I�u�W�F�N�g�B���ꂪ NULL �łȂ���΁Athis �� VisualTree �v�f�ł���B
	/// �Ⴆ�� Button �Ƀe���v���[�g��K�p����ƁAButton ��肵���̃r�W���A���v�f.m_rootLogicalParent �͂��ׂ� Button ���w���B
	UIElement*		m_rootLogicalParent;
	Delegate01<PropertyChangedEventArgs*>	m_templateBindingHandler;
	Event01<PropertyChangedEventArgs*>		PropertyChangedForTemplateBindings;

	bool			m_childTemplateModified;	///< �q�v�f�̃e���v���[�g���X�V����ׂ���
	bool			m_templateModified;

protected:
	RefPtr<Style>		m_style;
	RefPtr<UIElement>	m_templateChild;		///< TODO: Control �Ɉړ�����ׂ������B�e���v���[�g�q�v�f�̃��[�g�v�f�B�_���v�f���������B
	UIElement*			m_templateParent;		///< �e���v���[�g�̃��[�g�v�f�B�Ⴆ�� ScrollViewer - Grid - ScrollContentPresenter �Ƃ����K�w������ꍇ�AScrollContentPresenter.m_templateParent �� ScrollViewer ���w���B

private:
	// ���̃��X�g�ɒǉ����ꂽ UIElement �́AOnEvent�ARender�A�q�b�g�e�X�g�������̃N���X��莩���I�ɌĂ΂��B
	// �������A���C�A�E�g�֌W�̓m�[�^�b�`�BMeasure �� Arrange �͌Ă΂�Ȃ��B
	// �g�����Ƃ��ẮA�_���v�f���낤���r�W���A���v�f���낤���֌W�Ȃ��܂��͂��̃��X�g�ɒǉ����Ă����B
	// ���C�A�E�g�� (�����h��������) �X�̃R���e�i�v�f�Ŗʓ|������B
	// �������Am_templateChild �� ���̃N���X�ł��ʓ|�����Ă�����B�ł��A�Â������C�A�E�g���K�v�Ȏ���
	// �I�[�o�[���C�h����K�v������B
	Array<UIElement*>	m_visualChildren;
};

} // namespace GUI
} // namespace Lumino
