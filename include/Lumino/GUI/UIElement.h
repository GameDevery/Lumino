
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button

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

public:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

	//
	//void SetWidth(float width) { m_size.Width = width; }

	///// �v�f�̕����擾���܂��B
	//float GetWidth() { return m_size.Width; }

	///// �v�f�̍�����ݒ肵�܂��B�K��l�� NAN �ŁA�����I�ɃT�C�Y���v�Z���܂��B
	//void SetHeight(float width) { m_size.Width = width; }

	//

	/// �v�f�̃T�C�Y��ݒ肵�܂��B�K��l�� NAN �ŁA�����I�ɃT�C�Y���v�Z���܂��B
	void SetSize(const SizeF& size) { SetValue(SizeProperty, size); }	// TODO: ���C�A�E�g�X�V���͕p�ɂɃA�N�Z�X����̂łł���� �����o�ϐ��ɂ������E�E�E
	
	/// �v�f�̃T�C�Y���擾���܂��B
	const SizeF& GetSize() const { return GetValue(SizeProperty).GetSizeF(); }	// TODO: ��Ȃ��B�Q�ƂŕԂ��ƃX�^�b�N�� Variant �ŏ�����


	/// �q�b�g�e�X�g�̗L����ݒ肵�܂��B
	void SetHitTest(bool enabled) { SetValue(IsHitTestProperty, Variant(enabled)); }

	/// �q�b�g�e�X�g�̗L�����擾���܂��B
	bool IsHitTest() const { return GetValue(IsHitTestProperty).GetBool(); }

	

	/// (�T�C�Y�̎����v�Z���L���ɂȂ��Ă���v�f�ɑ΂��Ă͌Ăяo���Ă����ʂ͂���܂���)
	void UpdateLayout();
	virtual void Render();

	virtual void ApplyTemplate(CombinedLocalResource* parent);

public:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);

	virtual void OnApplyTemplate() {}
	virtual void OnRender() {}
	virtual bool OnEvent(EventType type, EventArgs* args) { return false; }

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:

	GUIManager*			m_manager;
	SizeF				m_desiredSize;			///< MeasureLayout() �Ō��肳��邱�̃R���g���[���̍ŏI�v���T�C�Y
	RectF				m_finalRect;			///< �`��Ɏg�p����ŏI���E��` (�O���[�o�����W�n=RootPane �̃��[�J�����W�n)

	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;
};

/**
	@brief		1 �̎q�v�f�̎��͂𑕏����� UI �v�f�̊�{�N���X�ł��B
*/
class Decorator
	: public UIElement
{
public:
	Decorator(GUIManager* manager);
	virtual ~Decorator();

public:
	void SetChild(UIElement* element);
	UIElement* GetChild() const;

public:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplate(CombinedLocalResource* parent);

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

private:
	RefPtr<UIElement>	m_child;
};

/**
	@brief		Button �R���g���[���̊O�ςł��B
*/
class ButtonChrome
	: public Decorator
{
public:
	static const String TypeName;	///< "ButtonChrome"

public:
	static CoreObject* CreateInstance(GUIManager* manager) { return LN_NEW ButtonChrome(manager); }

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

protected:
	virtual void OnApplyTemplate();
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	int								m_bgMargin;	///< �w�i�C���[�W��`�悷�鎞�ɏk������s�N�Z����
};


/**
	@brief	ContentControl �̃R���e���c��\�����܂��B
*/
class ContentPresenter
	: public UIElement
{
public:
	static const String TypeName;	///< "ContentPresenter"

	static CoreObject* CreateInstance(GUIManager* manager) { return LN_NEW ContentPresenter(manager); }

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
public:
	static const String ControlTemplateTypeName;


public:
	Control(GUIManager* manager);
	virtual ~Control();


	virtual void ApplyTemplate(CombinedLocalResource* parent);

	virtual bool ApplyTemplateInternal();
	bool ApplyTemplateInternalMain(const String& typeName);
};

/**
	@brief		1 �̃R���e���c�����R���g���[����\���܂��B
*/
class ContentControl
	: public Control
{
public:
	ContentControl(GUIManager* manager);
	virtual ~ContentControl();


	/// ���̃R���g���[���̃R���e���c��ݒ肵�܂��B
	void SetContent(Variant value);

	virtual void Render();

protected:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplate(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender();
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

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
public:
	static const String ControlTemplateTypeName;	///< "Button"

	Button(GUIManager* manager);
	virtual ~Button();

public:


	//virtual void Render();

protected:
	virtual void OnClick();
	virtual bool OnEvent(EventType type, EventArgs* args);
	virtual void OnRender();

	virtual bool ApplyTemplateInternal()
	{
		if (ApplyTemplateInternalMain(ControlTemplateTypeName)) { return true; }
		return ContentControl::ApplyTemplateInternal();	// base
	}

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: ���B�����ƃe�[�}����ǂ݂���
};

} // namespace GUI
} // namespace Lumino
