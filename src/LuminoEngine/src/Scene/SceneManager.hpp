﻿
#pragma once
#include <stack>
#include <LuminoEngine/Scene/Scene.hpp>

namespace ln {
class Shader;
class Material;
class World;

namespace detail {

class SceneManager
	: public RefObject
{
public:
	SceneManager();
	virtual ~SceneManager();
	void init();
	void dispose();

    Scene* loadScene(const StringRef& sceneAssetFilePath);
    void unloadScene(Scene* scene);

#if 0
	void gotoScene(Scene* scene);
	void callScene(Scene* scene);
	void returnScene();
#endif
	Scene* activeScene() const { return m_activeScene; }

	void updateFrame();

    void setActiveWorld(World* world) { m_activeWorld = world; }
    World* activeWorld() const { return m_activeWorld; }
    const Ref<Shader>& atmosphereShader() const { return m_atmosphereShader; }
	const Ref<Raycaster>& raycasterCache() const { return m_raycasterCache; }

	const Ref<Material>& primitiveMeshDefaultMaterial() const { return m_primitiveMeshDefaultMaterial; };

    // テスト用の設定。
    // 0.9.0 以降、WorldObject は World へ自動追加しないようにしたい。
    // ただ、UnitTest が自動追加ありきで組まれているので、そちらの修正が終わるまでこの設定でカバーする。
	// Note: [2020/2/1] 小規模なプログラム作るのにはやっぱり億劫。Lumino で大規模開発するかっていうとまだまだ微妙なところがあるので、
	// のちのメジャーバージョンアップ時でいいだろう。まずは使い勝手重視で。
    bool autoAddingToActiveWorld = true;

private:
	void executeCommands();
	void releaseAndTerminateAllRunningScenes();

	enum class EventType
	{
		Goto,
		Call,
		Return,
		/*
		SMC_CHANGE_LOADING,
		SMC_PUSH_LOADING
		*/
	};

	struct EventCommsnd
	{
		EventType			type;
		Ref<Scene>   scene;
	};

	Ref<Scene>				m_activeScene;
	std::deque<EventCommsnd>		m_eventQueue;
	std::stack<Ref<Scene>>	m_sceneStack;	// not contains m_activeScene

    World* m_activeWorld;
    Ref<Shader> m_atmosphereShader;
	Ref<Raycaster> m_raycasterCache;

	Ref<Material> m_primitiveMeshDefaultMaterial;
};

} // namespace detail
} // namespace ln
