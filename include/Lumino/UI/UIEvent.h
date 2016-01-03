
#pragma once
#include <functional>
#include <Lumino/Reflection/Notify.h>
#include <Lumino/Reflection/TypeInfo.h>
#include "Common.h"
#include "UIEventArgs.h"

LN_NAMESPACE_BEGIN

///**
//	@brief		
//*/
//class UIEventSlotBase
//{
//protected:
//	UIEventSlotBase() = default;
//	virtual ~UIEventSlotBase() = default;
//
//private:
//	friend class UIElement;
//	virtual void Raise(UIEventArgs* e) = 0;
//};
/**
	@brief		
	@details
*/
template<class TArgs>
class UIEvent
	: public tr::ReflectionEvent<TArgs>
{
public:
	UIEvent() = default;
	virtual ~UIEvent() = default;
};
///**
//	@brief		
//	@details	RoutedEvent �� UIElement �y�т��̃T�u�N���X��������̂ݔ��������邱�Ƃ��o���܂��B
//*/
//template<class TArgs>
//class UIEventSlot
//	: public UIEventSlotBase
//{
//public:
//	UIEventSlot() = default;
//	virtual ~UIEventSlot() = default;
//
//public:
//
//	/**
//		@brief	���[�e�B���O�C�x���g�̃n���h����ǉ����܂��B
//	*/
//	void AddHandler(const Delegate<void(TArgs*)>& handler)
//	{
//		m_handlerList.Add(handler);
//	}
//	
//	/**
//		@brief	�w�肵���n���h���Ɉ�v����n���h�����A���̃X���b�g����폜���܂��B
//	*/
//	void RemoveHandler(const Delegate<void(TArgs*)>& handler)
//	{
//		m_handlerList.Remove(handler);
//	}
//	
//	/**
//		@brief	���[�e�B���O�C�x���g�̃n���h����ǉ����܂��B
//	*/
//	void operator += (const std::function<void(TArgs*)>& handler)
//	{
//		AddHandler(handler);
//	}
//
//	void operator += (const Delegate<void(TArgs*)>& handler)
//	{
//		m_handlerList.Add(handler);
//	}
//	
//	/**
//		@brief	�w�肵���n���h���Ɉ�v����n���h�����A���̃X���b�g����폜���܂��B
//	*/
//	void operator -= (const Delegate<void(TArgs*)>& handler)
//	{
//		m_handlerList.Remove(handler);
//	}
//
//private:
//	Array< Delegate<void(TArgs*)> > m_handlerList;
//
//	virtual void Raise(UIEventArgs* e)
//	{
//		for (Delegate<void(TArgs*)>& d : m_handlerList)
//		{
//			d.Call(static_cast<TArgs*>(e));
//		}
//	}
//};
//
/**
	@brief		
*/
class UIEventInfo
	: public tr::ReflectionEventInfo
{
public:
	UIEventInfo(tr::TypeInfo* ownerClass, const String& name, RaiseEventFunc raiseEvent)
		: tr::ReflectionEventInfo(ownerClass, name, raiseEvent)
	{}
};
//	// Event �𒼐ڎQ�Ƃ��Ă͂Ȃ�Ȃ��B���̃N���X�� Property �Ɠ������A������ UIElement �ŋ��L�����B��Ԃ������Ă͂Ȃ�Ȃ��B
//	// �Ȃ̂ŁA�C�x���g�� Raise ����֐��|�C���^���Q�Ƃ���B
//
//	typedef void(*RaiseEventFunc)(UIElement* obj, UIEventArgs* e);
//
//public:
//	UIEvent(UITypeInfo* ownerClass, const String& name, RaiseEventFunc raiseEvent);
//	~UIEvent();
//
//public:
//	//const String& GetName() const { return m_name; }
//
//	//void CallEvent(UIElement* target, UIEventArgs* e) const
//	//{
//	//	m_raiseEvent(target, e);
//	//}
//
//private:
//	//friend class UITypeInfo;
//	//String		m_name;
//	//RaiseEventFunc	m_raiseEvent;
//	//bool	m_registerd;
//};
//
//#define LN_ROUTED_EVENT(eventArgs, eventVar) \
//	public:  static const UIEvent* eventVar; \
//	private: static void	_raise_##eventVar(UIElement* obj, UIEventArgs* e); \
//	private: static UIEvent _init_##eventVar;
//
//#define LN_ROUTED_EVENT_IMPLEMENT(ownerClass, eventArgs, eventVar, name, slot) \
//	UIEvent					ownerClass::_init_##eventVar(static_cast<UITypeInfo*>(tr::TypeInfo::GetTypeInfo<ownerClass>()), name, &ownerClass::_raise_##eventVar); \
//	const UIEvent*			ownerClass::eventVar = &_init_##eventVar; \
//	void					ownerClass::_raise_##eventVar(UIElement* obj, UIEventArgs* e) { static_cast<ownerClass*>(obj)->EmitEventSlot(static_cast<ownerClass*>(obj)->slot, static_cast<eventArgs*>(e)); }
//
#define LN_ROUTED_EVENT(eventArgs, eventInfoVar) \
	LN_REFLECTION_EVENT_COMMON(UIEventInfo, eventArgs, eventInfoVar);

#define LN_ROUTED_EVENT_IMPLEMENT(ownerClass, eventArgs, eventInfoVar, name, ev) \
	LN_REFLECTION_EVENT_IMPLEMENT_COMMON(UIEventInfo, ownerClass, eventArgs, eventInfoVar, name, ev);


LN_NAMESPACE_END
