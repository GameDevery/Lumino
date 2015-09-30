#include <iostream>
#include <LuminoC.h>

#include <Windows.h>
HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // �R���\�[�� �E�B���h�E�̃^�C�g���p�̃o�b�t�@�T�C�Y
	HWND hwndFound;         // �Ăяo�����֕Ԃ����l
	TCHAR pszNewWindowTitle[MY_BUFSIZE];
	// �쐬�����E�B���h�E�̃^�C�g��������܂�
	TCHAR pszOldWindowTitle[MY_BUFSIZE]; // ���̃E�B���h�E�^�C�g��������܂�

	// ���݂̃E�B���h�E�^�C�g�����擾���܂�

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// �Ǝ��ɁA�E�B���h�E�̐V�K�^�C�g�����t�H�[�}�b�g���܂�

	wsprintf(pszNewWindowTitle, _T("%d/%d"),
		GetTickCount(),
		GetCurrentProcessId());

	// ���݂̃E�B���h�E�^�C�g����ύX���܂�

	SetConsoleTitle(pszNewWindowTitle);

	// �E�B���h�E�^�C�g���̃A�b�v�f�[�g���m���Ȃ��̂ɂ����܂�

	Sleep(40);

	// �E�B���h�E�̐V�K�^�C�g����T���ɂ����܂�

	hwndFound = FindWindow(NULL, pszNewWindowTitle);

	// ���̃E�B���h�E�^�C�g���֖߂��܂�

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}

int main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LNResult r;

	//LNConfig_SetUserWindowHandle((intptr_t)GetConsoleHwnd());
	//LNConfig_SetDirectMusicMode(LN_DIRECTMUSICMODE_NORMAL);
	//LNConfig_SetDirectMusicReverbLevel(1.0f);

	//LNConfig_RegisterArchive(_T("D:/Proj/Lumino/build/bindings/C/Test/Debug/audio.lna"), _T("test"));
	LNApplication_InitializeAudio();

	//LNGameAudio_PlayBGM(_T("D:/tmp/ZIGG-ZAGG.mp3"));
	//::Sleep(2000);
	//LNGameAudio_PlayME(_T("D:/Proj/Lumino/bindings/DotNet/Samples/Media/lnme_victory1.ogg"));

	LNVector3 pos = { 2, 0, 0 };

	LNHandle sound1;
	r = LNSound_Create(_T("D:/Proj/Lumino/bindings/Common/Media/water02.wav"), &sound1);
	//const LNChar* str;
	//LNError_GetLastErrorMessage(&str);
	LNSound_Set3DEnabled(sound1, LN_TRUE);
	LNSound_SetEmitterPosition(sound1, &pos);
	LNSound_Play(sound1);

	//LNSound_Stop(sound1);
	//
	getchar();
//	::Sleep(2000);
//LNObject_Release(sound1);
//
	//::Sleep(2000);
	LNApplication_Terminate();
	return 0;
}

