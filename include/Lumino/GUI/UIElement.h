
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button
#include "RoutedEvent.h"

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
typedef String	PropertyID;
typedef String	EventID;


	struct Enum
	{
		int m_value;
		Enum() : m_value(0) {}
		Enum(int value) : m_value(value) {}
		operator int() const { return m_value; }
	};

	struct HorizontalAlignment : public Enum
	{
		enum
		{
			Left = 0,
			Center,
			Right,
			Stretch,
		};

		// WPF �� Button �̏ꍇ�A�����l�� Stretch ������

		HorizontalAlignment() : Enum() {}
		HorizontalAlignment(int value) : Enum(value) {}

	};

	struct VerticalAlignment : public Enum
	{
		enum
		{
			Bottom = 0,		///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
			Center,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
			Stretch,		///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B
			Top,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
		};

		// WPF �� Button �̏ꍇ�A�����l�� Stretch ������

		VerticalAlignment() : Enum() {}
		VerticalAlignment(int value) : Enum(value) {}

	};

	struct Orientation : public Enum
	{
		enum
		{
			Vertical = 0,
			Horizontal,
		};

		Orientation() : Enum() {}
		Orientation(int value) : Enum(value) {}

	};

	//enum Orientation
	//{
	//	Horizontal = 0,
	//	Orientation_Vertical,
	//};


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
public:
	static const String	SizeProperty;
	static const String	HorizontalAlignmentProperty;
	static const String	VerticalAlignmentProperty;
	static const String	IsHitTestProperty;

	static const String	MouseMoveEvent;
	static const String	MouseLeaveEvent;
	static const String	MouseEnterEvent;

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
	void SetHitTest(bool enabled) { SetValue(IsHitTestProperty, Variant(enabled)); }

	/// �q�b�g�e�X�g�̗L�����擾���܂��B
	bool IsHitTest() const { return GetValue(IsHitTestProperty).GetBool(); }




	Event02<CoreObject*, MouseEventArgs*>	MouseMove;
	Event02<CoreObject*, MouseEventArgs*>	MouseEnter;
	Event02<CoreObject*, MouseEventArgs*>	MouseLeave;

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
	void SetTemplateBinding(Property* thisProp, const String& srcPropPath, UIElement* rootLogicalParent);

	/// (�T�C�Y�̎����v�Z���L���ɂȂ��Ă���v�f�ɑ΂��Ă͌Ăяo���Ă����ʂ͂���܂���)
	void UpdateLayout();
	virtual void Render();


	/// ���݂̃e���v���[�g����r�W���A���c���[���č\�z���܂��B
	/// ���̊֐��͕K�v�ȃ^�C�~���O�Ń��C�A�E�g�V�X�e������Ăяo����܂��B�ʏ�A�����I�ɌĂяo���K�v�͂���܂���B
	void ApplyTemplate();

public:
	const SizeF& GetDesiredSize() const {
		return m_desiredSize;
	}


	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);

	/// ���݂̃e���v���[�g����r�W���A���c���[���č\�z���ꂽ��ɌĂяo����܂��B
	/// �h���N���X�� localResource �ɑ΂��ăL�[�l���烊�\�[�X���擾���邱�Ƃ��ł��܂��B
	virtual void OnApplyTemplate(CombinedLocalResource* localResource) {}
	virtual void OnRender() {}
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

public:	// internal
	void SetParent(UIElement* parent) { m_parent = parent; }
	//virtual void AddChildToVisualTree(UIElement* element) { LN_THROW(0, InvalidOperationException); }

protected:
	virtual const String& GetTypeID() const = 0;

	friend class Decorator;
	friend class Control;
	friend class ContentControl;
	friend class Panel;
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	friend class UIElementFactory;
	friend class ControlTemplate;
	friend class DataTemplate;
	virtual void AddVisualChild(UIElement* child);	///< AddChild() �͘_���c���[�ւ̒ǉ��AAddVisualChild() �̓r�W���A���c���[�ւ̒ǉ��B

private:
	// TypedRoutedEvent ����R�[���o�b�N�����BOn�`�Ƃ͕ʂɁA�P�ɃC�x���g�𔭐�������R�[���o�N�Ƃ��ĕK�v�B(TypedRoutedEvent �͏�Ԃ����ĂȂ��̂�)
	// �ǂ�Ȏ���g���Ă��A���� TypedRoutedEvent ���� Event02 ���Ăׂ��i���K�v�Ȃ̂ŁA�����͕K���K�v�ɂȂ�B
	void CallMouseMoveEvent(CoreObject* sender, MouseEventArgs* e) { MouseMove.Raise(sender, e); }
	void CallMouseEnterEvent(CoreObject* sender, MouseEventArgs* e) { MouseEnter.Raise(sender, e); }
	void CallMouseLeaveEvent(CoreObject* sender, MouseEventArgs* e) { MouseLeave.Raise(sender, e); }

