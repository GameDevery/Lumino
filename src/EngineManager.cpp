/*
[2016/3/9] �����_�����O�p�X
	��Ԃ��炢�̂� Viewport �N���X�ɂ������B
	Engine �� Window �̓R���ɑ΂��� Render() ���ĂԂ�����OK�A�݂����ȁB

	Editor ���[�h�̂Ƃ��AViewport �Ńy�C���𕪂���ƍĕ`��͈̔͂�����ł���B
	1��Widget�ɍĕ`�悪�K�v�ɂȂ��Ă��E�B���h�E�S�̂��ĕ`�悷��K�v�͂Ȃ��B
	�� Viewport �� �����_�[�^�[�Q�b�g�����B

	Game ���[�h�̂Ƃ��͊�{�I�� Viewport �� 1�����ɂȂ�Ǝv���B

	Window ���� Viweport �̊K�w�\���B
	���[�g�� Viewport �� Window �S�̂ŁAswapchain �̃����_�[�^�[�Q�b�g���A�^�b�`����̂����������B

	GameMode (���t���[��)
		1.Scene �`��BScene ���̂��ׂẴJ�����̃����_�[�^�[�Q�b�g�ɕ`�悷��
		2.Viewport �`��B���[�g���珇�ɁA�����_�[�^�[�Q�b�g���o�b�N�o�b�t�@�ɕ`�悷��

	EditorMode
		�ĕ`��v���������Ƃ��������̂��s���B

	ImageEffect
		�|�X�g�G�t�F�N�g�B�J�����ɂ������BUnity �Ɠ������񂶁B


	�Q�l:UE4
	https://shikihuiku.wordpress.com/2014/05/09/%E3%83%A1%E3%83%A2-ue4%E3%81%AErendering%E3%81%AE%E5%91%BC%E3%81%B0%E3%82%8C%E6%96%B9/


[2015/8/30] �v���p�e�B�̌p��
	���r�W�����J�E���g��p�������@
	�E�e�`���Ԃ̗v�f�̃v���p�e�B���ύX���ꂽ���́H
		���̃v���p�e�B�̓Ǝ��ݒ�ON/OFF���؂�ւ�������A�S�Ă̎q�̓����v���p�e�B�ɍčX�V�t���O�𗧂Ă�B
		���S�q�����͎��Ԃ�����E�E�E�e�̃��r�W�����𑀍삷��̂́H
			�����[�g�܂Ńf�t�H���g���������Ή��ł��Ȃ�
				�����[�g���p�����Ƃ��ĎQ�Ƃ���B
	�E�c���[����c���[���؂藣���ꂽ/�ǉ����ꂽ���́H
		�؂藣���ꂽ/�ǉ����ꂽ�c���[�̑S�m�[�h�̃v���p�e�B�̍čX�V�t���O��ON�ɂ���B


[2015/7/31] �g�b�v���x���C���^�[�t�F�C�X
	- �p���ɂ��g��
	- ����o�C���_�����������ł̎g���₷��
	- Variant �ւ̕ێ����₷��
	- �A�v�������������ł̎g���₷��

	�ȉ��̂悤�ȕ��j�ŁB
	- �g�b�v���x���I�u�W�F�N�g (EngineManager) �̓O���[�o���B
	  (���S�ɃO���[�o���ł͂Ȃ��A�C���X�^���X�̃|�C���^���O���[�o���ϐ��ɓ���Ă����C���[�W�B�K�v�ɉ����Čp�����A�g���ł���)
	- 



	�EFont font = Font::CreateBitmapFont();
	

	�EFontPtr font = Font::CreateBitmapFont();

	- �X�^�b�N�ւ̐����������邩�H
		���S�ɋ֎~���邱�Ƃ͏o���Ȃ��B�h��������΂Ȃ�Ƃł��o���Ă��܂��B
	

	�ESiv3D�A�Z�KGameLib
		���J����̂̓X�}�[�g�|�C���^�N���X�B���͉̂\�Ȍ��茩���Ȃ��B
		�� �h�������Ċg���ł��Ȃ��BGUI �̃��[�U�[�R���g���[���Ƃ����Ȃ����ƂɂȂ�B
		�����ASiv3D �� GUI �� static Create() �� shared_ptr �Ԃ��Ă����B

	�ESDL2�AGLFW�AGDI+�ANux �Ȃ񂩂̓g�b�v���x���I�u�W�F�N�g�̓O���[�o���C���X�^���X�B
	  OpenSceneGraph �����Ԃ񂻂��B
*/


