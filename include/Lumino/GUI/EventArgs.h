
#pragma once
#include "../Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{
class UIElement;

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


	EventType_MouseEnter,
	EventType_MouseLeave,
};

/**
	@brief		GUI �C�x���g�̈����̊�{�N���X�ł��B
*/
class EventArgs
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	EventArgs();
	virtual ~EventArgs();

public:
	CoreObject*	Sender;
	UIElement* HandlerOwner;
	bool Handled;

protected:

};

/**
	@brief	�}�E�X�C�x���g�̈�����\���܂��B
*/
class MouseEventArgs
	: public EventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	MouseEventArgs();
	virtual ~MouseEventArgs();

public:
	MouseButton	Button;		///< �{�^���ԍ�
	int			Wheel;		///< �}�E�X�z�C�[���̉�]�� (���܂��͕��̉�)
	float		X;			///< �}�E�X�C�x���g�������̃}�E�X�� X ���W (�O���[�o�����W�B�N���C�A���g�̈�O�� -1)
	float		Y;			///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W (�O���[�o�����W�B�N���C�A���g�̈�O�� -1)
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
	@brief	�X�N���[���C�x���g�̈�����\���܂��B
*/
class ScrollEventArgs
	: public EventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	ScrollEventArgs();
	virtual ~ScrollEventArgs();

public:
	double			NewValue;	///< �V�����l
	//ScrollEventType	EventType;	///< �X�N���[���C�x���g�̎��
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
			�Ⴆ�� C# ��B�͒l�n���̂܂܂ł悢�̂��Aref ������̂��BRuby �� struct �͎g���Ȃ��̂Ō��� new ����H�ڂɂȂ邪�����̂��B

			Pool ���Ă����΁A�኱�����I�ł͂Ȃ��Ȃ邪�A�o�C���_���� new ���}���邱�Ƃ��ł���B

			��X�C�x���g�̎�ނ������Ă����Ƃ��͊g�����̂��߁A�C�x���g�����L�[�ɂ��� Create ����悤�Ȏd�g�݂��K�v�ɂȂ邩������Ȃ��B
*/
class EventArgsPool
{
public:
	EventArgsPool();
	~EventArgsPool();

	MouseEventArgs* CreateMouseEventArgs(MouseButton button, int wheel, float x, float y);
	KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* Create(TArgs... args)
	{
		TEventArgs* e = static_cast<TEventArgs* >(Find(TEventArgs::GetClassTypeInfo()));
		if (e == NULL) {
			e = LN_NEW TEventArgs(args...);
			Register(e);
		}
		else {
			e->~TEventArgs();
			new (e)TEventArgs(args...);
		}
		e->Handled = false;
		e->AddRef();
		return e;
	}

private:
	typedef Array<EventArgs*>					EventArgsList;
	typedef SortedArray<TypeInfo*, EventArgsList*>	EventArgsMap;
	EventArgsMap	m_pool;

	EventArgs* Find(TypeInfo* typeId)
	{
		EventArgsList* list;
		if (m_pool.TryGetValue(typeId, &list))
		{
			for (auto e : (*list))
			{
				if (e->GetRefCount() == 1) {	// ���̃��X�g���炵���Q�Ƃ���Ă��Ȃ���ΕԂ�
					return e;
				}
			}
		}
		return NULL;
	}

	void Register(EventArgs* e)
	{
		EventArgsList* list;
		if (!m_pool.TryGetValue(GetTypeInfo(e), &list))
		{
			list = LN_NEW EventArgsList();
			m_pool.Add(GetTypeInfo(e), list);
		}
		list->Add(e);
	}


	//Array<MouseEventArgs*>	m_mouseEventArgsPool;





	template<class T>
	T FindFreeObject(const Array<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetRefCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	Array<MouseEventArgs*>	m_mouseEventArgsPool;
	Array<KeyEventArgs*>	m_keyEventArgsPool;
};




} // namespace GUI
} // namespace Lumino
