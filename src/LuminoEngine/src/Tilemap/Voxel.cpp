﻿
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>

namespace ln {

//==============================================================================
// VoxelmapModel

LN_OBJECT_IMPLEMENT(VoxelmapModel, Object)
{
    context->registerType<VoxelmapModel>({});
}

VoxelmapModel::VoxelmapModel()
{
}

void VoxelmapModel::init()
{
    Object::init();
    resizeMap(16, 16, 16);
    m_box = StaticMeshModel::load(u"D:/Proj/LN/PrivateProjects/HC0/Assets/AutoVolume1.glb");
    m_mapBlocksMesh = makeObject<StaticMeshModel>();
}

void VoxelmapModel::resizeMap(int width, int height, int depth)
{
    m_width = width;
    m_height = height;
    m_depth = depth;
    m_mapData.resize(m_width * m_height * m_depth);
}

void VoxelmapModel::render(RenderingContext* context)
{
    for (const auto& node : m_box->meshNodes()) {
        if (node->meshContainerIndex() >= 0) {
            context->setTransfrom(m_box->nodeGlobalTransform(node->index()));


            const auto& meshContainer = m_box->meshContainers()[node->meshContainerIndex()];

            Mesh* mesh = meshContainer->mesh();
            if (mesh) {
                for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
                    context->setMaterial(m_box->materials()[mesh->sections()[iSection].materialIndex]);
                    context->drawMesh(mesh, iSection);
                }
            }

        }
    }

}

void VoxelmapModel::serialize(Archive& ar)
{
    Object::serialize(ar);
}

} // namespace ln




namespace ln {

//==============================================================================
// VoxelmapComponent

LN_OBJECT_IMPLEMENT(VoxelmapComponent, VisualComponent)
{
    context->registerType<VoxelmapComponent>({});
}

VoxelmapComponent::VoxelmapComponent()
{
}

void VoxelmapComponent::init()
{
    VisualComponent::init();
    m_model = makeObject<VoxelmapModel>();
}

void VoxelmapComponent::onRender(RenderingContext* context)
{
    m_model->render(context);
}

void VoxelmapComponent::serialize(Archive& ar)
{
    VisualComponent::serialize(ar);
}

} // namespace ln