#pragma once
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include <Lumino/Platform/PlatformWindow.h>
#include "Input/InputManager.h"
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Engine.h>
#include "Graphics/RendererImpl.h"
#include "Graphics/ProfilerRenderer.h"
#include "Graphics/RenderingThread.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include "Scene/SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include "Effect/EffectManager.h"
#include "UI/UIManager.h"
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/Foundation/Application.h>
#include "EngineManager.h"

LN_NAMESPACE_BEGIN

////=============================================================================
//// NativeWindowEventListener
////=============================================================================
//class EngineManager::NativeWindowEventListener
//{
//public:
//	NativeWindowEventListener(EngineManager)
//	{
//	}
//};

//=============================================================================
// EngineManager
//=============================================================================

EngineManager* EngineManager::Instance = nullptr;
const TCHAR* EngineManager::LogFileName = _T("lnlog.txt");

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager* EngineManager::Create(const EngineSettings& configData)
{
	RefPtr<EngineManager> app(LN_NEW EngineManager(configData), false);
	//app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager::EngineManager(const EngineSettings& configData)
	: m_configData(configData)
	, m_fileManager(nullptr)
	, m_inputManager(nullptr)
	, m_audioManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_effectManager(nullptr)
	, m_modelManager(nullptr)
	, m_uiManager(nullptr)
	, m_sceneGraphManager(nullptr)
	, m_profilerRenderer(nullptr)
	, m_application(nullptr)
	, m_frameRenderingSkip(false)
	, m_frameRenderd(false)
	, m_commonInitied(false)
	, m_endRequested(false)
	, m_comInitialized(false)
{
	m_fpsController.SetEnableFpsTest(true);
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_MainThread, 60.0f);	// TODO 
	Profiler::Instance.SetBaseFrameRate(Profiler::Group_RenderThread, 60.0f);



	// COM ������
	if (m_configData.autoCoInitialize && SUCCEEDED(::CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		// �G���[�ɂ͂��Ȃ��B�ʂ̐ݒ�� COM ���������ς݂������肷��Ǝ��s���邱�Ƃ����邪�ACOM ���͎̂g����悤�ɂȂ��Ă���
		m_comInitialized = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineManager::~EngineManager()
{
	LN_SAFE_RELEASE(m_application);

	LN_SAFE_RELEASE(m_profilerRenderer);

	if (m_graphicsManager != nullptr)
	{
		// ��ɕ`��X���b�h���I�����Ă����B
		// �����W���[���Ŕ��s���ꂽ�R�}���h���܂����s�ҋ@���ɂ��̃��W���[������������ƃ}�Y�C�B
		m_graphicsManager->Finalize();
	}

	if (m_platformManager != nullptr) {
		m_platformManager->Dispose();
	}
	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->ReleaseDefaultSceneGraph();
		LN_SAFE_RELEASE(m_sceneGraphManager);
	}
	if (m_modelManager != nullptr) {
		m_modelManager->Finalize();
		LN_SAFE_RELEASE(m_modelManager);
	}
	if (m_effectManager != nullptr) {
		m_effectManager->Finalize();
		LN_SAFE_RELEASE(m_effectManager);
	}
	if (m_uiManager != nullptr) {
		m_uiManager->Finalize();
		LN_SAFE_RELEASE(m_uiManager);
	}

	LN_SAFE_RELEASE(m_graphicsManager);

	if (m_physicsManager != nullptr) {
		m_physicsManager->Finalize();
		m_physicsManager.SafeRelease();
		//LN_SAFE_RELEASE(m_physicsManager);
	}
#ifdef LN_BUILD_AUDIO_MODULE
	if (m_audioManager != nullptr) {
		m_audioManager->Finalize();
		LN_SAFE_RELEASE(m_audioManager);
	}
#endif
	if (m_inputManager != nullptr) {
		m_inputManager->Finalize();
		LN_SAFE_RELEASE(m_inputManager);
	}

	LN_SAFE_RELEASE(m_fileManager);

	Console::Free();


	if (m_comInitialized)
	{
		::CoUninitialize();
		m_comInitialized = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::Initialize()
{
	InitializePlatformManager();
	InitializeInputManager();
	InitializeAudioManager();
	InitializePhysicsManager();
	InitializeGraphicsManager();
	InitializeEffectManager();
	InitializeModelManager();
	InitializeUIManager();
#ifdef LN_BUILD_SCENE_MODULE
	InitializeSceneGraphManager();
#endif
	m_application = LN_NEW detail::InternalApplicationImpl();	// TODO: �Ƃ肠����
	m_application->Initialize(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeCommon()
{
	if (!m_commonInitied)
	{
		// ���O�t�@�C���o��
		if (m_configData.ApplicationLogEnabled) {
			Logger::Initialize(LogFileName);
		}
		// �R���\�[�����蓖��
		if (m_configData.ConsoleEnabled) {
			Console::Alloc();
		}

		m_commonInitied = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeFileManager()
{
	if (m_fileManager == nullptr)
	{
		FileManager::Settings data;
		data.AccessMode = m_configData.FileAccessPriority;
		m_fileManager = LN_NEW FileManager(data);
		for (auto& e : m_configData.ArchiveFileEntryList) {
			m_fileManager->RegisterArchive(e.FilePath, e.Password);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializePlatformManager()
{
	if (m_platformManager.IsNull())
	{
		InitializeCommon();

		PlatformManager::Settings data;
		data.API = WindowSystemAPI_Win32API;
		data.MainWindowSettings.Title = m_configData.mainWindowTitle;
		data.MainWindowSettings.ClientSize = m_configData.mainWindowSize;
		data.MainWindowSettings.Fullscreen = false;
		data.MainWindowSettings.Resizable = true;
		data.MainWindowSettings.UserWindow = m_configData.UserMainWindow;
		data.UseInternalUIThread = false;

		m_platformManager.Attach(LN_NEW PlatformManager());
		m_platformManager->Initialize(data);

		// �C�x���g���X�i�[�o�^
		m_platformManager->GetMainWindow()->AttachEventListener(this, 0);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeInputManager()
{
	if (m_inputManager == nullptr)
	{
		InitializePlatformManager();

		detail::InputManager::Settings data;
		data.mainWindow = m_platformManager->GetMainWindow();

		m_inputManager = LN_NEW detail::InputManager();
		m_inputManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeAudioManager()
{
#ifdef LN_BUILD_AUDIO_MODULE
	if (m_audioManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();

		// ���[�U�[��`�̃E�B���h�E�n���h�����w�肳��Ă���ꍇ�A
		// �_�~�[�E�B���h�E�N���X����邽�߂� PlatformManager �̏��������K�v�B
		if (m_configData.UserMainWindow != nullptr) {
			InitializePlatformManager();
		}

		AudioManagerImpl::Settings data;
		data.FileManager = m_fileManager;
		data.StreamCacheObjectCount = m_configData.soundCacheCapacity.objectCount;
		data.StreamSourceCacheMemorySize = m_configData.soundCacheCapacity.memorySize;
		data.DMInitMode = m_configData.directMusicMode;
#ifdef LN_OS_WIN32
		data.hWnd = (m_platformManager != nullptr) ? PlatformSupport::GetWindowHandle(m_platformManager->GetMainWindow()) : nullptr;
#endif
		data.DirectMusicReverbLevel = m_configData.DirectMusicReverbLevel;
		m_audioManager = AudioManagerImpl::Create(data);
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializePhysicsManager()
{
#ifdef LN_BUILD_SCENE_MODULE
	if (m_physicsManager.IsNull())
	{
		InitializeCommon();
		m_physicsManager.Attach(LN_NEW Physics::PhysicsManager(Physics::SimulationType_Sync));
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeGraphicsManager()
{
	if (m_graphicsManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();
		InitializePlatformManager();
		InitializePhysicsManager();

		GraphicsManager::ConfigData data;
		data.GraphicsAPI = m_configData.GraphicsAPI;
		data.RenderingType = m_configData.RenderingType;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.backBufferSize = m_configData.backBufferSize;
		data.FileManager = m_fileManager;
		data.PlatformTextureLoading = true;
#ifdef LN_OS_WIN32
		data.D3D9Device = m_configData.D3D9Device;
#endif
		m_graphicsManager = LN_NEW GraphicsManager(data);
		GraphicsManager::Instance = m_graphicsManager;

		m_profilerRenderer = LN_NEW ProfilerRenderer(m_graphicsManager, &Profiler::Instance);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeEffectManager()
{
	if (m_effectManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();

		detail::EffectManager::Settings data;
		data.fileManager = m_fileManager;
		data.audioManager = m_audioManager;
		data.graphicsManager = m_graphicsManager;
		m_effectManager = LN_NEW detail::EffectManager();
		m_effectManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeModelManager()
{
	if (m_modelManager == nullptr)
	{
		InitializeCommon();
		InitializeGraphicsManager();
		InitializePhysicsManager();

		detail::ModelManager::Settings data;
		data.FileManager = m_fileManager;
		data.PhysicsManager = m_physicsManager;
		data.GraphicsManager = m_graphicsManager;
		data.ModelCoreCacheSize = 32;
		data.ModelCoreCacheMemorySize = 0;
		m_modelManager = LN_NEW detail::ModelManager();
		m_modelManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeDocumentsManager()
{
	if (m_documentsManager.IsNull())
	{
		InitializeCommon();
		InitializeGraphicsManager();

		Documents::DocumentsManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		m_documentsManager.Attach(LN_NEW Documents::DocumentsManager(data));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::InitializeUIManager()
{
	if (m_uiManager == nullptr)
	{
		InitializeCommon();
		InitializePlatformManager();
		InitializeGraphicsManager();
		InitializeDocumentsManager();

		detail::UIManager::Settings data;
		data.graphicsManager = m_graphicsManager;
		data.mainWindow = m_platformManager->GetMainWindow();
		data.documentsManager = m_documentsManager;
		m_uiManager = LN_NEW detail::UIManager();
		m_uiManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#ifdef LN_BUILD_SCENE_MODULE
void EngineManager::InitializeSceneGraphManager()
{
	if (m_sceneGraphManager == nullptr)
	{
		InitializeCommon();
		InitializeFileManager();
		InitializeGraphicsManager();
		InitializePhysicsManager();
		InitializeEffectManager();
		InitializeModelManager();
		SceneGraphManager::ConfigData data;
		data.FileManager = m_fileManager;
		data.GraphicsManager = m_graphicsManager;
		data.PhysicsManager = m_physicsManager;
		data.effectManager = m_effectManager;
		data.modelManager = m_modelManager;
		data.mainViewport = m_graphicsManager->GetMainViewport();
		m_sceneGraphManager = LN_NEW SceneGraphManager(data);
		m_sceneGraphManager->CreateDefaultSceneGraph();
		SceneGraphManager::Instance = m_sceneGraphManager;
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EngineManager::UpdateFrame()
{
	m_endRequested = !m_platformManager->DoEvents();

	if (m_inputManager != nullptr) {
		m_inputManager->UpdateFrame();
	}

	if (m_sceneGraphManager != nullptr) {
		m_sceneGraphManager->UpdateFrameDefaultSceneGraph(m_fpsController.GetElapsedGameTime());
	}

	if (m_uiManager != nullptr)
	{
		m_uiManager->GetDefaultUIContext()->InjectElapsedTime(m_fpsController.GetElapsedGameTime());

		{	// �v���t�@�C�����O�͈�
			ScopedProfilerSection prof(Profiler::Group_MainThread, Profiler::Section_MainThread_GUILayput);
			const Size& size = m_graphicsManager->GetMainSwapChain()->GetBackBuffer()->GetSize();
			m_uiManager->GetDefaultUIContext()->GetMainWindowView()->UpdateLayout(SizeF(static_cast<float>(size.Width), static_cast<float>(size.Height)));
		}
	}

	// �蓮�`�悳��Ă��Ȃ���΂����Ŏ����`�悷��
	if (!m_frameRenderd)
	{
		if (BeginRendering())
		{
			Render();
			EndRendering();
		}
	}

	m_fpsController.Process();

	Profiler::Instance.SetMainFPS(m_fpsController.GetFps());
	Profiler::Instance.SetMainFPSCapacity(m_fpsController.GetCapacityFps());

	m_frameRenderd = false;
	return !m_endRequested;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EngineManager::BeginRendering()
{
	m_frameRenderingSkip = true;
	if (m_graphicsManager == nullptr) return false;

	// �`��x���̊m�F
	bool delay = false;
	if (m_graphicsManager->GetRenderingType() == RenderingType::Deferred)
	{
		if (m_graphicsManager->GetRenderingThread()->IsRunning()) {
			delay = true;
		}
	}
	else {
		// TODO:
	}
	if (delay) {
		return false;
	}

	m_frameRenderingSkip = false;



	if (m_effectManager != nullptr) {
		m_effectManager->PreRender();	// Effekseer �̍X�V�X���b�h���J�n����̂͂���
	}

	Details::Renderer* renderer = m_graphicsManager->GetRenderer();
	SwapChain* swap = m_graphicsManager->GetMainSwapChain();
	renderer->Begin();
	renderer->SetRenderTarget(0, swap->GetBackBuffer());
	renderer->SetDepthBuffer(swap->GetBackBufferDepth());
	renderer->Clear(ClearFlags::All, ColorF::White);

	m_frameRenderd = true;
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::EndRendering()
{
	if (m_graphicsManager == nullptr || m_frameRenderingSkip) return;

	Details::Renderer* renderer = m_graphicsManager->GetRenderer();
	SwapChain* swap = m_graphicsManager->GetMainSwapChain();
	m_graphicsManager->SwitchActiveContext(nullptr);
	renderer->End();
	swap->Present();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::Render()
{
	if (m_graphicsManager != nullptr)
	{
		//// �`��x���̊m�F
		//bool delay = false;
		//if (m_graphicsManager->GetRenderingType() == RenderingType::Deferred)
		//{
		//	if (m_graphicsManager->GetRenderingThread()->IsRunning()) {
		//		delay = true;
		//	}
		//}
		//else {
		//	// TODO:
		//}
		//if (delay) {
		//	m_frameRenderingSkip = true;
		//	return;
		//}
		//m_frameRenderingSkip = false;


		//if (m_effectManager != nullptr) {
		//	m_effectManager->PreRender();	// Effekseer �̍X�V�X���b�h���J�n����̂͂���
		//}

		Details::Renderer* renderer = m_graphicsManager->GetRenderer();
		SwapChain* swap = m_graphicsManager->GetMainSwapChain();



		//renderer->Begin();
		//renderer->SetRenderTarget(0, swap->GetBackBuffer());
		//renderer->SetDepthBuffer(swap->GetBackBufferDepth());
		//renderer->Clear(ClearFlags::All, ColorF::White);


		//m_graphicsManager->GetRenderer()->Clear(Graphics::ClearFlags::All, Graphics::ColorF::White);

		//Graphics::DepthStencilState state;
		//state.DepthEnable = false;
		//state.DepthWriteEnable = false;
		//state.StencilEnable = false;
		//m_graphicsManager->GetRenderer()->SetDepthStencilState(state);

		//if (m_sceneGraphManager != nullptr) {
		//	m_sceneGraphManager->RenderDefaultSceneGraph(swap->GetBackBuffer());
		//}

		if (m_graphicsManager != nullptr)
		{
			m_graphicsManager->GetMainViewport()->Render();
		}

		if (m_uiManager != nullptr) {
			m_uiManager->GetDefaultUIContext()->Render();
		}

		if (m_profilerRenderer != nullptr) {
			//m_profilerRenderer->Render(Vector2(640, 480));	//TODO
		}

	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineManager::ResetFrameDelay()
{
	m_fpsController.RefreshSystemDelay();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EngineManager::OnEvent(const PlatformEventArgs& e)
{
	UILayoutView* uiView = nullptr;
	if (m_uiManager != nullptr) {
		uiView = m_uiManager->GetDefaultUIContext()->GetMainWindowView();
	}


	switch (e.Type)
	{
	case EventType_Quit:	///< �A�v���I���v��
	case EventType_Close:			///< �E�B���h�E�������悤�Ƃ��Ă���
		break;

	case EventType_MouseDown:		// �E�X�{�^���������ꂽ
		if (uiView != nullptr) {
			if (uiView->InjectMouseButtonDown(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr) {
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseButtonDown(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case EventType_MouseUp:			// �}�E�X�{�^���������ꂽ
		if (uiView != nullptr) {
			if (uiView->InjectMouseButtonUp(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr) {
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseButtonUp(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case EventType_MouseMove:		// �}�E�X���ړ�����
		if (uiView != nullptr) {
			if (uiView->InjectMouseMove(e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr) {
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseMove(e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case EventType_MouseWheel:		// �}�E�X�z�C�[�������삳�ꂽ
		if (uiView != nullptr) {
			if (uiView->InjectMouseWheel(e.Mouse.WheelDelta, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		if (m_sceneGraphManager != nullptr) {
			if (m_sceneGraphManager->GetDefault3DSceneGraph()->InjectMouseWheel(e.Mouse.WheelDelta)) { return true; }
		}
		break;
	case EventType_KeyDown:	// �L�[����
		if (uiView != nullptr) {
			if (uiView->InjectKeyDown(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl)) { return true; }
		}
		break;
	case EventType_KeyUp:		//  �L�[�����グ
		if (uiView != nullptr) {
			if (uiView->InjectKeyUp(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl/*, e.Key.Char*/)) { return true; }
		}
		break;
	case EventType_KeyChar:		//  ��������
		if (uiView != nullptr) {
			if (uiView->InjectTextInput(e.Key.Char)) { return true; }
		}
		break;
	default:
		break;
	}

	if (m_inputManager != nullptr) {
		m_inputManager->OnEvent(e);
	}
	return false;
}

LN_NAMESPACE_END
