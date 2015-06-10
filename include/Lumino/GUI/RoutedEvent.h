
#pragma once

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class RoutedEvent
{
public:
	RoutedEvent() {}
	~RoutedEvent() {}

public:
	virtual const String& GetName() const = 0;

	virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e) = 0;
};

/**
	@brief		
*/
template<class TClass, typename TEventArgs>
class TypedRoutedEvent : public RoutedEvent
{
public:
	typedef void (TClass::*RaiseEvent)(CoreObject* sender, TEventArgs);

	// Event �𒼐ڎQ�Ƃ��Ă͂Ȃ�Ȃ��B���̃N���X�� Property �Ɠ������A������ UIElement �ŋ��L�����B��Ԃ������Ă͂Ȃ�Ȃ��B
	// �Ȃ̂ŁA�C�x���g�� Raise ����֐��|�C���^���Q�Ƃ���B

public:
	TypedRoutedEvent(const String& name, RaiseEvent raiseEvent)
		: m_name(name)
		, m_setter(setter)
		, m_getter(getter)
	{}

	virtual ~TypedRoutedEvent() {}

	virtual const String& GetName() const { return m_name; }

	virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e)
	{
		TClass* instance = static_cast<TClass*>(target);
		TClass* et = static_cast<TClass*>(target);
		(instance->*m_raiseEvent)(instance, sender, et);
	}

private:
	String		m_name;
	RaiseEvent	m_raiseEvent;
};

#define LN_DEFINE_ROUTED_EVENT(classType, eventType, name, raiseEventFuncPtr) \
{ \
	static ::Lumino::TypedRoutedEvent<classType, eventType> ev(name, raiseEventFuncPtr); \
	RegisterRoutedEvent(&ev); \
}

} // namespace GUI
} // namespace Lumino
