
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/Property.h>
#include "Common.h"
#include "../RoutedEvent.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button
#include "ControlTemplate.h"

#define LN_UI_ELEMENT_SUBCLASS_DECL(subClassName) \
public: \
	static const String TypeID; \
	static CoreObject* CreateInstance(GUIManager* manager) { return LN_NEW subClassName(manager); } \
	virtual const String& GetTypeID() const { return TypeID; }

#define LN_UI_ELEMENT_SUBCLASS_IMPL(subClassName) \
	const String subClassName::TypeID(_T(#subClassName))

namespace Lumino
{
namespace GUI
{
//typedef String	PropertyID;
//typedef String	EventID;

class CanExecuteRoutedCommandEventArgs;
class ExecuteRoutedCommandEventArgs;
class RoutedCommandTypeContext;

LN_ENUM(HorizontalAlignment)
{
	Left = 0,
	Center,
	Right,
	Stretch,
};
LN_ENUM_DECLARE(HorizontalAlignment);

LN_ENUM(VerticalAlignment)
{
	Bottom = 0,		///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
	Center,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
	Stretch,		///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B
	Top,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
};
LN_ENUM_DECLARE(VerticalAlignment);
	
LN_ENUM(Orientation)
{
	Vertical = 0,
	Horizontal,
};
LN_ENUM_DECLARE(Orientation);



class IAddChild
{
public:
	virtual void AddChild(const Variant& value) = 0;
	virtual void AddText(const String& text) = 0;
};

/**
	@brief		
*/
class UIElement
	: public CoreObject
	, public IAddChild
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	static const Property*		SizeProperty;
	static const Property*		HorizontalAlignmentProperty;
	static const Property*		VerticalAlignmentProperty;
	static const Property*		IsHitTestProperty;

	static const RoutedEvent*	MouseMoveEvent;
	static const RoutedEvent*	MouseLeaveEvent;
	static const RoutedEvent*	MouseEnterEvent;
	static const RoutedEvent*	MouseDownEvent;
	static const RoutedEvent*	MouseUpEvent;

	static const RoutedEvent*	CanExecuteRoutedCommandEvent;	///< ���̃C�x���g�͓����p�ł��邽�߁A���[�U�[�̓n���h�����邱�Ƃ͂ł��Ȃ�
	static const RoutedEvent*	ExecuteRoutedCommandEvent;		///< ���̃C�x���g�͓����p�ł��邽�߁A���[�U�[�̓n���h�����邱�Ƃ͂ł��Ȃ�

public:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

	/// �_���c���[��̐e�v�f���擾���܂��B
	UIElement* GetParent() const { return m_parent; }

public:
	// Property

	//
	//void SetWidth(float width) { m_size.Width = width; }

	///// �v�f�̕����擾���܂��B
	//float GetWidth() { return m_size.Width; }

	///// �v�f�̍�����ݒ肵�܂��B�K��l�� NAN �ŁA�����I�ɃT�C�Y���v�Z���܂��B
	//void SetHeight(float width) { m_size.Width = width; }

	//

	/// �v�f�̃T�C�Y��ݒ肵�܂��B�K��l�� NAN �ŁA�����I�ɃT�C�Y���v�Z���܂��B
	void SetSize(const SizeF& size) { m_size = size; }// { SetValue(SizeProperty, size); }	// TODO: ���C�A�E�g�X�V���͕p�ɂɃA�N�Z�X����̂łł���� �����o�ϐ��ɂ������E�E�E
	
	/// �v�f�̃T�C�Y���擾���܂��B
	const SizeF& GetSize() const { return m_size; }// { return GetValue(SizeProperty).GetSizeF(); }	// TODO: ��Ȃ��B�Q�ƂŕԂ��ƃX�^�b�N�� Variant �ŏ�����


	void SetHorizontalAlignment(HorizontalAlignment value) { m_horizontalAlignment = value; }
	HorizontalAlignment GetHorizontalAlignment() const { return m_horizontalAlignment; }
	void SetVerticalAlignment(VerticalAlignment value) { m_verticalAlignment = value; }
	VerticalAlignment GetVerticalAlignment() const { return m_verticalAlignment; }

	/// �q�b�g�e�X�g�̗L����ݒ肵�܂��B
	void SetHitTest(bool enabled) { SetPropertyValue(IsHitTestProperty, Variant(enabled)); }

	/// �q�b�g�e�X�g�̗L�����擾���܂��B
	bool IsHitTest() const { return GetPropertyValue(IsHitTestProperty).GetBool(); }


	void CaptureMouse();
	void ReleaseMouseCapture();








	Event01<MouseEventArgs*>	MouseMove;
	Event01<MouseEventArgs*>	MouseEnter;
	Event01<MouseEventArgs*>	MouseLeave;
	Event01<MouseEventArgs*>	MouseDown;
	Event01<MouseEventArgs*>	MouseUp;

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
	const SizeF& GetDesiredSize() const {
		return m_desiredSize;
	}


	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize) { return finalSize; }

	/// ���݂̃e���v���[�g����r�W���A���c���[���č\�z���ꂽ��ɌĂяo����܂��B
	/// �h���N���X�� localResource �ɑ΂��ăL�[�l���烊�\�[�X���擾���邱�Ƃ��ł��܂��B
	virtual void OnApplyTemplate(CombinedLocalResource* localResource) {}
	virtual void OnRender() {}
	virtual bool OnEvent(EventType type, EventArgs* args);

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
	virtual void OnPropertyChanged(const String& name, const Variant& newValue);

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
	virtual void PollingTemplateChildCreated(UIElement* element) {}

private:
	// TypedRoutedEvent ����R�[���o�b�N�����BOn�`�Ƃ͕ʂɁA�P�ɃC�x���g�𔭐�������R�[���o�N�Ƃ��ĕK�v�B(TypedRoutedEvent �͏�Ԃ����ĂȂ��̂�)
	// �ǂ�Ȏ���g���Ă��A���� TypedRoutedEvent ���� Event02 ���Ăׂ��i���K�v�Ȃ̂ŁA�����͕K���K�v�ɂȂ�B
	//void CallMouseMoveEvent(CoreObject* sender, MouseEventArgs* e) { MouseMove.Raise(sender, e); }
	//void CallMouseEnterEvent(CoreObject* sender, MouseEventArgs* e) { MouseEnter.Raise(sender, e); }
	//void CallMouseLeaveEvent(CoreObject* sender, MouseEventArgs* e) { MouseLeave.Raise(sender, e); }
	//void CallMouseDownEvent(CoreObject* sender, MouseEventArgs* e) { MouseDown.Raise(sender, e); }
	//void CallMouseUpEvent(CoreObject* sender, MouseEventArgs* e) { MouseUp.Raise(sender, e); }

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
	void RaiseEvent(const RoutedEvent* ev, CoreObject* sender, EventArgs* e)
	{
		e->Sender = sender;
		RaiseEventInternal(ev, e);
	}

private:
	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	void RaiseEventInternal(const RoutedEvent* ev, EventArgs* e)
	{
		CoreObject::RaiseEventInternal(ev, e);

		// bubble
		if (!e->Handled && m_parent != NULL)
		{
			m_parent->RaiseEventInternal(ev, e);
		}
	}

	//bool RaiseCommand(RoutedCommand* command, const Variant& parameter, bool execute)
	//{
	//	if (execute)
	//	{
	//		for (auto context : m_commandOwnerClassContextList) {
	//			if (context->Execute(this, command, parameter)) { return true; }
	//		}
	//	}
	//	else
	//	{
	//		for (auto context : m_commandOwnerClassContextList) {
	//			bool r;
	//			if (context->CanExecute(this, command, parameter, &r)) { return r; }
	//		}
	//	}

	//}



protected:
	// �T�u�N���X�͂ł��邾�� Event �Ƀn���h����o�^����̂ł͂Ȃ��AOn�`���I�[�o�[���C�h���邱�ƂŃC�x���g���n���h�����O����B
	// �n���h�����O������ e->Handled �� true �ɂ���B������ super ���Ăяo���B�������邱�ƂŁARaiseEvent() �ł̃C�x���g������
	// ���[�e�B���O���s��Ȃ��̂ŕ��׌y�����ł���B
	virtual void OnMouseMove(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }
	virtual void OnMouseDown(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseDownEvent, this, e); } }
	virtual void OnMouseUp(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseUpEvent, this, e); } }



	GUIManager*				m_manager;
	UIElement*				m_parent;				///< �e�v�f (�_���E�r�W���A���͊֌W�Ȃ��BRoutedEvent(Bubble) �̒ʒm��ƂȂ�)

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() �Ō��肳��邱�̃R���g���[���̍ŏI�v���T�C�Y
	RectF					m_finalLocalRect;			///< �`��Ɏg�p����ŏI���E��` (�O���[�o�����W�n=RootPane �̃��[�J�����W�n)
	RectF					m_finalGlobalRect;


	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;

	// �폜�\��
	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;
	//Event02<CoreObject*, MouseEventArgs*> m_eventMouseMove;

	//typedef SortedArray<String, RoutedEvent*>	RoutedEventList;
	//RoutedEventList	m_routedEventList;

	//typedef SortedArray<EventID, RoutedEventHandler*>	RoutedEventHandlerList;
	//RoutedEventHandlerList	m_routedEventHandlerList;

	/// ����N���X�Ɋ܂܂�� RoutedCommand �̃��X�g�̃��X�g�B���̃��X�g�ɂ͊��N���X���珇�ɋl�܂��Ă���
	Array<RoutedCommandTypeContext*>	m_routedCommandTypeContextList;

	// Property
	SizeF				m_size;
	HorizontalAlignment	m_horizontalAlignment;
	VerticalAlignment	m_verticalAlignment;


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

	bool			m_childTemplateModified;	///< �q�v�f�̃e���v���[�g���X�V����ׂ���
	bool			m_templateModified;

protected:
	RefPtr<Style>		m_style;
	RefPtr<UIElement>	m_templateChild;
	UIElement*			m_templateParent;

	// ���̃��X�g�ɒǉ����ꂽ UIElement �́AOnEvent�ARender�A�q�b�g�e�X�g�������̃N���X��莩���I�ɌĂ΂��B
	// �������A���C�A�E�g�֌W�̓m�[�^�b�`�BMeasure �� Arrange �͌Ă΂�Ȃ��B
	// �g�����Ƃ��ẮA�_���v�f���낤���r�W���A���v�f���낤���֌W�Ȃ��܂��͂��̃��X�g�ɒǉ����Ă����B
	// ���C�A�E�g�� (�����h��������) �X�̃R���e�i�v�f�Ŗʓ|������B
	// �������Am_templateChild �� ���̃N���X�ł��ʓ|�����Ă�����B�ł��A�Â������C�A�E�g���K�v�Ȏ���
	// �I�[�o�[���C�h����K�v������B
	Array<UIElement*>	m_visualChildren;
};

