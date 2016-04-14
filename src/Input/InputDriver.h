
#pragma once
#include <array>
#include <Lumino/Platform/PlatformEventArgs.h>
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN

/// �W���C�p�b�h�� POV �̌�����\���t���O
LN_ENUM_FLAGS(PovDirFlags)
{
	None = 0x00,
	Up = 0x01,
	Right = 0x02,
	Down = 0x04,
	Left = 0x08,
};
LN_ENUM_FLAGS_DECLARE(PovDirFlags);

/// �W���C�p�b�h�̏�Ԃ�\���\����
struct JoypadDeviceState
{
	static const int MaxJoypadButtons = 16;    ///< �W���C�p�b�h�̃{�^���̍ő吔
	static const int MaxJoypadAxis = 8;        ///< �W���C�p�b�h�̎��̍ő吔

	float		Axes[MaxJoypadAxis];
	uint32_t	POV;						        ///< POV ��� (POVDir �̑g�ݍ��킹  GLFW�ł͎��Ȃ�)
	uint8_t		Buttons[MaxJoypadButtons];
};

// 
class InputDriver
	: public RefObject
{
public:
	InputDriver();
	virtual ~InputDriver();

	bool GetKeyState(Key key) const { return m_keyStatus[(int)key]; }
	bool GetMouseState(MouseButton button) const { return m_mouseStatus[(int)button]; }
	const Point& GetMousePoint() const { return m_mousePoint; }
	int GetMouseWheel() const { return m_mouseWheel; }
	
	virtual void UpdateFrame();
	virtual void OnEvent(const PlatformEventArgs& e);

private:
	std::array<bool, (int)Key::TERMINATOR>			m_keyStatus;
	std::array<bool, (int)MouseButton::TERMINATOR>	m_mouseStatus;
	Point	m_mousePoint;
	int		m_mouseWheel;
};

LN_NAMESPACE_END
