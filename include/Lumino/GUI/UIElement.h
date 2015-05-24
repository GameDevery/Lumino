
#pragma once

#include <Lumino/Graphics/Painter.h>
#include "Common.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class UIElement
	: public CoreObject
{
public:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

	virtual void Render();

protected:
	virtual void OnRender() {}
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	GUIManager*	m_manager;
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
	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

protected:
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
};

/**
	@brief		���[�U�[����ɂ���ē���̓�����s�� UI �v�f�̊�{�N���X�ł��B
*/
class Control
	: public UIElement
{
public:
	Control(GUIManager* manager);
	virtual ~Control();
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
	virtual void OnRender();

private:
	Variant		m_content;		
	UIElement*	m_childElement;	///< m_content �� UIElement �ł���΂�����w��
};

/**
	@brief		���[�g�v�f�B
*/
class Workbench
	: public ContentControl
{
public:
	Workbench(GUIManager* manager);
	virtual ~Workbench();

	//virtual void Render();

protected:
	//virtual void OnRender() {}

private:
};

/**
	@brief		
*/
class Button
	: public ContentControl
{
public:
	Button(GUIManager* manager);
	virtual ~Button();

public:


	//virtual void Render();

protected:
	virtual void OnRender();

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: ���B�����ƃe�[�}����ǂ݂���
};

} // namespace GUI
} // namespace Lumino
