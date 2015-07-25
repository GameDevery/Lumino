
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include "Platform/PlatformManager.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"
#include "Documents/DocumentsManager.h"
#include "GUI/GUIManager.h"
#include "Game/FpsController.h"
#include "ApplicationConfigData.h"

namespace Lumino
{
	
/**
	@brief		Lumino �A�v���P�[�V������\���܂��B
*/
class Application
	: public RefObject
	, public Platform::IEventListener
{
public:

	/**
		@brief		Application �̃C���X�^���X���쐬���A�A�v���P�[�V���������������܂��B
	*/
	static Application* Create(const ApplicationConfigData& configData);

public:

	/**
		@brief		
		@return		�A�v���P�[�V�����̏I�����v������Ă���ꍇ�� false ��Ԃ��܂��B
	*/
	bool UpdateFrame();

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


	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	GUI::GUIManager* GetGUIManager() const { return m_guiManager; }

private:
	Application(const ApplicationConfigData& configData);
	virtual ~Application();
	void Initialize();
	void InitialzePlatformManager();
	void InitialzePhysicsManager();
	void InitialzeGraphicsManager();
	void InitialzeDocumentsManager();
	void InitialzeGUIManager();
	virtual bool OnEvent(const Platform::EventArgs& e);

private:
	//class NativeWindowEventListener;
	ApplicationConfigData				m_configData;
	FpsController						m_fpsController;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	RefPtr<GUI::GUIManager>				m_guiManager;
	
	//NativeWindowEventListener*			m_nativeWindowEventListener;
	bool								m_endRequested;
};

} // namespace Lumino
