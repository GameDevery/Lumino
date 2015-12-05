
#include "../Internal.h"
#include <wbemidl.h>
#include <oleauto.h>
#include "Win32InputDriver.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
LN_NAMESPACE_BEGIN

//=============================================================================
// Win32InputDriver
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32InputDriver::Win32InputDriver()
	: m_hWnd(nullptr)
	, m_directInput(nullptr)
	, m_XInputDeviceCount(0)
	, m_comInited(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32InputDriver::~Win32InputDriver()
{
	Finalize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	// COM ������
	if (SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		// �G���[�ɂ͂��Ȃ��B�ʂ̐ݒ�� COM ���������ς݂������肷��Ǝ��s���邱�Ƃ����邪�ACOM ���͎̂g����悤�ɂȂ��Ă���
		m_comInited = true;
	}

	// DirectInput ������
	LN_COMCALL(CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_ALL, IID_IDirectInput8, (void**)&m_directInput));
	LN_COMCALL(m_directInput->Initialize(::GetModuleHandle(NULL), DIRECTINPUT_VERSION));

	RefreshDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::Finalize()
{
	ReleaseDevice();
	LN_SAFE_RELEASE(m_directInput);

	if (m_comInited)
	{
		::CoUninitialize();
		m_comInited = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Win32InputDriver::GetJoypadCount()
{
	return m_joypadList.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::GetJoypadState(int joypadNumber, JoypadDeviceState* state)
{
	m_joypadList[joypadNumber]->GetJoypadDeviceState(state);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::StartVibration(int joypadNumber, int power, int time)
{
	m_joypadList[joypadNumber]->StartVibration(power, time);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::StopVibration(int joypadNumber)
{
	m_joypadList[joypadNumber]->StopVibration();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::RefreshDevice()
{
	ReleaseDevice();

	// �ڑ��ς݂̃Q�[���R���g���[���[�f�o�C�X��񋓂���
	m_XInputDeviceCount = 0;
	LN_COMCALL(
		m_directInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,		// XBox �F��������ۂ�
										//DI8DEVTYPE_JOYSTICK,		// ���̔F��������ۂ�
			EnumJoysticksCallback,
			this,
			DIEDFL_ATTACHEDONLY));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::ReleaseDevice()
{
	for (Win32JoystickDriver* joy : m_joypadList) {
		joy->Release();
	}
	m_joypadList.Clear();
	m_XInputDeviceCount = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CALLBACK Win32InputDriver::EnumJoysticksCallback(const DIDEVICEINSTANCE* instance, VOID* thisInputDevice)
{
	Win32InputDriver* self = static_cast<Win32InputDriver*>(thisInputDevice);
	self->CreateJoysticksDevice(instance);
	return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32InputDriver::CreateJoysticksDevice(const DIDEVICEINSTANCE* instance)
{
	if (IsXInputDevice(&instance->guidProduct))
	{
		// �f�o�C�X�̏����擾���邽�߁A��x�f�o�C�X�����
		IDirectInputDevice8* device = NULL;
		HRESULT hr = m_directInput->CreateDevice(instance->guidInstance, &device, NULL);
		if SUCCEEDED(hr)
		{
			DIDEVICEINSTANCE di = { 0 };
			device->GetDeviceInfo(&di);
			//printf( "XInput Device  No:%d Name:%s\n", mXInputNo, di.tszInstanceName );
		}
		LN_SAFE_RELEASE(device);

		Win32JoystickDriver* joypad = LN_NEW Win32JoystickDriver();
		joypad->Initialize(NULL, m_hWnd, m_XInputDeviceCount, true);
		m_joypadList.Add(joypad);
		++m_XInputDeviceCount;
	}
	else
	{
		IDirectInputDevice8* device = NULL;
		HRESULT hr = m_directInput->CreateDevice(instance->guidInstance, &device, NULL);
		if (SUCCEEDED(hr))
		{
			// �f�o�C�X�̏��擾
			DIDEVCAPS caps;
			memset(&caps, 0, sizeof(caps));
			caps.dwSize = sizeof(caps);
			if (SUCCEEDED(device->GetCapabilities(&caps)))
			{
				//DXST_ERRORLOGFMT_ADDA(( "�f�o�C�X�̓o�^��:%s" , State.tszInstanceName )) ;
				//DXST_ERRORLOGFMT_ADDA(( "�f�o�C�X�̐��i�o�^��:%s" , State.tszProductName )) ;
				Win32JoystickDriver* joypad = LN_NEW Win32JoystickDriver();
				joypad->Initialize(device, m_hWnd, -1, (caps.dwFlags & DIDC_FORCEFEEDBACK) != 0);
				m_joypadList.Add(joypad);
			}
		}
	}
	/*
	// �f�o�C�X�̃^�C�v���擾���� ( ���ʃo�C�g���^�C�v )
	lnU32 type = instance_->dwDevType & 0xff;
	// �W���C�X�e�B�b�N�̏ꍇ
	// (DI8DEVTYPE_JOYSTICK �������ƃq�b�g���Ȃ��R���g���[��������̂� DI8DEVTYPE_GAMEPAD���`�F�b�N)
	if ( type == DI8DEVTYPE_JOYSTICK || type == DI8DEVTYPE_GAMEPAD )
	{
	// �W���C�X�e�B�b�N�̃f�o�C�X�I�u�W�F�N�g���쐬����
	IDirectInputDevice8* device = NULL;
	HRESULT hr = mDirectInput->CreateDevice( instance_->guidInstance, &device, NULL );
	if ( SUCCEEDED( hr ) )
	{
	// �W���C�X�e�B�b�N�̊Ǘ��N���X���쐬����
	mJoypads[ mJoypadNum ] = NEW JoypadDevice();
	mJoypads[ mJoypadNum ]->initialize( device, mWindowHandle );
	++mJoypadNum;
	}
	}
	*/
}

//-------------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
// Unfortunately this information can not be found by just using DirectInput 
//-------------------------------------------------------------------------------
// http://msdn.microsoft.com/ja-jp/library/bb173051(v=vs.85).aspx
#pragma warning(disable:4996)

BOOL Win32InputDriver::IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
	IWbemLocator*           pIWbemLocator = NULL;
	IEnumWbemClassObject*   pEnumDevices = NULL;
	IWbemClassObject*       pDevices[20] = { 0 };
	IWbemServices*          pIWbemServices = NULL;
	BSTR                    bstrNamespace = NULL;
	BSTR                    bstrDeviceID = NULL;
	BSTR                    bstrClassName = NULL;
	DWORD                   uReturned = 0;
	bool                    bIsXinputDevice = false;
	UINT                    iDevice = 0;
	VARIANT                 var;
	HRESULT                 hr;

	// CoInit if needed
	hr = CoInitialize(NULL);
	bool bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(
		__uuidof(WbemLocator),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWbemLocator),
		(LPVOID*)&pIWbemLocator);
	if (FAILED(hr) || pIWbemLocator == NULL)
		goto LCleanup;

	bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == NULL) goto LCleanup;
	bstrClassName = SysAllocString(L"Win32_PNPEntity");   if (bstrClassName == NULL) goto LCleanup;
	bstrDeviceID = SysAllocString(L"DeviceID");          if (bstrDeviceID == NULL)  goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer(bstrNamespace, NULL, NULL, 0L,
		0L, NULL, NULL, &pIWbemServices);
	if (FAILED(hr) || pIWbemServices == NULL)
		goto LCleanup;

	// Switch security level to IMPERSONATE. 
	CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, NULL, &pEnumDevices);
	if (FAILED(hr) || pEnumDevices == NULL)
		goto LCleanup;

	// Loop over all devices
	for (;;)
	{
		// Get 20 at a time
		hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
		if (FAILED(hr))
			goto LCleanup;
		if (uReturned == 0)
			break;

		for (iDevice = 0; iDevice<uReturned; iDevice++)
		{
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, NULL, NULL);
			if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != NULL)
			{
				// Check if the device ID contains "IG_".  If it does, then it's an XInput device
				// This information can not be found from DirectInput 
				if (wcsstr(var.bstrVal, L"IG_"))
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
					if (strVid && swscanf(strVid, L"VID_%4X", &dwVid) != 1)
						dwVid = 0;
					WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
					if (strPid && swscanf(strPid, L"PID_%4X", &dwPid) != 1)
						dwPid = 0;

					// Compare the VID/PID to the DInput device
					DWORD dwVidPid = MAKELONG(dwVid, dwPid);
					if (dwVidPid == pGuidProductFromDirectInput->Data1)
					{
						bIsXinputDevice = true;
						goto LCleanup;
					}
				}
			}
			LN_SAFE_RELEASE(pDevices[iDevice]);
		}
	}

LCleanup:
	if (bstrNamespace)
		SysFreeString(bstrNamespace);
	if (bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if (bstrClassName)
		SysFreeString(bstrClassName);
	for (iDevice = 0; iDevice<20; iDevice++)
		LN_SAFE_RELEASE(pDevices[iDevice]);
	LN_SAFE_RELEASE(pEnumDevices);
	LN_SAFE_RELEASE(pIWbemLocator);
	LN_SAFE_RELEASE(pIWbemServices);

	if (bCleanupCOM)
		CoUninitialize();

	return bIsXinputDevice;
}

LN_NAMESPACE_END