/**
	@brief		1 �̎q�v�f�̎��͂𑕏����� UI �v�f�̊�{�N���X�ł��B
*/
class Decorator
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Decorator);
public:
	Decorator(GUIManager* manager);
	virtual ~Decorator();

public:
	void SetChild(UIElement* element);
	UIElement* GetChild() const;

public:
	//virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }
	//virtual void AddVisualChild(UIElement* child);


protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);;
	//virtual void AddChildToVisualTree(UIElement* element);

private:
	RefPtr<UIElement>	m_child;
	//RefPtr<UIElement>	m_visualChild;
};

/**
	@brief		Button �R���g���[���̊O�ςł��B
*/
class ButtonChrome
	: public Decorator
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ButtonChrome);
public:
	static const Property*	IsMouseOverProperty;	///< IsMouseOver �v���p�e�B�̎��ʎq
	static const Property*	FrameWidthProperty;		///< FrameWidth �v���p�e�B�̎��ʎq

public:
	static ButtonChrome* Create(GUIManager* manager);

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

public:
	// Property
	void SetMouseOver(bool value) { m_isMouseOver = value; }
	bool IsMouseOver() const { return m_isMouseOver; }

	/// �{�^���C���[�W�̊O�g�̕���ݒ肵�܂��B
	void SetFrameWidth(float width) { m_frameWidth = width; }

	/// �{�^���C���[�W�̊O�g�̕����擾���܂��B
	float GetFrameWidth() const { return m_frameWidth; }

