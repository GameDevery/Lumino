
#include "../Internal.h"
#include <Lumino/Input/InputController.h>
#include "InputManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// InputController
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputController::InputController(detail::InputManager* manager)
	: m_manager(manager)
	, m_repeatIntervalStart(20)	// TODO �v�����B���Ԃ̕������������H
	, m_repeatIntervalStep(5)
{
	m_inputStateForAny.current = 0;
	m_inputStateForAny.state = 0;
	m_inputStateForAny.ref = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputController::~InputController()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool InputController::IsPressed(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return (state->state > 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool InputController::IsTriggered(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return (state->state == 1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool InputController::IsOffTriggered(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return (state->state == -1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool InputController::IsRepeated(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	int s = state->state;
	return ((s == 1) || (s > m_repeatIntervalStart && (s % m_repeatIntervalStep) == 0));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float InputController::GetAxisValue(const StringRef& bindingName) const
{
	auto* state = m_inputStatus.Find(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return state->current;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputController::AddBinding(InputBinding* binding)
{
	m_bindings.Add(RefPtr<InputBinding>(binding));

	auto* state = m_inputStatus.Find(binding->GetName());
	if (state == nullptr)
	{
		InputState newState;
		newState.current = 0;
		newState.state = 0;
		newState.ref = 1;
		m_inputStatus.Add(binding->GetName(), newState);
	}
	else
	{
		state->ref++;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputController::RemoveBinding(InputBinding* binding)
{
	m_bindings.Remove(RefPtr<InputBinding>(binding));

	auto* state = m_inputStatus.Find(binding->GetName());
	state->ref--;
	if (state->ref <= 0) {
		m_inputStatus.Remove(binding->GetName());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputController::ClearBindings()
{
	Array<RefPtr<InputBinding>> list = m_bindings;
	for (int i = list.GetCount() - 1; i >= 0; ++i)	// ��납��񂵂�����������ƍ폜�̌���������
	{
		RemoveBinding(list[i]);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputController::SetRepeatInterval(int start, int step)
{
	m_repeatIntervalStart = start;
	m_repeatIntervalStep = step;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputController::UpdateFrame()
{
	//SortedArray<String, InputState>::iterator itr = m_inputStatus.begin();
	//SortedArray<String, InputState>::iterator end = m_inputStatus.end();
	//for (; itr != end; ++itr) {
	//	(*itr).second.current = 0;
	//}
	for (auto& state : m_inputStatus) {
		state.second.current = 0;
	}
	m_inputStateForAny.current = 0;

	// m_inputStatus �Ɍ��݂̓��͒l��W�J����
	for (auto& binding : m_bindings)
	{
		float v = m_manager->GetVirtualButtonState(binding->GetDeviceInputSource(), true, true);
		InputState* state = m_inputStatus.Find(binding->GetName());
		if (state != nullptr)
		{
			v = (binding->IsNegativeValue()) ? -v : v;
			if (v >= binding->GetMinValidMThreshold()) {
				state->current = std::max(state->current, v);	// Binding ���d�������Ƃ��A�Ƃ肠�����傫�������g��
			}
		}
	}

	// ���݂̓��͒l�����Ԃ�J�ڂ�����
	for (auto& state : m_inputStatus)
	{
		UpdateOneInputState(&state.second);

		if (state.second.current > 0.0f)
		{
			m_inputStateForAny.current = 1.0f;
		}
	}
	UpdateOneInputState(&m_inputStateForAny);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputController::UpdateOneInputState(InputState* state)
{
	if (state->current > 0.0f) {
		state->state++;	// ������Ă�Ԃ͖��t���[���C���N�������g����
	}
	else
	{
		if (state->state > 0.0f) {
			state->state = -1;	// �����ꂽ�u�Ԃ̃t���[��
		}
		else {
			state->state = 0;		// �����ꂽ�u�Ԃ̎��̃t���[��
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const InputController::InputState* InputController::LockupState(const StringRef& bindingName) const
{
	if (bindingName.IsEmpty())
	{
		return &m_inputStateForAny;
	}
	return m_inputStatus.Find(bindingName);
}

LN_NAMESPACE_END
