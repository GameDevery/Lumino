
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include "Platform/PlatformManager.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"
#include "Game/FpsController.h"

namespace Lumino
{
	
/**
	@brief		Lumino �A�v���P�[�V������\���܂��B
*/
class Application
	: public RefObject
{
public:

	/**
		@brief		Application �̃C���X�^���X���쐬���A�A�v���P�[�V���������������܂��B
	*/
	static Application* Create();

public:

	/**
		@brief		
		@return		�A�v���P�[�V�����̏I�����v������Ă���ꍇ�� false ��Ԃ��܂��B
	*/
	bool UpdateFrame();

	/**
		@brief		�A�v���P�[�V�����̏I�����v������Ă��邩���m�F���܂��B
	*/
	bool IsEndRequested() const { return m_endRequested; }

private:
	Application();
	virtual ~Application();
	void Initialize();
	void InitialzePlatformManager();
	void InitialzePhysicsManager();
	void InitialzeGraphicsManager();

private:
	FpsController						m_fpsController;
	RefPtr<Platform::PlatformManager>	m_platformManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	bool								m_endRequested;
};

} // namespace Lumino