public:
	// override CoreObject
	//virtual void SetPropertyValue(const Property* prop, const Variant& value);
	//virtual Variant GetPropertyValue(const Property* prop) const;

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;	///< �g�̕� (�`��Ŏg�p���邽�߃p�t�H�[�}���X���l�����AGetValue() �ł͂Ȃ������o�Ɏ����Ă���)
	bool							m_isMouseOver;
};


/**
	@brief	ContentControl �̃R���e���c��\�����܂��B
*/
class ContentPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentPresenter);
public:

public:
	static ContentPresenter* Create(GUIManager* manager);

	ContentPresenter(GUIManager* manager);
	virtual ~ContentPresenter();

	void SetContent(UIElement* content);

protected:
#if 1
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void Render();
#endif

private:
	RefPtr<UIElement>	m_content;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ContentPresenter �͘_���I�Ȏq�v�f�̔z�u����}�[�N���郁�^�f�[�^�̂悤�Ȃ��̂Ȃ̂ŁA�q�v�f�͎����Ȃ��B
};

/**
	@brief	ItemsControl �̃R���e���c��\�����܂��B
*/
class ItemsPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsPresenter);
public:

public:
	static ItemsPresenter* Create(GUIManager* manager);

	ItemsPresenter(GUIManager* manager);
	virtual ~ItemsPresenter();

