
#pragma once

namespace Lumino
{

/// �}�E�X�{�^��
enum MouseButton
{
	MouseButton_Left = 0,		///< ���{�^��
	MouseButton_Right,			///< �E�{�^��
	MouseButton_Middle,			///< �����{�^��

	MouseButton_None = 0xff		///< �����܂��͉�����Ă��Ȃ����Ƃ�����
};

/// �L�[�R�[�h (NumPad �̓T�|�[�g���Ă��Ȃ�)
enum Key
{
	Key_Unknown = 0,

	Key_A,
	Key_B,
	Key_C,
	Key_D,
	Key_E,
	Key_F,
	Key_G,
	Key_H,
	Key_I,
	Key_J,
	Key_K,
	Key_L,
	Key_M,
	Key_N,
	Key_O,
	Key_P,
	Key_Q,
	Key_R,
	Key_S,
	Key_T,
	Key_U,
	Key_V,
	Key_W,
	Key_X,
	Key_Y,
	Key_Z,

	Key_0,
	Key_1,
	Key_2,
	Key_3,
	Key_4,
	Key_5,
	Key_6,
	Key_7,
	Key_8,
	Key_9,

	Key_F1,
	Key_F2,
	Key_F3,
	Key_F4,
	Key_F5,
	Key_F6,
	Key_F7,
	Key_F8,
	Key_F9,
	Key_F10,
	Key_F11,
	Key_F12,

	Key_Space,	// 0x20 = ' ' (�X�y�[�X)
	Key_Escape,
	Key_Up,
	Key_Down,
	Key_Left,
	Key_Right,
	Key_LShift,
	Key_RShift,
	Key_LCtrl,
	Key_RCtrl,
	Key_LAlt,
	Key_RAlt,
	Key_Tab,
	Key_Enter,
	Key_BackSpace,
	Key_Insert,
	Key_Delete,
	Key_PageUp,
	Key_PageDown,
	Key_Home,
	Key_End,

	Key_Colon,		///< :	[�񐄏� GLFW�ł�;]
	Key_SemiColon,	///< ;	[�񐄏� GLFW�ł�=]
	Key_Comma,		///< ,
	Key_Period,		///< .
	Key_Slash,		///< /
	Key_Minus,		///< -
	Key_BackSlash,	///< \(�o�b�N�X���b�V��) [�񐄏� GLFW�ł�NOWORD2]
	Key_Yen,		///< \(BaskSpace�L�[�̍���\�L�[) [�񐄏� GLFW�ł�I]
	Key_Caret,		///< ^ [�񐄏� GLFW�ł�GLFW_KEY_APOSTROPHE(')]
	Key_LBracket,	///< [
	Key_RBracket,	///< ]

	Key_Android_Plus,		///< Android '+'

	key_Max = 256,
};

} // namespace Lumino
