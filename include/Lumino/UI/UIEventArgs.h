
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
	UIMouseEventArgs(MouseButton button, int wheel, float x, float y, int clickCount);
	virtual ~UIMouseEventArgs();

public:
	MouseButton	button;			/**< �{�^���̎�� */
	int			wheel;			/**< �}�E�X�z�C�[���̉�]�� (���܂��͕��̉�) */
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
	UIKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);
	virtual ~UIKeyEventArgs();

public:
	Key			keyCode;		/**< �L�[�R�[�h */
	bool		isAlt;			/**< Alt �L�[��������Ă���ꍇ true */
	bool		isShift;		/**< Shift �L�[��������Ă���ꍇ true */
	bool		isControl;		/**< Ctrl �L�[��������Ă���ꍇ true */
	TCHAR		charCode;		/**< TextInput �C�x���g�ɂ��ʒm���ꂽ�ꍇ�A���̕����R�[�h���i�[����܂��B*/
};

LN_NAMESPACE_END
