
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include <Lumino/Platform/PlatformManager.h>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/Game/FpsController.h>
#include <Lumino/ApplicationSettings.h>

namespace Lumino
{
class AudioManagerImpl;
class ProfilerRenderer;
class SceneGraphManager;
class GUIManagerImpl;

class ApplicationImpl
	: public RefObject
	, public Platform::IEventListener
{
public:
	static ApplicationImpl*	Instance;

public:
	static const TCHAR*	LogFileName;

public:

	/**
		@brief		Application �̃C���X�^���X���쐬���A�A�v���P�[�V���������������܂��B
	*/
	static ApplicationImpl* Create(const ApplicationSettings& configData);

public:

	/**
		@brief		
		@return		�A�v���P�[�V�����̏I�����v������Ă���ꍇ�� false ��Ԃ��܂��B
	*/
	bool UpdateFrame();

	/**
		@brief		
	*/
	void Render();

	/**
		@brief		�x�������Z�b�g���܂��B
		@details	���\�[�X�̃��[�h���Ŏ��Ԃ������蒷�����ԍX�V�������s���Ȃ������ꍇ�A
					UpdateFrame() �͖{������ׂ����Ԃɒǂ������Ƃ��Ă��΂炭�̊ԃm�[�E�F�C�g�Ńt���[���X�V���s���܂��B
					���̊Ԃ̓A�v���P�[�V���������ɍ����ɓ��삵�Ă���悤�Ɍ����Ă��܂��܂��B
					�����������邽�߁A���Ԃ̂����鏈���̒���ł��̊֐����ĂԂ��ƂŁAFPS ����ɒx�����������Ă��Ȃ����Ƃ�`���܂��B
	*/
	void ResetFrameDelay();

	/**
		@brief		�A�v���P�[�V�����̏I�����v������Ă��邩���m�F���܂��B
	*/
	bool IsEndRequested() const { return m_endRequested; }


	Platform::PlatformManager* GetPlatformManager() const { return m_platformManager; }
	AudioManagerImpl* GetAudioManager() const { return m_audioManager; }
	Physics::PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }
	GUIManagerImpl* GetGUIManager() const { return m_guiManager; }
	SceneGraphManager* GetSceneGraphManager() const { return m_sceneGraphManager; }

protected:
	ApplicationImpl(const ApplicationSettings& configData);
	virtual ~ApplicationImpl();

public:
	void Initialize();
	void InitializeCommon();
	void InitialzeFileManager();
	void InitialzePlatformManager();
	void InitialzeAudioManager();
	void InitialzePhysicsManager();
	void InitialzeGraphicsManager();
	void InitialzeDocumentsManager();
	void InitialzeGUIManager();
	void InitialzeSceneGraphManager();
	virtual bool OnEvent(const Platform::EventArgs& e);

private:
	//class NativeWindowEventListener;
	ApplicationSettings				m_configData;
	FpsController						m_fpsController;
	FileManager*						m_fileManager;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	AudioManagerImpl*					m_audioManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	GUIManagerImpl*						m_guiManager;
	SceneGraphManager*					m_sceneGraphManager;

	ProfilerRenderer*			m_profilerRenderer;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_commonInitied;
	bool								m_endRequested;
};

} // namespace Lumino
