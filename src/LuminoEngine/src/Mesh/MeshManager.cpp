﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include "MeshManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshManager
	
MeshManager::MeshManager()
	: m_graphicsManager(nullptr)
	//, m_predefinedVertexLayouts{}
{
}

void MeshManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;

	//{
	//	static VertexElement elements[] =
	//	{
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
	//		{ 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
	//		{ 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
	//	};
	//	m_predefinedVertexLayouts[PredefinedVertexLayout_Standard] = newObject<VertexDeclaration>(elements, 4);
	//}

	//{
	//	static VertexElement elements[] =
	//	{
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
	//		{ 0, VertexElementType::Float3, VertexElementUsage::Normal, 0 },
	//		{ 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
	//		{ 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
	//		{ 1, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0 },
	//		{ 1, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0 },
	//	};
	//	m_predefinedVertexLayouts[PredefinedVertexLayout_Standard_BlendWeight] = newObject<VertexDeclaration>(elements, 4);
	//}
}

void MeshManager::dispose()
{
	m_predefinedVertexLayouts.clear();
}

VertexDeclaration* MeshManager::getPredefinedVertexLayout(PredefinedVertexLayoutFlags flags)
{
	auto itr = m_predefinedVertexLayouts.find(flags);
	if (itr != m_predefinedVertexLayouts.end()) {
		return itr->second;
	}
	else {
		auto vertexLayout = newObject<VertexDeclaration>();
		int stream = 0;

		// BasicVertices
		if ((flags & PredefinedVertexLayoutFlags_Geometry) != 0)
		{
			vertexLayout->addVertexElement(stream, VertexElementType::Float3, VertexElementUsage::Position, 0);
			vertexLayout->addVertexElement(stream, VertexElementType::Float3, VertexElementUsage::Normal, 0);
			vertexLayout->addVertexElement(stream, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::Color, 0);
			++stream;
		}

		// BlendWeights
		if ((flags & PredefinedVertexLayoutFlags_BlendWeights) != 0)
		{
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::BlendWeight, 0);
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::BlendIndices, 0);
			++stream;
		}

		// AdditionalUVs
		if ((flags & PredefinedVertexLayoutFlags_AdditionalUV) != 0)
		{
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 1);
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 2);
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 3);
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 4);
			++stream;
		}

		// SdefInfo
		if ((flags & PredefinedVertexLayoutFlags_SdefInfo) != 0)
		{
			vertexLayout->addVertexElement(stream, VertexElementType::Float4, VertexElementUsage::TexCoord, 5);
			vertexLayout->addVertexElement(stream, VertexElementType::Float3, VertexElementUsage::TexCoord, 6);
			vertexLayout->addVertexElement(stream, VertexElementType::Float3, VertexElementUsage::TexCoord, 7);
			++stream;
		}

		// MmdExtra
		if ((flags & PredefinedVertexLayoutFlags_MmdExtra) != 0)
		{
			vertexLayout->addVertexElement(stream, VertexElementType::Float1, VertexElementUsage::TexCoord, 8);
			vertexLayout->addVertexElement(stream, VertexElementType::Float1, VertexElementUsage::PointSize, 15);
			++stream;
		}

		m_predefinedVertexLayouts.insert({ flags, vertexLayout });

		return vertexLayout;
	}
}

} // namespace detail
} // namespace ln

