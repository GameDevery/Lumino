
#pragma once
#include "../Internal.h"
#include <algorithm>
#include "Win32JoystickDriver.h"

LN_NAMESPACE_BEGIN

GUID GUID_SINE = { 0x13541C23, 0x8E33, 0x11D0, 0x9A, 0xD0, 0x00, 0xA0, 0xC9, 0xA0, 0x6E, 0x35 };

// �W���C�X�e�B�b�N�� GUID
GUID GUIDDIJOYSTICK[8] =
{
//	Data1		Data2	Data3	Data4
	0xa36d02e0,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e1,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f5,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e3,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
} ;

// �W���C�X�e�B�b�N�̃f�o�C�X�I�u�W�F�N�g�f�[�^
DIOBJECTDATAFORMAT C_ODFDIJOYSTICK[44] =
{
//	pguid				dwOfs	dwType		dwFlags
	&GUIDDIJOYSTICK[0],	0x00,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[1],	0x04,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[2],	0x08,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[3],	0x0c,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[4],	0x10,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[5],	0x14,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[6],	0x18,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[6],	0x1c,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[7],	0x20,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x24,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x28,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x2c,	0x80ffff10,	0x000,
	NULL,				0x30,	0x80ffff0c,	0x000,
	NULL,				0x31,	0x80ffff0c,	0x000,
	NULL,				0x32,	0x80ffff0c,	0x000,
	NULL,				0x33,	0x80ffff0c,	0x000,
	NULL,				0x34,	0x80ffff0c,	0x000,
	NULL,				0x35,	0x80ffff0c,	0x000,
	NULL,				0x36,	0x80ffff0c,	0x000,
	NULL,				0x37,	0x80ffff0c,	0x000,
	NULL,				0x38,	0x80ffff0c,	0x000,
	NULL,				0x39,	0x80ffff0c,	0x000,
	NULL,				0x3a,	0x80ffff0c,	0x000,
	NULL,				0x3b,	0x80ffff0c,	0x000,
	NULL,				0x3c,	0x80ffff0c,	0x000,
	NULL,				0x3d,	0x80ffff0c,	0x000,
	NULL,				0x3e,	0x80ffff0c,	0x000,
	NULL,				0x3f,	0x80ffff0c,	0x000,
	NULL,				0x40,	0x80ffff0c,	0x000,
	NULL,				0x41,	0x80ffff0c,	0x000,
	NULL,				0x42,	0x80ffff0c,	0x000,
	NULL,				0x43,	0x80ffff0c,	0x000,
	NULL,				0x44,	0x80ffff0c,	0x000,
	NULL,				0x45,	0x80ffff0c,	0x000,
	NULL,				0x46,	0x80ffff0c,	0x000,
	NULL,				0x47,	0x80ffff0c,	0x000,
	NULL,				0x48,	0x80ffff0c,	0x000,
	NULL,				0x49,	0x80ffff0c,	0x000,
	NULL,				0x4a,	0x80ffff0c,	0x000,
	NULL,				0x4b,	0x80ffff0c,	0x000,
	NULL,				0x4c,	0x80ffff0c,	0x000,
	NULL,				0x4d,	0x80ffff0c,	0x000,
	NULL,				0x4e,	0x80ffff0c,	0x000,
	NULL,				0x4f,	0x80ffff0c,	0x000,
};

// �W���C�X�e�B�b�N�̃f�o�C�X�f�[�^�t�H�[�}�b�g
const DIDATAFORMAT c_dfDIJoystick =
{
	24, 	// dwSize
	16,		// dwObjSize
	0x01,	// dwFlags
	80,		// dwDataSize
	44,		// dwNumObjs
	C_ODFDIJOYSTICK, // rgodf
};

