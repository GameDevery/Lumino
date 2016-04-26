
#pragma once
#include "Common.h"
#include <Lumino/Reflection/ReflectionEventArgs.h>

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIEventArgs
	: public tr::ReflectionEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIEventArgs();
	virtual ~UIEventArgs();

public:
	tr::ReflectionObject*	sender;
	//bool					handled;
};

/**
	@brief	�}�E�X�C�x���g�̈����ł��B
*/
class UIMouseEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIMouseEventArgs();
	UIMouseEventArgs(MouseButton button, float x, float y, int clickCount);
	virtual ~UIMouseEventArgs();

public:
	MouseButton	button;			/**< �{�^���̎�� */
	float		x;				/**< �}�E�X�C�x���g�������̃}�E�X�� X ���W (�R���e�L�X�g���̍��W�n) */
	float		y;				/**< �}�E�X�C�x���g�������̃}�E�X�� Y ���W (�R���e�L�X�g���̍��W�n) */
	int			clickCount;		/**< �{�^�����N���b�N���ꂽ�񐔁B�_�u���N���b�N��g���v���N���b�N����ʂ��邽�߂Ɏg�p����B�ő�3�܂ŁB */
};

/**
	@brief	�L�[�{�[�h�C�x���g�̈����ł��B
*/
class UIKeyEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIKeyEventArgs();
	UIKeyEventArgs(Key keyCode, ModifierKeys modifierKeys);
	virtual ~UIKeyEventArgs();

public:
	Key				keyCode;		/**< �L�[�R�[�h */
	ModifierKeys	modifierKeys;	/** �C���L�[ */
	TCHAR			charCode;		/**< TextInput �C�x���g�ɂ��ʒm���ꂽ�ꍇ�A���̕����R�[�h���i�[����܂��B*/
};

/**
	@brief	�}�E�X�z�C�[���̈ړ��C�x���g�̈����ł��B
*/
class UIMouseWheelEventArgs
	: public UIEventArgs
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIMouseWheelEventArgs();
	UIMouseWheelEventArgs(int delta);
	virtual ~UIMouseWheelEventArgs();

public:
	int			wheel;			/**< �}�E�X�z�C�[���̉�]�� (���܂��͕��̉�) */
};

LN_NAMESPACE_END
