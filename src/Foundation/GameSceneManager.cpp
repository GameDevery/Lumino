﻿
#include "../Internal.h"
#include <Lumino/Foundation/GameScene.h>
#include "GameSceneManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// GameScene
//==============================================================================

//------------------------------------------------------------------------------
GameScene::GameScene()
{
}

//------------------------------------------------------------------------------
GameScene::~GameScene()
{
}

//------------------------------------------------------------------------------
void GameScene::OnStart()
{
}

//------------------------------------------------------------------------------
void GameScene::OnUpdate()
{
}

//------------------------------------------------------------------------------
void GameScene::OnTerminate()
{
}


//==============================================================================
// GameScene
//==============================================================================

//------------------------------------------------------------------------------
GameSceneManager::GameSceneManager()
{
}

//------------------------------------------------------------------------------
GameSceneManager::~GameSceneManager()
{
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GameSceneManager::GotoScene(GameScene* scene)
//{
//	if (scene == m_nextScene) {
//		return;
//	}
//
//	size_t index = m_sceneStack.size();
//
//	m_sceneStack.replace(index - 1, scene);
//
//	m_nextScene = scene;
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GameSceneManager::CallScene(GameScene* scene)
//{
//	m_sceneStack.Push(m_runningScene);
//	m_nextScene = scene;
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GameSceneManager::ReturnScene()
//{
//	m_sceneStack.Pop();
//	m_nextScene = m_sceneStack.GetTop();
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GameSceneManager::UpdateFrame()
//{
//	if (m_nextScene != nullptr)
//	{
//		ChangeNextScene();
//	}
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GameSceneManager::ChangeNextScene()
//{
//	if (m_runningScene != nullptr)
//	{
//		m_runningScene->onExitTransitionDidStart();
//		_runningScene->onExit();
//		_runningScene.SafeRelease();
//	}
//
//	m_runningScene = m_nextScene;
//	m_nextScene = nullptr;
//
//	if (m_runningScene != nullptr)
//	{
//		m_runningScene->onEnter();
//		m_runningScene->onEnterTransitionDidFinish();
//	}
//}

LN_NAMESPACE_END
