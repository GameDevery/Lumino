
#pragma once

#include "Internal.h"
#include <Lumino/Application.h>

namespace Lumino
{

////=============================================================================
//// NativeWindowEventListener
////=============================================================================
//class Application::NativeWindowEventListener
//{
//public:
//	NativeWindowEventListener(Application)
//	{
//	}
//};

//=============================================================================
// Application
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application* Application::Create(const ApplicationConfigData& configData)
{
	RefPtr<Application> app(LN_NEW Application(configData));
	app->Initialize();
	app.SafeAddRef();
	return app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfigData& configData)
	: m_configData(configData)
	, m_endRequested(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application::~Application()
{
	if (m_platformManager != NULL) {
		m_platformManager->Dispose();
	}

	if (m_guiManager != NULL) {
		m_guiManager->Finalize();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::Initialize()
{
	InitialzePlatformManager();
	InitialzePhysicsManager();
	InitialzeGraphicsManager();
	InitialzeGUIManager();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzePlatformManager()
{
	if (m_platformManager.IsNull())
	{
		Platform::ApplicationSettings data;
		data.API = Platform::WindowSystemAPI_Win32API;
		data.MainWindowSettings.Title = _T("");
		data.MainWindowSettings.ClientSize.Set(640, 480);
		data.MainWindowSettings.Fullscreen = false;
		data.MainWindowSettings.Resizable = true;
		data.UseInternalUIThread = false;

		m_platformManager.Attach(LN_NEW Platform::PlatformManager());
		m_platformManager->Initialize(data);

		// �C�x���g���X�i�[�o�^
		m_platformManager->GetMainWindow()->AttachEventListener(this, 0);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzePhysicsManager()
{
	if (m_physicsManager.IsNull())
	{
		m_physicsManager.Attach(LN_NEW Physics::PhysicsManager(Physics::SimulationType_Sync));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzeGraphicsManager()
{
	if (m_graphicsManager.IsNull())
	{
		InitialzePlatformManager();
		InitialzePhysicsManager();

		Graphics::GraphicsManagerConfigData data;
		data.GraphicsAPI = m_configData.GraphicsAPI;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.FileManager = &FileManager::GetInstance();
		data.PlatformTextureLoading = true;
		m_graphicsManager.Attach(LN_NEW Graphics::GraphicsManager(data));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzeDocumentsManager()
{
	if (m_documentsManager.IsNull())
	{
		InitialzeGraphicsManager();

		Documents::DocumentsManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		m_documentsManager.Attach(LN_NEW Documents::DocumentsManager(data));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::InitialzeGUIManager()
{
	if (m_guiManager.IsNull())
	{
		InitialzePlatformManager();
		InitialzeGraphicsManager();
		InitialzeDocumentsManager();

		GUI::GUIManager::ConfigData data;
		data.GraphicsManager = m_graphicsManager;
		data.MainWindow = m_platformManager->GetMainWindow();
		data.DocumentsManager = m_documentsManager;
		m_guiManager.Attach(LN_NEW GUI::GUIManager());
		m_guiManager->Initialize(data);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Application::UpdateFrame()
{
	m_endRequested = !m_platformManager->DoEvents();

	m_fpsController.Process();

	return !m_endRequested;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Application::ResetFrameDelay()
{
	m_fpsController.RefreshSystemDelay();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Application::OnEvent(const Platform::EventArgs& e)
{
	switch (e.Type)
	{
	case Platform::EventType_Quit:	///< �A�v���I���v��
	case Platform::EventType_Close:			///< �E�B���h�E�������悤�Ƃ��Ă���
		break;

	case Platform::EventType_MouseDown:		// �E�X�{�^���������ꂽ
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseButtonDown(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_MouseUp:			// �}�E�X�{�^���������ꂽ
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseButtonUp(e.Mouse.Button, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_MouseMove:		// �}�E�X���ړ�����
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseMove(e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_MouseWheel:		// �}�E�X�z�C�[�������삳�ꂽ
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectMouseWheel(e.Mouse.Delta, e.Mouse.X, e.Mouse.Y)) { return true; }
		}
		break;
	case Platform::EventType_KeyDown:	// �L�[����
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectKeyDown(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl)) { return true; }
		}
		break;
	case Platform::EventType_KeyUp:		//  �L�[�����グ
		if (m_guiManager != NULL) {
			if (m_guiManager->InjectKeyUp(e.Key.KeyCode, e.Key.IsAlt, e.Key.IsShift, e.Key.IsControl)) { return true; }
		}
		break;

	default:
		break;
	}
	return false;
}

} // namespace Lumino
