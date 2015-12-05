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
#include "../Internal.h"
#include "InputDriver.h"
#include "InputManager.h"
//#ifdef _WIN32
//	#include "Win32/Win32InputDevice.h"
//#endif
//#include "Mouse.h"
//#include "Keyboard.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// InputManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::InputManager()
	: m_InputDriver(nullptr)
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
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::Finalize()
{
	LN_SAFE_RELEASE(m_InputDriver);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::OnEvent(const Platform::EventArgs& e)
{
}


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

