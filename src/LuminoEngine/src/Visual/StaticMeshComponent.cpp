﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Visual/StaticMeshComponent.hpp>

namespace ln {

//=============================================================================
// StaticMeshComponent

StaticMeshComponent::StaticMeshComponent()
    : m_model(nullptr)
{
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::initialize()
{
    VisualComponent::initialize();
}

void StaticMeshComponent::setModel(MeshModel* model)
{
    m_model = model;
}

void StaticMeshComponent::onRender(RenderingContext* context)
{
    for (auto& meshContainer : m_model->meshContainers())
    {
        MeshResource* meshResource = meshContainer->meshResource();
        for (int iSection = 0; iSection < meshResource->sections().size(); iSection++)
        {
            context->setMaterial(m_model->materials()[meshResource->sections()[iSection].materialIndex]);
            context->drawMesh(meshResource, iSection);
        }
    }
}

} // namespace ln