protected:
	void RegisterRoutedEvent(RoutedEvent* ev) {
		m_routedEventList.Add(ev->GetName(), ev);
	}

	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	void RaiseEvent(const String& eventName, CoreObject* sender, EventArgs* e)
	{
		LN_FOREACH(RoutedEventList::Pair& pair, m_routedEventList)
		{
			if (pair.first == eventName) {
				pair.second->CallEvent(this, sender, e);
				break;	// ev �Ɠ����C�x���g��1�������X�g���ɖ����͂�
			}
		}

		// bubble
		if (!e->Handled && m_parent != NULL)
		{

			LN_FOREACH(RoutedEventList::Pair& pair, m_parent->m_routedEventList)
			{
				if (pair.first == eventName) {
					pair.second->Raise(m_parent, sender, e);	// �e��On�` ���Ă΂��
					break;	// ev �Ɠ����C�x���g��1�������X�g���ɖ����͂�
				}
			}

			//m_parent->RaiseEvent(eventName, sender, e);
		}
		
	}

	// �T�u�N���X�͂ł��邾�� Event �Ƀn���h����o�^����̂ł͂Ȃ��AOn�`���I�[�o�[���C�h���邱�ƂŃC�x���g���n���h�����O����B
	// �n���h�����O������ e->Handled �� true �ɂ���B������ super ���Ăяo���B�������邱�ƂŁARaiseEvent() �ł̃C�x���g������
	// ���[�e�B���O���s��Ȃ��̂ŕ��׌y�����ł���B
	virtual void OnMouseMove(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }


	GUIManager*				m_manager;
	UIElement*				m_parent;				///< �_���c���[��̐e�v�f

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() �Ō��肳��邱�̃R���g���[���̍ŏI�v���T�C�Y
	RectF					m_finalRect;			///< �`��Ɏg�p����ŏI���E��` (�O���[�o�����W�n=RootPane �̃��[�J�����W�n)

	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;

	// �폜�\��
	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;
	//Event02<CoreObject*, MouseEventArgs*> m_eventMouseMove;

	typedef SortedArray<String, RoutedEvent*>	RoutedEventList;
	RoutedEventList	m_routedEventList;


	// Property
	SizeF				m_size;
	HorizontalAlignment	m_horizontalAlignment;
	VerticalAlignment	m_verticalAlignment;

private:

	void TemplateBindingSource_PropertyChanged(CoreObject* sender, PropertyChangedEventArgs* e);


	struct TemplateBindingInfo
	{
		Property*	ThisProp;
		String		SourcePropPath;
	};

	typedef Array<TemplateBindingInfo>	TemplateBindingInfoList;
	TemplateBindingInfoList		m_templateBindingInfoList;

	UIElement*		m_rootLogicalParent;	///< �e���v���[�g��K�p�����v�f�BTemplateBinding �̃\�[�X�I�u�W�F�N�g�B���ꂪ NULL �łȂ���΁Athis �� VisualTree �v�f�ł���B

protected:
	/// ���ڂ̎q Visual ���X�g�B
	/// ���̃��X�g�ɘ_���v�f�͒��ڊ܂܂Ȃ��B�_���v�f�� ������ Visual �̉��ɂ��� ContentPresenter �܂��� ItemsPresenter �̎q�Ƃ��Ēǉ������B
	/// �Ⴆ�� Button::SetContent() �� �Z�b�g���ꂽ UIElement �� m_visualChildren ����H��� ContentPresenter �ɒǉ������B
	Array< RefPtr<UIElement> >	m_visualChildren;	// TOOD: List �ɂ��Ȃ��Ă����������H
	//UIElement*		m_visualChildren;
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
	static const String	IsMouseOverProperty;	///< IsMouseOver �v���p�e�B�̎��ʎq
	static const String	FrameWidthProperty;		///< FrameWidth �v���p�e�B�̎��ʎq

public:
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
	virtual void SetValue(const String& propertyName, const Variant& value);
	virtual Variant GetValue(const String& propertyName) const;

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
	ContentPresenter(GUIManager* manager);
	virtual ~ContentPresenter();

protected:
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


public:
	Control(GUIManager* manager);
	virtual ~Control();

protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

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
	//virtual void ArrangeLayout(const RectF& finalRect);
	//virtual void OnRender();
	//virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
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
