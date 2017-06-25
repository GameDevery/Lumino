
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
class UIButton;
using UIButtonPtr = RefPtr<UIButton>;
class UITextBlock;

/** �{�^���̃N���b�N�C�x���g�𔭐�������^�C�~���O��\���܂��B*/
enum class ClickMode
{
	/** �{�^���𗣂����Ƃ��ɃC�x���g�𔭐������܂��B*/
	Release,

	/** �{�^�����������Ƃ��ɃC�x���g�𔭐������܂��B*/
	Press,
};

/**
	@brief		
*/
class UIButtonBase
	: public UIControl
{
	LN_OBJECT();
public:
	void setText(const StringRef& text);

	/** onClick �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B*/
	LN_METHOD(Event)
	EventConnection connectOnGotFocus(UIEventHandler handler);

protected:

	/** �{�^�����N���b�N���ꂽ�Ƃ��ɌĂяo����܂��B*/
	virtual void onClick(UIEventArgs* e);

	// UIElement interface
	virtual void onMouseDown(UIMouseEventArgs* e) override;
	virtual void onMouseUp(UIMouseEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIButtonBase();
	virtual ~UIButtonBase();
	void initialize();

private:
	ClickMode			m_clickMode;
	bool				m_isPressed;
	RefPtr<UITextBlock>	m_textContent;

	UIEventHandler::EventType	m_onClick;
};

/**
	@brief		
*/
class UIButton
	: public UIButtonBase
{
	LN_OBJECT();
public:
	static RefPtr<UIButton> create();
	static RefPtr<UIButton> create(const StringRef& text, float width, float height);

LN_CONSTRUCT_ACCESS:
	UIButton();
	virtual ~UIButton();
	void initialize();
	void initialize(const StringRef& text, float width, float height);
};


/**
	@brief		
*/
class UIToggleButton
	: public UIButtonBase
{
	LN_OBJECT();
public:
	static const String CheckedState;
	static const String UncheckedState;

	static RefPtr<UIToggleButton> create();
	
	/** Checked �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B*/
	LN_METHOD(Event)
	EventConnection connectOnChecked(UIEventHandler handler);

	/** Unchecked �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B*/
	LN_METHOD(Event)
	EventConnection connectOnUnchecked(UIEventHandler handler);

protected:
	virtual void onClick(UIEventArgs* e);

LN_CONSTRUCT_ACCESS:
	UIToggleButton();
	virtual ~UIToggleButton();
	void initialize();

private:
	void checkChanged();

	bool						m_isChecked;
	UIEventHandler::EventType	m_onChecked;
	UIEventHandler::EventType	m_onUnchecked;
	//Nullable<bool>			m_isChecked;
	//UIEventHandler::EventType	m_onIndeterminate; 
};

LN_NAMESPACE_END
