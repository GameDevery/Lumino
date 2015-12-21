
#include "Internal.h"
#include <Lumino/IO/Console.h>
#include <Lumino/Profiler.h>
#include "Input/InputManager.h"
#include <Lumino/Audio/AudioManager.h>
#include <Lumino/Engine.h>
#include "Graphics/RendererImpl.h"
#include "Graphics/ProfilerRenderer.h"
#include "Graphics/RenderingThread.h"
#include "Scene/SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include "Effect/EffectManager.h"
#include "GUI/UIManagerImpl.h"
#include "EngineManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Engine
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Engine::Initialize(const ApplicationSettings& settings)
{
	EngineManager::Instance = EngineManager::Create(settings);
	EngineManager::Instance->Initialize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Engine::Finalize()
{
	LN_SAFE_RELEASE(EngineManager::Instance);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Engine::UpdateFrame()
{
	LN_CHECK_STATE(EngineManager::Instance != NULL);
	return EngineManager::Instance->UpdateFrame();
}


LN_NAMESPACE_END
