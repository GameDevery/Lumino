#include "DxLib.h"
#include <d3d9.h>
#include <LuminoDxLib.h>
using namespace Lumino;

int PlayerX, PlayerY;
int PlayerGraph;

void DeviceLostFunction(void *Data)
{
	printf("Start LostFunction\n");
	Graphics::ChangeDirectX9Device(NULL);
	printf("End LostFunction\n");
}
void DeviceRestoreFunction(void *Data)
{
	printf("Start RestoreFunction\n");
	Graphics::ChangeDirectX9Device((IDirect3DDevice9*)GetUseDirect3DDevice9());
	printf("End RestoreFunction\n");
}

GUIContext* g_mainContext = NULL;

LRESULT CALLBACK HookDxLibMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		g_mainContext->InjectMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	{
		float x = LOWORD(lParam);
		float y = HIWORD(lParam);
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			g_mainContext->InjectMouseButtonDown(MouseButton::Left, x, y);
			break;
		case WM_LBUTTONUP:
			g_mainContext->InjectMouseButtonUp(MouseButton::Left, x, y);
			break;
		case WM_RBUTTONDOWN:
			g_mainContext->InjectMouseButtonDown(MouseButton::Right, x, y);
			break;
		case WM_RBUTTONUP:
			g_mainContext->InjectMouseButtonUp(MouseButton::Right, x, y);
			break;
		case WM_MBUTTONDOWN:
			g_mainContext->InjectMouseButtonDown(MouseButton::Middle, x, y);
			break;
		case WM_MBUTTONUP:
			g_mainContext->InjectMouseButtonUp(MouseButton::Middle, x, y);
			break;
		}
		return 0;
	}
	}
	//SetUseHookWinProcReturnValue(TRUE);
	return 0;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine, int nCmdShow)
int main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// DX ���C�u�����̏�����
	bool isFullScreen = false;
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	//SetUseDirect3D9Ex(FALSE);
	SetUseDirect3DVersion(DX_DIRECT3D_9);	// DirectX9���g�p����悤�ɂ���B
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK)) return -1;

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// Z�o�b�t�@��L���ɂ���B
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	SetWriteZBuffer3D(TRUE);

	SetHookWinProc(HookDxLibMessage);

	//printf("%p\n", GetUseDirect3DDevice9());

	// Lumino ������
	ApplicationSettings appData;
	appData.GraphicsAPI = GraphicsAPI::DirectX9;
	appData.RenderingType = RenderingType::Immediate;
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


	{
		auto* d = (IDirect3DDevice9*)GetUseDirect3DDevice9();
		d->AddRef();
		printf("%p ref:%d\n", d, d->Release());
	}

	Graphics::ChangeDirectX9Device(NULL);

	Graphics::ChangeDirectX9Device((IDirect3DDevice9*)GetUseDirect3DDevice9());

#if 1
	GCPtr<GUIContext> context1 = GUIContext::Create();
	g_mainContext = context1;
	context1->InjectViewportSizeChanged(640, 480);


	GCPtr<PilePanel> panel1 = PilePanel::Create();
	context1->SetRootElement(panel1);

	GCPtr<UIButton> button1 = UIButton::Create();
	button1->SetSize(SizeF(200, 100));
	button1->Click += [](RoutedEventArgs* e) { printf("Click!\n"); };
	panel1->GetChildren()->Add(button1);

	//GCPtr<UIButton> button2 = UIButton::Create();
	//button2->SetSize(SizeF(100, 200));
	//panel1->GetChildren()->Add(button2);

	button1->SetEnabled(false);
#endif


	// ���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		printf("a\n");
		// ��ʂ�����������
		ClearDrawScreen();

		DrawBox(0, 0, 640, 480, GetColor(0, 0, 255), TRUE);

		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		DrawGraph(0, 0, dummyGraph, TRUE);

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
		RenderVertex();


		context1->InjectElapsedTime(0.016);

		UI::UpdateAllContext();

		Renderer::BeginRendering();

		Renderer::Clear(ClearFlags::All, ColorF::White);
		UI::RenderAllContext();

		Renderer::EndRendering();


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
			printf("%p\n", GetUseDirect3DDevice9());
		}
		if (CheckHitKey(KEY_INPUT_F2) && isFullScreen)
		{
			ChangeWindowMode(TRUE);
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
			printf("%p\n", GetUseDirect3DDevice9());
		}
	}

	DeleteGraph(dummyGraph);

	Graphics::ChangeDirectX9Device(NULL);
	Application::Finalize();

	{
		auto* d = (IDirect3DDevice9*)GetUseDirect3DDevice9();
		d->AddRef();
		printf("%p ref:%d\n", d, d->Release());
	}


	DxLib_End();


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