protected:
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ItemsPresenter �͘_���I�Ȏq�v�f�̔z�u����}�[�N���郁�^�f�[�^�̂悤�Ȃ��̂Ȃ̂ŁA�q�v�f�͎����Ȃ��B
};

/**
	@brief		���[�U�[����ɂ���ē���̓�����s�� UI �v�f�̊�{�N���X�ł��B
*/
class Control
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Control);
//public:
//	static const String ControlTemplateTypeName;
	static const Property* TemplateProperty;

public:
	Control(GUIManager* manager);
	virtual ~Control();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	void SetTemplate(ControlTemplate* controlTemplate);
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }

	/** @} */

protected:
	// override UIElement
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	//friend class UIElementFactory;

	//friend class ControlTemplate;
	//friend class DataTemplate;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// Contro ���͎̂q�v�f�������Ȃ��B�T�u�N���X�ł���ɃI�[�o�[���C�h�����B

	//virtual void ApplyTemplate(CombinedLocalResource* parent);

//	virtual bool ApplyTemplateInternal();
//	bool ApplyTemplateInternalMain(const String& typeName);
//
//
//protected:
//	// override UIElement
//	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
private:
	RefPtr<ControlTemplate>	m_controlTemplate;
};

/**
	@brief		1 �̃R���e���c�����R���g���[����\���܂��B
*/
class ContentControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentControl);
public:
	ContentControl(GUIManager* manager);
	virtual ~ContentControl();


	/// ���̃R���g���[���̃R���e���c��ݒ肵�܂��B
	void SetContent(Variant value);

	//virtual void Render();

protected:
	//virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	////virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);
	//virtual void OnRender();
	//virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

	virtual void PollingTemplateChildCreated(UIElement* element);

protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
	ContentPresenter*	m_contentPresenter;
	Variant		m_content;		
	RefPtr<UIElement>	m_childElement;	///< m_content �� UIElement �ł���΂�����w��
};

/**
	@brief		���[�g�v�f�B
*/
class RootPane
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RootPane);
public:
	static RootPane* Create(GUIManager* manager);

	RootPane(GUIManager* manager);
	virtual ~RootPane();

	//virtual void Render();

protected:
	//virtual void OnRender() {}

	friend class GUIManager;

private:
};

/**
	@brief		
*/
class Button
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Button);
public:
	static const String	IsMouseOverProperty;	///< IsMouseOver �v���p�e�B�̎��ʎq


	static const String ControlTemplateTypeName;	///< "Button"


	static Button* Create(GUIManager* manager);

	Button(GUIManager* manager);
	virtual ~Button();

public:
	bool IsMouseOver() const { return m_isMouseOver; }

	//virtual void Render();

protected:
	virtual void OnClick();
	virtual bool OnEvent(EventType type, EventArgs* args);
	virtual void OnRender();
	//virtual void Render();


	virtual void OnMouseMove(MouseEventArgs* e);
	//void UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e);

protected:
	// override UIElement

	//virtual bool ApplyTemplateInternal()
	//{
	//	if (ApplyTemplateInternalMain(ControlTemplateTypeName)) { return true; }
	//	return ContentControl::ApplyTemplateInternal();	// base
	//}

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: ���B�����ƃe�[�}����ǂ݂���
	bool m_isMouseOver;
};

} // namespace GUI
} // namespace Lumino
