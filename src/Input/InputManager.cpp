/*
	[2015/12/5]
		�|�[�����O�̓��͔���͂��܂܂łǂ���ɂ���B
		1�t���[���̊Ԃ�ON/OFF�������ɗ����Ƃ��ɂ� ON �ɂ���悤�ɂ��ł��邯�ǁA
		���Ⴀ�t��OFF�ɂ������Ƃ��͂ǂ�����́H�Ƃ��l���Ȃ�����΂Ȃ�Ȃ����Ƃ�����Ƃ��肻���B
		�i�قƂ�ǂ̏ꍇ��ON�������낤���AON�����͑Ή����Ă����������H������ɂ��Ă���Łj
		ON/OFF�������ɗ��Č��ʂ�OFF�ɂ����Ȃ�Ȃ��̂��C���Ȃ�C�x���g���n���h�����O����B

	[2015/12/5]
		�L�[�{�[�h�ƃ}�E�X�̓��͂̓E�B���h�E�C�x���g��������������Ǝv���B
		GUI ���쒆�ɃL�����N�^�[���ړ�����̂��ǂ��Ȃ����B

	[2015/12/5]
		�C�x���g�͏㑤�� Manager �ɑ����Ă��炤�B
		InputManager ������ MainWindow �Ƀ��X�i�[��o�^����ƁA��X�D��x��������Â炭�Ȃ邩���B
		(�܂������N���X�Ȃ̂łǂ��ύX���Ă��ǂ����ǁE�E�E)
*/
#include "Internal.h"
#include "InputDriver.h"
#include "InputManager.h"
#ifdef LN_OS_WIN32
	#include "Win32InputDriver.h"
#endif
#include <Lumino/Input/Input.h>
#include <Lumino/Input/InputBinding.h>
#include <Lumino/Input/InputController.h>

LN_NAMESPACE_BEGIN
namespace detail
{

InputManager* GetInputManager(InputManager* priority)
{
	if (priority != NULL) return priority;
	return InputManager::Instance;
}

//=============================================================================
// InputManager
//=============================================================================

InputManager* InputManager::Instance = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::InputManager()
	: m_inputDriver(nullptr)
	, m_defaultVirtualPads{}
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::~InputManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::Initialize(const Settings& settings)
{
#ifdef LN_OS_WIN32
	RefPtr<Win32InputDriver> driver(LN_NEW Win32InputDriver());
	driver->Initialize(PlatformSupport::GetWindowHandle(settings.mainWindow));
	m_inputDriver = driver;
#endif

	auto pad = RefPtr<InputController>::Construct(this);
	m_defaultVirtualPads[0] = pad;
	m_defaultVirtualPads[0]->AddRef();

	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Left, Key::Left));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Right, Key::Right));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Up, Key::Up));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Down, Key::Down));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Ok, Key::Z));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Cancel, Key::X));

	if (Instance == nullptr) {
		Instance = this;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::Finalize()
{
	for (auto* pad : m_defaultVirtualPads) {
		LN_SAFE_RELEASE(pad);
	}

	if (m_inputDriver != nullptr) {
		LN_SAFE_RELEASE(m_inputDriver);
	}

	if (Instance == this) {
		Instance = nullptr;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
	for (auto* pad : m_defaultVirtualPads)
	{
		if (pad != nullptr) {
			pad->UpdateFrame();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::OnEvent(const PlatformEventArgs& e)
{
	if (m_inputDriver != nullptr) {
		m_inputDriver->OnEvent(e);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float InputManager::GetVirtualButtonState(const detail::DeviceInputSource& input, bool keyboard, bool mouse)
{
	// �L�[�{�[�h
	if (input.id & detail::DeviceInputSource::KeyboardFlag)
	{
		uint32_t k = input.id & detail::DeviceInputSource::ValumeMask;
		return m_inputDriver->GetKeyState((Key)k) ? 1.0f : 0.0f;
	}
	// �}�E�X
	if (input.id & detail::DeviceInputSource::MouseFlag)
	{
		uint32_t k = input.id & detail::DeviceInputSource::ValumeMask;
		return m_inputDriver->GetMouseState((MouseButton::enum_type)k) ? 1.0f : 0.0f;
	}
	// �W���C�X�e�B�b�N - �{�^��
	if (input.id & detail::DeviceInputSource::JoystickButtonFlag)
	{
		uint32_t number = (input.id & detail::DeviceInputSource::JoystickNumberMask) >> 12;
		LN_THROW(0, NotImplementedException);
	}
	// �W���C�X�e�B�b�N - ��
	if (input.id & detail::DeviceInputSource::JoystickAxisFlag)
	{
		LN_THROW(0, NotImplementedException);
	}
	// �W���C�X�e�B�b�N - POV
	if (input.id & detail::DeviceInputSource::JoystickPovFlag)
	{
		LN_THROW(0, NotImplementedException);
	}
	return 0.0f;
}

} // namespace detail
LN_NAMESPACE_END

#if 0
namespace Lumino
{
namespace Input
{
namespace Private
{

//=============================================================================
// InputManager
//=============================================================================
//
//public:
//	static InputManager& GetInstance();
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//InputManager& InputManager::GetInstance()
//{
//	static InputManager instance;
//	return instance;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager* InputManager::Create(void* hWnd)
{
	return LN_NEW InputManager(hWnd);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::InputManager(void* hWnd)
	: m_inputDevice(NULL)
	, m_mouse(NULL)
	, m_keyboard(NULL)
{
#ifdef _WIN32
	m_inputDevice = LN_NEW Win32InputDevice((HWND)hWnd);
#endif

	m_mouse = LN_NEW Mouse(this);
	m_keyboard = LN_NEW Keyboard(this);

	for (int i = 0; i < MaxJoypads; ++i) {
		m_joypadList.Add(LN_NEW Joypad(this));
	}

	RefreshDevices();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::~InputManager()
{
	LN_SAFE_RELEASE(m_mouse);
	LN_SAFE_RELEASE(m_keyboard);

	LN_FOREACH(Joypad* j, m_joypadList) {
		j->Release();
	}
	m_joypadList.Clear();

	LN_SAFE_RELEASE(m_inputDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Mouse* InputManager::GetMouse() const
{
	return m_mouse;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
	m_mouse->UpdateFrame();
	m_keyboard->UpdateFrame();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::RefreshDevices()
{
	// �F���ς݃W���C�p�b�h�̔ԍ�������U���Ă���
	int joyCount = m_inputDevice->GetJoypadCount();
	for (int i = 0; i < MaxJoypads; ++i)
	{
		if (i < joyCount) {
			m_joypadList[i]->SetJoypadNumber(i);
		}
		else {
			m_joypadList[i]->SetJoypadNumber(-1);
		}
	}
}

} // namespace Private
} // namespace Input
} // namespace Lumino
#endif

