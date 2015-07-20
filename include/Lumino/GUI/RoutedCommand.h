
#pragma once
#include <memory>
#include <functional>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Event.h>
#include "../Variant.h"
#include "EventArgs.h"
//#include "RoutedEvent.h"

namespace Lumino
{
namespace GUI
{
class UIElement;

/**
	@brief
*/
class Command
{
public:
	Event01<bool>	CanExecuteChanged;

	virtual bool CanExecute(const Variant& parameter) = 0;

	virtual void Execute(const Variant& parameter) = 0;

	/*
		CanExecute() �� Execute() �͒��ڌĂяo�����ACommandManager �o�R���邱�ƁB
	*/

protected:
	virtual ~Command() {}
};

/**
	@brief		
	@note		�K�� static �ȃC���X�^���X�ł��邱�ƁB
				����o�C���_�ł��A�K�� static �����o�� new ���邱�ƁB
*/
class RoutedCommand : public Command
{
public:
	typedef std::function<void(CoreObject, const Variant&) >	Handler;

public:
	RoutedCommand(TypeInfo* ownerClass, const String& commandName, Handler canExecute, Handler execute)
		: m_typeInfo(ownerClass)
		, m_commandName(commandName)
		, m_canEcecute(canExecute)
		, m_ececute(execute)
		, m_registerd(false)
	{}

	virtual ~RoutedCommand() {}

	TypeInfo* GetOwnerClass() const
	{
		return m_typeInfo;
	}

	virtual bool CanExecute(const Variant& parameter) {}

	virtual void Execute(const Variant& parameter) {}

private:
	TypeInfo*		m_typeInfo;
	String			m_commandName;
	Handler			m_canEcecute;	///< static �֐�
	Handler			m_ececute;		///< static �֐�

	friend class CommandManager;
	bool	m_registerd;
};

//#define LN_DEFINE_ROUTED_EVENT(classType, eventArgsType, name, callEventFuncPtr) \
//{ \
//	static ::Lumino::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, callEventFuncPtr); \
//	RegisterRoutedEvent(&ev); \
//}


/// TypeInfo �������N���X�Ɋ܂܂�Ă��� RoutedCommand �̃��X�g
class RoutedCommandTypeContext
{
public:
	const TypeInfo*			Type;
	Array<RoutedCommand*>	RoutedCommandList;
};

class CommandManager
{
public:

public:

	/// �e�N���X�̃R���X�g���N�^����Ă΂��Bcommand �� static �I�u�W�F�N�g�ɂ��邱�ƁB
	static RoutedCommandTypeContext* RegisterCommand(TypeInfo* type, RoutedCommand* command);

	static bool CanExecute(UIElement* caller, Command* command, const Variant& parameter);

	static void Execute(UIElement* caller, Command* command, const Variant& parameter);

private:
	typedef SortedArray<TypeInfo*, std::shared_ptr<RoutedCommandTypeContext> >	TypeContextList;
	static TypeContextList	m_typeContextList;
};

class CanExecuteRoutedCommandEventArgs
	: public EventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	CanExecuteRoutedCommandEventArgs(const Variant& parameter) : Parameter(parameter) { CanExecute = false; }
	virtual ~CanExecuteRoutedCommandEventArgs() {}

public:
	const Variant&	Parameter;
	bool			CanExecute;		///< �����Ɍ��ʂ��i�[����
};

class ExecuteRoutedCommandEventArgs
	: public EventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	ExecuteRoutedCommandEventArgs(const Variant& parameter) : Parameter(parameter) {}
	virtual ~ExecuteRoutedCommandEventArgs() {}

public:
	const Variant&	Parameter;
};


} // namespace GUI
} // namespace Lumino
