
#pragma once
#include "../Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/// �����C�x���g�̎��
enum EventType
{
	EventType_Unknown = 0,
	EventType_MouseMove,
	EventType_MouseButtonDown,
	EventType_MouseButtonUp,
	EventType_MouseWheel,

	EventType_KeyDown,
	EventType_KeyUp,
	//EventType_Char,
	EventType_ElapsedTime,


	EventType_MouseLeave,
	EventType_MouseEnter,
};

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
	@brief	�}�E�X�C�x���g�̈�����\���܂��B
*/
class MouseEventArgs
	: public EventArgs
{
public:
	MouseEventArgs();
	virtual ~MouseEventArgs();

public:
	MouseButton	Button;		///< �{�^���ԍ�
	short		Wheel;		///< �}�E�X�z�C�[���̉�]�� (���܂��͕��̉�)
	short		X;			///< �}�E�X�C�x���g�������̃}�E�X�� X ���W (�O���[�o�����W�B�N���C�A���g�̈�O�� -1)
	short		Y;			///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W (�O���[�o�����W�B�N���C�A���g�̈�O�� -1)
	//short		MoveX;	    ///< X ���W�̈ړ���
	//short		MoveY;      ///< Y ���W�̈ړ���
};

/**
	@brief	�L�[�{�[�h�C�x���g�̈�����\���܂��B	
*/
class KeyEventArgs
	: public EventArgs
{
public:
	KeyEventArgs();
	virtual ~KeyEventArgs();

public:
	Key			KeyCode;	///< �L�[�R�[�h
	bool		IsAlt;		///< Alt �L�[��������Ă���ꍇ true
	bool		IsShift;	///< Shift �L�[��������Ă���ꍇ true
	bool		IsControl;	///< Ctrl �L�[��������Ă���ꍇ true
};

/**
	@brief	
	@note	���̃N���X�̓C�x���g�����̕p�ɂ� new ������邽�߂Ɏg�p����B
			�Ⴆ�� MouseMove �C�x���g�͍ň����t���[���̂悤�ɔ�������\��������Anew �͑傫�ȃI�[�o�[�w�b�h�ɂȂ�B
			����Ȃ� union �𗘗p�����C�x���g�����\���̂��g���̂��ЂƂ̎肩������Ȃ��B
			
			�������A�{���C�u������ C# �� Ruby ���A���̌���ւ̃o�C���f�B���O��z�肵�Ă���B
			���R GUI ���W���[�������J����A�g������邱�Ƃ��O��B
			C# ���ō쐬�������[�U�[�R���g���[���� MouseEventArgs ���g�p���邱�Ƃ͑z�肵�Ȃ���΂Ȃ�Ȃ��B

			union �� struct �ɂ����ꍇ�A����͂���Ō���ʂɗ]�v�ȃI�[�o�[�w�b�h�⍇�킹���݂̎����������邱�ƂɂȂ�B
			�Ⴆ�� C# �ł͒l�n���̂܂܂ł悢�̂��Aref ������̂��BRuby �� struct �͎g���Ȃ��̂Ō��� new ����H�ڂɂȂ邪�����̂��B

			Pool ���Ă����΁A�኱�����I�ł͂Ȃ��Ȃ邪�A�o�C���_���� new ���}���邱�Ƃ��ł���B

			��X�C�x���g�̎�ނ������Ă����Ƃ��͊g�����̂��߁A�C�x���g�����L�[�ɂ��� Create ����悤�Ȏd�g�݂��K�v�ɂȂ邩������Ȃ��B
*/
class EventArgsPool
{
public:
	EventArgsPool();
	~EventArgsPool();

	MouseEventArgs* CreateMouseEventArgs(MouseButton button, short wheel, short x, short y);
	KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);

private:
	template<class T>
	T FindFreeObject(const ArrayList<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetRefCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	ArrayList<MouseEventArgs*>	m_mouseEventArgsPool;
	ArrayList<KeyEventArgs*>	m_keyEventArgsPool;
};








template<typename A1, typename A2>
class Event02 : public RefObject
{
public:
	typedef Delegate02<A1, A2> DelegateType;

public:
	void AddHandler(const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void RemoveHandler(const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void operator += (const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void operator -= (const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void Raise(A1 a1, A2 a2)	// GUI �� EventArgs �� Handler ��Ԃ������Ƃ�������̂� const �Q�Ƃɂ͂��Ȃ�
	{
		LN_FOREACH(DelegateType& d, m_handlerList)
		{
			d.Call(a1, a2);
		}
	}

private:
	ArrayList<DelegateType> m_handlerList;
};

} // namespace GUI
} // namespace Lumino
