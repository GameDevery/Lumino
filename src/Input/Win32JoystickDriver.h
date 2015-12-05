
#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define INITGUID
#include <dinput.h>
#include <xinput.h>
#include <Lumino/Base/DllLoader.h>
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

/// �W���C�X�e�B�b�N1��
class Win32JoystickDriver
    : public RefObject
{
public:

	/// �X�e�B�b�N�̏�Ԃ�\���\����
	struct AnalogState
	{
		int32_t	X1;
		int32_t	Y1;
		int32_t	X2;
		int32_t	Y2;
	};

public:
	Win32JoystickDriver();
	virtual ~Win32JoystickDriver();

public:

	/// ������ (xinputNumber �� XInput �p�R���g���[���̊��蓖�Ĕԍ��BXInput �ȊO�̂��̂� -1 ��n������)
	void Initialize(IDirectInputDevice8* device, HWND hwnd, int xinputNumber, bool forcefeedback);

	/// ���
	void Dispose();

	/// �W���C�p�b�h�̏�Ԃ��擾����
	void GetJoypadDeviceState(JoypadDeviceState* state);

	/// XInput �f�o�C�X���𔻒肷��
	bool IsXInputDevice() const { return mXInputNo >= 0; }

	/// �W���C�p�b�h�̐U�����J�n����
	void StartVibration(int power, int time);

	/// �W���C�p�b�h�̐U�����~����
	void StopVibration();

	/// �t���[���X�V
	void Update();

private:

	/// �W���C�X�e�B�b�N�̎���񋓂���֐�
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID thisDevice);

public:

	static const int AXIS_RANGE = 1000;							///< DirectInput �ɐݒ肷�鎲�͈̔� ( -1000�`1000 )
	static const int MAX_BUTTON_NUM = JoypadDeviceState::MaxJoypadButtons;	///< �{�^���̐�
	static const int MAX_AXIS_NUM = JoypadDeviceState::MaxJoypadButtons;			///< ���̐�
	

private:
	IDirectInputDevice8*	mDevice;

	IDirectInputEffect*     mDeviceEffect;
	DWORD                   mDIAxes[2];
	LONG                    mDIDirection[2];
	DIPERIODIC              mDIPeriodic;
	DIENVELOPE              mDIEnvelope;
	DIEFFECT                mDIEffect;
	uint32_t				mVibrationStartTime;
	uint32_t				mVibrationTime;

	float                   mPlayRange;
	int                     mXInputNo;
};

/// XInput DLL ���W���[��
class XInputModule
{
public:

	/// ������
	static void Initialize();

public:

	typedef DWORD(WINAPI *MD_XInputGetState)(
		DWORD         dwUserIndex,
		XINPUT_STATE* pState
		);

	typedef DWORD(WINAPI *MD_XInputSetState)(
		DWORD             dwUserIndex,
		XINPUT_VIBRATION* pVibration
		);

	static MD_XInputGetState   XInputGetState;
	static MD_XInputSetState   XInputSetState;

private:
	static DllLoader			m_XInputModule;   ///< XINPUT_DLL ("XInput�`.dll")
};

LN_NAMESPACE_END
