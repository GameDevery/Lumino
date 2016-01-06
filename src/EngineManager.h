
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

LN_NAMESPACE_BEGIN

namespace detail { class InputManager; }
namespace detail { class EffectManager; }
namespace detail { class ModelManager; }
namespace detail { class UIManager; }
class AudioManagerImpl;
class ProfilerRenderer;
class SceneGraphManager;

class EngineManager
	: public RefObject
	, public Platform::IEventListener
{
public:
	static EngineManager*	Instance;

public:
	static const TCHAR*	LogFileName;

public:

	/**
		@brief		Application �̃C���X�^���X���쐬���A�A�v���P�[�V���������������܂��B
	*/
	static EngineManager* Create(const ApplicationSettings& configData);

public:

	bool UpdateFrame();
	bool BeginRendering();
	void EndRendering();
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
	detail::UIManager* GetUIManager() const { return m_uiManager; }
	SceneGraphManager* GetSceneGraphManager() const { return m_sceneGraphManager; }

protected:
	EngineManager(const ApplicationSettings& configData);
	virtual ~EngineManager();

public:
	void Initialize();
	void InitializeCommon();
	void InitializeFileManager();
	void InitializePlatformManager();
	void InitializeInputManager();
	void InitializeAudioManager();
	void InitializePhysicsManager();
	void InitializeGraphicsManager();
	void InitializeEffectManager();
	void InitializeModelManager();
	void InitializeDocumentsManager();
	void InitializeGUIManager();
	void InitializeSceneGraphManager();
	virtual bool OnEvent(const Platform::EventArgs& e);

private:
	//class NativeWindowEventListener;
	ApplicationSettings				m_configData;
	FpsController						m_fpsController;
	FileManager*						m_fileManager;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	detail::InputManager*				m_inputManager;
	AudioManagerImpl*					m_audioManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<GraphicsManager>	m_graphicsManager;
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	detail::UIManager*						m_uiManager;
	SceneGraphManager*					m_sceneGraphManager;

	ProfilerRenderer*			m_profilerRenderer;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_frameRenderingSkip;
	bool								m_frameRenderd;
	bool								m_commonInitied;
	bool								m_endRequested;
	bool								m_comInitialized;
};

LN_NAMESPACE_END