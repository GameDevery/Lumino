
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/RoutedCommand.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// CommandManager
//=============================================================================

CommandManager::TypeContextList	CommandManager::m_typeContextList;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RoutedCommandTypeContext* CommandManager::RegisterCommand(const TypeInfo& type, RoutedCommand* command)
{
	LN_VERIFY_RETURNV(command != NULL, NULL);

	std::shared_ptr<RoutedCommandTypeContext> ptr;
	if (!m_typeContextList.TryGetValue(&type, &ptr))
	{
		// ������Ȃ������̂ŐV�������
		ptr.reset(LN_NEW RoutedCommandTypeContext());
		ptr->Type = &type;
		m_typeContextList.Add(&type, ptr);
	}

	// command ���o�^�ς݂łȂ���Γo�^����
	if (!command->m_registerd) {
		ptr->RoutedCommandList.Add(command);
		command->m_registerd = true;	// �o�^�ς݂ɂ���
	}
	return ptr.get();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CommandManager::CanExecute(UIElement* caller, Command* command, const Variant& parameter)
{
	RoutedCommand* routedCommand = dynamic_cast<RoutedCommand*>(command);
	if (routedCommand != NULL)
	{
		// RoutedEvent �Ƃ��āA���s�ł���R�}���h��T���B
		// UIElement::Handler_CanExecuteRoutedCommandEventArgs() ���Ă΂�āA���̒��ł��� UIElement �������Ă���R�}���h��T���B
		RefPtr<CanExecuteRoutedCommandEventArgs> args(caller->GetManager()->GetEventArgsPool()->Create<CanExecuteRoutedCommandEventArgs>(parameter));
		caller->RaiseEvent(UIElement::CanExecuteRoutedCommandEvent, caller, args);
		return args->CanExecute;
	}
	else {
		// ViewModel �̃��[�U�[�J�X�^�}�C�Y�R�}���h�B���ʂɌĂяo���B
		return command->CanExecute(parameter);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CommandManager::Execute(UIElement* caller, Command* command, const Variant& parameter)
{
	RoutedCommand* routedCommand = dynamic_cast<RoutedCommand*>(command);
	if (routedCommand != NULL)
	{
		// RoutedEvent �Ƃ��āA���s�ł���R�}���h��T���B
		// UIElement::Handler_ExecuteRoutedCommandEventArgs() ���Ă΂�āA���̒��ł��� UIElement �������Ă���R�}���h��T���B
		RefPtr<ExecuteRoutedCommandEventArgs> args(caller->GetManager()->GetEventArgsPool()->Create<ExecuteRoutedCommandEventArgs>(parameter));
		caller->RaiseEvent(UIElement::ExecuteRoutedCommandEvent, caller, args);
	}
	else {
		// ViewModel �̃��[�U�[�J�X�^�}�C�Y�R�}���h�B���ʂɌĂяo���B
		command->Execute(parameter);
	}
}


} // namespace GUI
} // namespace Lumino
