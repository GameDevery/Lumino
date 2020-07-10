﻿
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
#include "../../LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../LuminoEngine/src/Scene/SceneManager.hpp"
using namespace ln;

class App_Experiment_Editor : public Application
{
	Ref<TransformControls> m_transformControls;

    virtual void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::camera()->addComponent(CameraOrbitControlComponent::create());
		ln::detail::EngineDomain::sceneManager()->m_editorMode = true;

		auto box1 = BoxMesh::create();
		box1->setPosition(-2, 0, 0);

		auto box2 = BoxMesh::create();
		box2->setPosition(2, 0, 0);

		//m_transformControls = Engine::renderView()->transformControls();
		//m_transformControls->setTarget(box);
	}

    virtual void onUpdate() override
    {
		if (Mouse::triggered(MouseButtons::Left)) {
			auto pos = Mouse::position();
			Engine::renderView()->findObjectInPoint(pos.x, pos.y);
		}
    }
};

void Experiment_Editor()
{
	App_Experiment_Editor app;
	detail::ApplicationHelper::run(&app);
}