//=============================================================================
// Win32JoystickDriver
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32JoystickDriver::Win32JoystickDriver()
	: mDevice(NULL)
	, mDeviceEffect(NULL)
	, mVibrationStartTime(0)
	, mVibrationTime(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32JoystickDriver::~Win32JoystickDriver()
{
	Dispose();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32JoystickDriver::Initialize( IDirectInputDevice8* device, HWND hwnd, int xinputNumber, bool forcefeedback )
{
	XInputModule::Initialize();
	Dispose();

	mDevice = device;
	mXInputNo = xinputNumber;

	//memset(mButtonState, 0, sizeof(mButtonState));
	//memset(mAxisState, 0, sizeof(mAxisState));

	if (!IsXInputDevice())
	{
		// �W���C�X�e�B�b�N�Ƃ��ăf�[�^�t�H�[�}�b�g��ݒ�
		//hr = mDevice->SetDataFormat( &c_dfDIJoystick2 );	// �����Ƃ����ȏ�񂪂ق����Ƃ�
		LN_COMCALL(mDevice->SetDataFormat(&c_dfDIJoystick));

		// �t�H�A�O���E���h�E�r�����[�h
		LN_COMCALL(mDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE));

		// �R�[���o�b�N�֐����g���Ċe���̃��[�h��ݒ�
		LN_COMCALL(mDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS));

		if (forcefeedback)
		{
			// �G�t�F�N�g�����ݒ�
			ZeroMemory(&mDIPeriodic, sizeof(mDIPeriodic));
			mDIPeriodic.dwMagnitude = DI_FFNOMINALMAX; // �G�t�F�N�g�̋���(0�`10,000)
			mDIPeriodic.lOffset = 0;
			mDIPeriodic.dwPhase = 0;
			mDIPeriodic.dwPeriod = (DWORD)(1.5 * DI_SECONDS); // �G�t�F�N�g�̊Ԋu

			// �G���x���[�v
			ZeroMemory(&mDIEnvelope, sizeof(mDIEnvelope));
			mDIEnvelope.dwSize = sizeof(DIENVELOPE);
			mDIEnvelope.dwAttackLevel = 0;
			mDIEnvelope.dwAttackTime = (DWORD)(0.5 * DI_SECONDS);
			mDIEnvelope.dwFadeLevel = 0;
			mDIEnvelope.dwFadeTime = (DWORD)(1.0 * DI_SECONDS);

			// �G�t�F�N�g��
			mDIAxes[0] = DIJOFS_X;
			mDIAxes[1] = DIJOFS_Y;

			// �G�t�F�N�g����
			mDIDirection[0] = 0;
			mDIDirection[1] = 0;

			// �U���G�t�F�N�g�ݒ�
			ZeroMemory(&mDIEffect, sizeof(mDIEffect));
			mDIEffect.dwSize = sizeof(mDIEffect);
			mDIEffect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;//DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
			mDIEffect.dwDuration = INFINITE; // �G�t�F�N�g�p������
			mDIEffect.dwSamplePeriod = 0;
			mDIEffect.dwGain = DI_FFNOMINALMAX;
			mDIEffect.dwTriggerButton = DIEB_NOTRIGGER;
			mDIEffect.dwTriggerRepeatInterval = 0;
			mDIEffect.cAxes = 2;
			mDIEffect.rgdwAxes = mDIAxes;
			mDIEffect.rglDirection = mDIDirection;
			mDIEffect.lpEnvelope = &mDIEnvelope; // �G���x���[�v�\����
			mDIEffect.cbTypeSpecificParams = sizeof(mDIPeriodic); // �G�t�F�N�g�����\���̂̃T�C�Y
			mDIEffect.lpvTypeSpecificParams = &mDIPeriodic; // �G�t�F�N�g�����\����

			// �G�t�F�N�g����
			LN_COMCALL(mDevice->CreateEffect(GUID_SINE, &mDIEffect, &mDeviceEffect, NULL));

		}

		mDevice->Acquire();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32JoystickDriver::Dispose()
{
    if ( mDeviceEffect )
    {
        mDeviceEffect->Unload();
        LN_SAFE_RELEASE( mDeviceEffect );
    }

    // ���͂��~���ăW���C�X�e�B�b�N�f�o�C�X�����
	if ( mDevice )
	{
		mDevice->Unacquire();
		LN_SAFE_RELEASE(mDevice);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32JoystickDriver::GetJoypadDeviceState(JoypadDeviceState* joyState)
{
	// XInput
	if (IsXInputDevice())
	{
		XINPUT_STATE state;
		if (XInputModule::XInputGetState(mXInputNo, &state) == ERROR_SUCCESS)
		{
			// �{�^��
			joyState->Buttons[0] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
			joyState->Buttons[1] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
			joyState->Buttons[2] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
			joyState->Buttons[3] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
			joyState->Buttons[4] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
			joyState->Buttons[5] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
			joyState->Buttons[6] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
			joyState->Buttons[7] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
			joyState->Buttons[8] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
			joyState->Buttons[9] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;

			// ��
			float inv_s16 = 1.0f / 32767.f;
			float inv_u8 = 1.0f / 255.f;
			joyState->Axes[0] = inv_s16 * static_cast<float>(state.Gamepad.sThumbLX);
			joyState->Axes[1] = inv_s16 * static_cast<float>(state.Gamepad.sThumbLY);
			joyState->Axes[2] = inv_s16 * static_cast<float>(state.Gamepad.sThumbRX);
			joyState->Axes[3] = inv_s16 * static_cast<float>(state.Gamepad.sThumbRY);
			joyState->Axes[4] = inv_u8  * static_cast<float>(state.Gamepad.bLeftTrigger);
			joyState->Axes[5] = inv_u8  * static_cast<float>(state.Gamepad.bRightTrigger);
			joyState->Axes[6] = 0.0f;
			joyState->Axes[7] = 0.0f;

			// ���͈̔͂� -32768 �` 32767�B32767.f �Ŋ��邾������ -������ 1.0f ���������Ȃ�̂ŁA���̕ӂ̏C��
			for (int i = 0; i < 4; ++i)
			{
				joyState->Axes[i] = std::max(joyState->Axes[i], -1.0f);
			}

			// POV
			joyState->POV =
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? PovDirFlags::Left : 0 |
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? PovDirFlags::Right : 0 |
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? PovDirFlags::Up : 0 |
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? PovDirFlags::Down : 0;

			/*
			for ( int i = 0; i < 10; ++i )
			{
			printf( "%d ", mButtonState[ i ] );
			}
			printf( "b:%x  pov:%x\n", state.Gamepad.wButtons, mPOVState );

			for ( int i = 0; i < 6; ++i )
			{
			printf( "%f ", mAxisState[ i ] );
			}
			printf( "\n" );
			*/
		}
	}
	// DirectInput
	else
	{
		DIJOYSTATE state;
		HRESULT hr = mDevice->GetDeviceState(sizeof(DIJOYSTATE), &state);
		if (SUCCEEDED(hr))
		{
			// �{�^��
			for (int i = 0; i < JoypadDeviceState::MaxJoypadButtons; ++i)
			{
				joyState->Buttons[i] = (state.rgbButtons[i] & 0x80) != 0x00;
			}

			// ��
			static const float r_range = 1.0f / AXIS_RANGE;
			joyState->Axes[0] = r_range * state.lX;
			joyState->Axes[1] = r_range * state.lY;
			joyState->Axes[2] = r_range * state.lZ;
			joyState->Axes[3] = r_range * state.lRz;
			joyState->Axes[4] = r_range * state.lRx;
			joyState->Axes[5] = r_range * state.lRy;
			joyState->Axes[6] = 0.0f;
			joyState->Axes[7] = 0.0f;

			// POV
			uint32_t pov = state.rgdwPOV[0];
			int dx, dy;
			if (LOWORD(pov) != 0xffff)
			{
				if (pov > 0 && pov < 18000) {
					dx = 1; 	        // �E
				}
				else if (pov > 18000) {
					dx = -1;
				}		        // ��
				else {
					dx = 0;
				}

				if (pov > 9000 && pov < 27000) {
					dy = 1;
				}	    // ��
				else if (pov < 9000 || pov > 27000) {
					dy = -1;
				}	// ��
				else {
					dy = 0;
				}
			}
			else
			{
				dx = dy = 0;
			}
			joyState->POV =
				((dx == 1) ? PovDirFlags::Right : 0) |
				((dx == -1) ? PovDirFlags::Left : 0) |
				((dy == 1) ? PovDirFlags::Down : 0) |
				((dy == -1) ? PovDirFlags::Up : 0);

			/*
			printf( "%f %f %f %f %f %f\n",
			mAxisState[ 0 ],
			mAxisState[ 1 ],
			mAxisState[ 2 ],
			mAxisState[ 3 ],
			mAxisState[ 4 ],
			mAxisState[ 5 ] );
			*/
		}
		else
		{
			mDevice->Acquire();
			memset(joyState, 0, sizeof(JoypadDeviceState));
		}
	}

	// ���̗V�ђׂ�
	//for (int i = 0; i < MAX_AXIS_NUM; ++i)
	//{
	//	if (abs(mAxisState[i]) < mPlayRange)
	//	{
	//		mAxisState[i] = 0.0f;
	//	}
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32JoystickDriver::StartVibration( int power, int time )
{
	if (IsXInputDevice())
	{
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = power * 65535 / 1000;
		vibration.wRightMotorSpeed = power * 65535 / 1000;
		XInputModule::XInputSetState(mXInputNo, &vibration);
	}
	else
	{
		if (mDeviceEffect)
		{
			mDIPeriodic.dwMagnitude = 10 * std::max(0, std::min(power, 1000));

			if (time > 0)
			{
				mDIPeriodic.dwPeriod = std::max(0, time) * 10000; // ���ʂ� 1000 �����ǁA�Ȃ񂩓r���ŐU������u�キ�Ȃ�
				//mDIEffect.dwDuration = mDIPeriodic.dwPeriod = time_ * 1000;
			}

			mDeviceEffect->SetParameters(&mDIEffect, DIEP_TYPESPECIFICPARAMS | DIEP_START);
			mDeviceEffect->Start(1, 0);
		}
	}

	if (time > 0)
	{
		mVibrationTime = time;
		mVibrationStartTime = ::GetTickCount();
	}
	else
	{
		mVibrationTime = 0;
		mVibrationStartTime = 0;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32JoystickDriver::StopVibration()
{
	if (IsXInputDevice())
	{
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
		XInputModule::XInputSetState(mXInputNo, &vibration);
	}
	else
	{
		if (mDeviceEffect)
		{
			mDeviceEffect->Stop();
			mVibrationStartTime = 0;
			mVibrationTime = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32JoystickDriver::Update()
{
	// �U���I���̊Ď�
	if (mVibrationStartTime > 0)
	{
		if (::GetTickCount() >= mVibrationStartTime + mVibrationTime)
		{
			StopVibration();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CALLBACK Win32JoystickDriver::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID thisDevice )
{
	Win32JoystickDriver* self = static_cast<Win32JoystickDriver*>( thisDevice );

	// ���̒l�͈̔͂�ݒ�
	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof( diprg ) );
	diprg.diph.dwSize		= sizeof( diprg );
	diprg.diph.dwHeaderSize	= sizeof( diprg.diph ); 
	diprg.diph.dwObj		= lpddoi->dwType;
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.lMin				= -AXIS_RANGE;
	diprg.lMax				= AXIS_RANGE;
	HRESULT hr = self->mDevice->SetProperty( DIPROP_RANGE, &diprg.diph );
	if ( FAILED( hr ) ) { 
		return DIENUM_STOP; 
	}

	return DIENUM_CONTINUE;
}

//=============================================================================
// XInputModule
//=============================================================================

XInputModule::MD_XInputGetState	XInputModule::XInputGetState;
XInputModule::MD_XInputSetState	XInputModule::XInputSetState;
DllLoader						XInputModule::m_XInputModule;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void XInputModule::Initialize()
{
	if (!XInputGetState)
	{
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)	// XINPUT_DLL �� "xinput1_4.dll" �Ȃ̂ɁA������Ȃ����Ƃ��������B 
		// ����΂��ĒT��
		if (DllLoader::Exists(XINPUT_DLL)) {
			m_XInputModule.Load(XINPUT_DLL);
		}
		else if (DllLoader::Exists(_T("xinput1_3.dll"))) {
			m_XInputModule.Load(_T("xinput1_3.dll"));
		}
		else if (DllLoader::Exists(_T("xinput1_2.dll"))) {
			m_XInputModule.Load(_T("xinput1_2.dll"));
		}
		else if (DllLoader::Exists(_T("xinput1_1.dll"))) {
			m_XInputModule.Load(_T("xinput1_1.dll"));
		}
		else {
			LN_THROW(0, FileNotFoundException);
		}
#else
		m_XInputModule.Load(XINPUT_DLL);
#endif
		XInputGetState = reinterpret_cast<MD_XInputGetState>(m_XInputModule.GetProcAddress("XInputGetState"));
		XInputSetState = reinterpret_cast<MD_XInputSetState>(m_XInputModule.GetProcAddress("XInputSetState"));
	}
}

LN_NAMESPACE_END
