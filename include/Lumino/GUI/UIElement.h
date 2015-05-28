
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"

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
	static const String	SizeProperty;

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

	/// ���� UIElement �Ƀv���p�e�B��o�^���܂��B
	void RegisterProperty(const String& propertyName, const Variant& defaultValue);

	/// �v���p�e�B�̒l��ݒ肵�܂��B
	void SetValue(const String& propertyName, const Variant& value);

	/// �v���p�e�B�̒l���擾���܂��B
	Variant GetValue(const String& propertyName) const;

	/// (�T�C�Y�̎����v�Z���L���ɂȂ��Ă���v�f�ɑ΂��Ă͌Ăяo���Ă����ʂ͂���܂���)
	void UpdateLayout();
	virtual void Render();

public:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);

	virtual void OnRender() {}
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }
	virtual bool OnEvent(EventType type, EventArgs* args) { return false; }

protected:
	typedef SortedArray<String, Variant>	PropertyDataStore;

	GUIManager*			m_manager;
	PropertyDataStore	m_propertyDataStore;
	SizeF				m_desiredSize;			///< MeasureLayout() �Ō��肳��邱�̃R���g���[���̍ŏI�v���T�C�Y
	RectF				m_finalRect;			///< �`��Ɏg�p����ŏI���E��`
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
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	int								m_bgMargin;	///< �w�i�C���[�W��`�悷�鎞�ɏk������s�N�Z����
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
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender();
	virtual bool OnEvent(EventType type, EventArgs* args);

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
