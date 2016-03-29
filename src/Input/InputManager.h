
#pragma once
#include <array>
#include <Lumino/Platform/EventArgs.h>
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN
class InputDriver;
namespace detail
{

class InputManager
	: public RefObject
{
public:
	static InputManager* Instance;

	static const int MaxJoypads = 4;

	struct Settings
	{
		PlatformWindow*	mainWindow;
	};

public:
	InputManager();
	virtual ~InputManager();

	void Initialize(const Settings& settings);
	void Finalize();

	InputController* GetVirtualPad(int index) { return m_defaultVirtualPads[index]; }

	/// ���͏��� 1 �t���[�����X�V
	void UpdateFrame();

	//InputDevice* GetInputDevice() { return m_inputDevice; }
	//int GetButtonRepertFirstFrameCount() const { return 20; }
	//int GetButtonRepertIntervalFrameCount()const { return 5; }
	//const Matrix& GetMouseTransform() const { return m_mouseTransform; }

	void OnEvent(const PlatformEventArgs& e);


	float GetVirtualButtonState(const detail::DeviceInputSource& input, bool keyboard, bool mouse);

private:
	void RefreshDevices();

private:
	static const int MaxVirtualPads = 1;
	InputDriver*							m_inputDriver;
	std::array<InputController*, MaxVirtualPads>	m_defaultVirtualPads;
	//Matrix				m_mouseTransform;
	//Mouse*				m_mouse;
	//Keyboard*			m_keyboard;
	//ArrayList<Joypad*>	m_joypadList;
};

} // namespace detail
LN_NAMESPACE_END
