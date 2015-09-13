#include "DxLib.h"
#include <d3d9.h>
#include <LuminoDxLib.h>
using namespace Lumino;

int PlayerX, PlayerY;
int PlayerGraph;

void DeviceLostFunction(void *Data)
{
	printf("Start LostFunction\n");

	printf("End LostFunction\n");
}
void DeviceRestoreFunction(void *Data)
{
	printf("Start RestoreFunction\n");
	printf("End RestoreFunction\n");
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine, int nCmdShow)
int main()
{
	// ������
	ChangeWindowMode(TRUE);
	SetUseDirect3D9Ex(FALSE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK)) return -1;
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	ApplicationSettings appData;
	appData.GraphicsAPI = Graphics::GraphicsAPI::DirectX9;
	appData.RenderingType = Graphics::RenderingType::Immediate;
	appData.UserMainWindow = GetMainWindowHandle();
	appData.D3D9Device = (IDirect3DDevice9*)GetUseDirect3DDevice9();

	Application::Initialize(appData);

	// �f�o�C�X���X�g���̃R�[���o�b�N�ݒ�(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// TODO: DxLib �A�[�J�C�u����̃��[�h

	// �O���t�B�b�N�̃��[�h
	//PlayerGraph = LoadGraph("Player.bmp");

	// �L�����N�^�[�̏����ʒu���Z�b�g
	PlayerX = 0;
	PlayerY = 0;


	int dummyGraph = MakeGraph(32, 32);

	// ���[�v
	bool isFullScreen = false;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �L�[���͎擾
		int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// ��������Ă������ɐi��
		if (Key & PAD_INPUT_UP) PlayerY -= 3;

		// ���������Ă����牺�ɐi��
		if (Key & PAD_INPUT_DOWN) PlayerY += 3;

		// �E�������Ă�����E�ɐi��
		if (Key & PAD_INPUT_RIGHT) PlayerX += 3;

		// ���������Ă����獶�ɐi��
		if (Key & PAD_INPUT_LEFT) PlayerX -= 3;

		// ��ʂ�����������
		ClearDrawScreen();

		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		DrawGraph(0, 0, dummyGraph, TRUE);

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
		RenderVertex();






		// �v���C���[��`�悷��
		//DrawGraph(PlayerX, PlayerY, PlayerGraph, TRUE);

		//app->Render();


		// DX���C�u�����̐ݒ��߂��B
		RefreshDxLibDirect3DSetting();

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// �t���X�N���[���؂�ւ�
		if (CheckHitKey(KEY_INPUT_F1) && !isFullScreen)
		{
			ChangeWindowMode(FALSE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = true;
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
		}
	}

	DxLib_End();

	Application::Finalize();

#if 0
	// ������
	ChangeWindowMode(TRUE);
	SetUseDirect3D9Ex(FALSE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK)) return -1;
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);


	Application::ConfigData appData;
	appData.GraphicsAPI = Graphics::GraphicsAPI::DirectX9;
	appData.RenderingType = Graphics::RenderingType::Immediate;
	appData.UserMainWindow = GetMainWindowHandle();
	appData.D3D9Device = (IDirect3DDevice9*)GetUseDirect3DDevice9();
	Application* app = Application::Create(appData);


	// �f�o�C�X���X�g���̃R�[���o�b�N�ݒ�(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// TODO: DxLib �A�[�J�C�u����̃��[�h

	// �O���t�B�b�N�̃��[�h
	//PlayerGraph = LoadGraph("Player.bmp");

	// �L�����N�^�[�̏����ʒu���Z�b�g
	PlayerX = 0;
	PlayerY = 0;


	int dummyGraph = MakeGraph(32, 32);

	// ���[�v
	bool isFullScreen = false;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �L�[���͎擾
		int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// ��������Ă������ɐi��
		if (Key & PAD_INPUT_UP) PlayerY -= 3;

		// ���������Ă����牺�ɐi��
		if (Key & PAD_INPUT_DOWN) PlayerY += 3;

		// �E�������Ă�����E�ɐi��
		if (Key & PAD_INPUT_RIGHT) PlayerX += 3;

		// ���������Ă����獶�ɐi��
		if (Key & PAD_INPUT_LEFT) PlayerX -= 3;

		// ��ʂ�����������
		ClearDrawScreen();

		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		DrawGraph(0, 0, dummyGraph, TRUE);

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
		RenderVertex();






		// �v���C���[��`�悷��
		//DrawGraph(PlayerX, PlayerY, PlayerGraph, TRUE);

		app->Render();


		// DX���C�u�����̐ݒ��߂��B
		RefreshDxLibDirect3DSetting();

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// �t���X�N���[���؂�ւ�
		if (CheckHitKey(KEY_INPUT_F1) && !isFullScreen)
		{
			ChangeWindowMode(FALSE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = true;
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
		}
	}

	app->Release();

	DxLib_End();
#endif
	return 0;
}