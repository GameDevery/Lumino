
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button

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

	/// �q�b�g�e�X�g�̗L����ݒ肵�܂��B
	void SetHitTest(bool enabled) { SetValue(IsHitTestProperty, Variant(enabled)); }

	/// �q�b�g�e�X�g�̗L�����擾���܂��B
	bool IsHitTest() const { return GetValue(IsHitTestProperty).GetBool(); }

	

	void AddMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { AddHandler(MouseMoveEvent, handler); }
	void RemoveMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { RemoveHandler(MouseMoveEvent, handler); }

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
	friend class ContentControl;
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

public:
	template<typename TArgs>
	void AddHandler(const String& eventName, const Delegate02<CoreObject*, TArgs*>& handler)
	{
		RefObject* ev;
		if (m_eventDataStore.TryGetValue(eventName, &ev)) {
			static_cast<Event02<CoreObject*, TArgs*>*>(ev)->AddHandler(handler);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}
	template<typename TArgs>
	void RemoveHandler(const String& eventName, const Delegate02<CoreObject*, TArgs*>& handler)
	{
		RefObject* ev;
		if (m_eventDataStore.TryGetValue(eventName, &ev)) {
			static_cast<Event02<CoreObject*, TArgs*>*>(ev)->RemoveHandler(handler);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}
	template<typename TArgs>
	void RaiseEvent(const String& eventName, CoreObject* sender, TArgs* args)
	{
		RefObject* ev;
		if (m_eventDataStore.TryGetValue(eventName, &ev)) {
			args->HandlerOwner = this;
			static_cast<Event02<CoreObject*, TArgs*>*>(ev)->Raise(sender, args);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}



protected:


	GUIManager*				m_manager;
	UIElement*				m_parent;				///< �_���c���[��̐e�v�f

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() �Ō��肳��邱�̃R���g���[���̍ŏI�v���T�C�Y
	RectF					m_finalRect;			///< �`��Ɏg�p����ŏI���E��` (�O���[�o�����W�n=RootPane �̃��[�J�����W�n)

	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;


	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;
	//Event02<CoreObject*, MouseEventArgs*> m_eventMouseMove;



	// Property
	SizeF	m_size;

private:

	void TemplateBindingSource_PropertyChanged(CoreObject* sender, PropertyChangedEventArgs* e);


	struct TemplateBindingInfo
	{
		Property*	ThisProp;
		String		SourcePropPath;
	};

	typedef ArrayList<TemplateBindingInfo>	TemplateBindingInfoList;
	TemplateBindingInfoList		m_templateBindingInfoList;

	UIElement*		m_rootLogicalParent;	///< �e���v���[�g��K�p�����v�f�BTemplateBinding �̃\�[�X�I�u�W�F�N�g�B���ꂪ NULL �łȂ���΁Athis �� VisualTree �v�f�ł���B
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
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }


protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);;
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

	virtual void Render();

protected:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender();
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
	Variant		m_content;		
	UIElement*	m_childElement;	///< m_content �� UIElement �ł���΂�����w��
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
