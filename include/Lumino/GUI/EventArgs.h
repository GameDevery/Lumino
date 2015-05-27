
#pragma once
#include "../Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		GUI �C�x���g�̈����̊�{�N���X�ł��B
*/
class EventArgs
	: public CoreObject
{
public:
	EventArgs();
	virtual ~EventArgs();

protected:

};

/**
	@brief		
*/
class PooledEventArgs
	: public EventArgs
{
public:
	PooledEventArgs();
	virtual ~PooledEventArgs();

protected:
	static const int DataSize = 64;
	byte_t	m_data[DataSize];
};

/**
	@brief	�}�E�X�C�x���g�̈�����\���܂��B
*/
class MouseEventArgs
	: public PooledEventArgs
{
public:
	MouseEventArgs();
	virtual ~MouseEventArgs();

protected:
	struct Data
	{
		MouseButton	Button;		///< �{�^���ԍ�
		short		Delta;		///< �}�E�X�z�C�[���̉�]�� (���܂��͕��̉�)
		short		X;			///< �}�E�X�C�x���g�������̃}�E�X�� X ���W (�N���C�A���g�̈�O�� -1)
		short		Y;			///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W (�N���C�A���g�̈�O�� -1)
		//short		MoveX;	    ///< X ���W�̈ړ���
		//short		MoveY;      ///< Y ���W�̈ړ���
	};
};

/**
	@brief	�L�[�{�[�h�C�x���g�̈�����\���܂��B	
*/
class KeyEventArgs
	: public PooledEventArgs
{
public:
	KeyEventArgs();
	virtual ~KeyEventArgs();

protected:
	struct Data
	{
		Key			KeyCode;	///< �L�[�R�[�h
		bool		IsAlt;		///< Alt �L�[��������Ă���ꍇ true
		bool		IsShift;	///< Shift �L�[��������Ă���ꍇ true
		bool		IsControl;	///< Ctrl �L�[��������Ă���ꍇ true
	};
};

class EventArgsPool
{
public:

private:
};

	//bool injectMouseMove(float delta_x, float delta_y);
	//bool injectMouseButtonDown(MouseButton button);
	//bool injectMouseButtonUp(MouseButton button);
	//bool injectKeyDown(Key::Scan scan_code);
	//bool injectKeyUp(Key::Scan scan_code);
	//bool injectChar(String::value_type code_point);
	//bool injectMouseWheelChange(float delta);


} // namespace GUI
} // namespace Lumino
