﻿/*
	MeshResource 注意点

	requestXXXX 系の関数は、ユーザーが要求したサイズなどの情報に応じて VertexBuffer と IndexBuffer の遅延構築を行う。
	m_vertexBuffers と m_indexBuffer に直接アクセスしてよいのは requestXXXX 系の関数だけ。
	それ以外でこれらのデータにアクセスしたい場合は必ず requestXXXX 系の関数でインスタンスを取得すること。
*/
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include "MeshManager.hpp"

namespace ln {

//==============================================================================
// MeshResource

const std::array<size_t, MeshResource::VBG_Count> MeshResource::VertexStrideTable =
{
	sizeof(Vertex),
	sizeof(VertexBlendWeight),
	sizeof(VertexAdditionalUV),
	sizeof(VertexSdefInfo),
	sizeof(VertexMmdExtra),
};

MeshResource::MeshResource()
	: m_manager()
    , m_ownerContainer(nullptr)
	, m_vertexCount(0)
	, m_indexCount(0)
	, m_usage(GraphicsResourceUsage::Static)
	, m_vertexBuffers{}
	, m_indexBuffer(nullptr)
	, m_sections()
{
}

MeshResource::~MeshResource()
{
}

void MeshResource::init()
{
	Object::init();
	m_manager = detail::EngineDomain::meshManager();
}

int MeshResource::vertexCount() const
{
	return m_vertexCount;
}

int MeshResource::indexCount() const
{
	return m_indexCount;
}

int MeshResource::faceCount() const
{
	return indexCount() / 3;
}

void MeshResource::resizeVertexBuffer(int vertexCount)
{
	m_vertexCount = vertexCount;
}

void MeshResource::resizeIndexBuffer(int indexCount)
{
	m_indexCount = indexCount;
}

void MeshResource::resizeSections(int sectionCount)
{
	m_sections.resize(sectionCount);
}

void MeshResource::setVertex(int index, const Vertex& value)
{
	if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;

	Vertex* v = (Vertex*)requestVertexData(VBG_Basic);
	*(v + index) = value;
}

const Vertex& MeshResource::vertex(int index)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return Vertex::Default;

    Vertex* v = (Vertex*)requestVertexData(VBG_Basic);
    return *(v + index);
}

void MeshResource::setBlendWeights(int index, float v0, float v1, float v2, float v3)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexBlendWeight* v = ((VertexBlendWeight*)requestVertexData(VBG_BlendWeights) + index);
    v->weights[0] = v0;
    v->weights[1] = v1;
    v->weights[2] = v2;
    v->weights[3] = v3;
}

void MeshResource::setBlendIndices(int index, float v0, float v1, float v2, float v3)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexBlendWeight* v = ((VertexBlendWeight*)requestVertexData(VBG_BlendWeights) + index);
    v->indices[0] = v0;
    v->indices[1] = v1;
    v->indices[2] = v2;
    v->indices[3] = v3;
}

const VertexBlendWeight& MeshResource::vertexBlendWeight(int index)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return VertexBlendWeight::Default;
    return *((VertexBlendWeight*)requestVertexData(VBG_BlendWeights) + index);
}

void MeshResource::setSdefInfo(int index, const Vector4& sdefC, const Vector3& sdefR0, const Vector3& sdefR1)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexSdefInfo* v = ((VertexSdefInfo*)requestVertexData(VBG_SdefInfo) + index);
    v->sdefC = sdefC;
    v->sdefR0 = sdefR0;
    v->sdefR1 = sdefR1;
}

void MeshResource::setMmdExtra(int index, float edgeWeight, float vertexIndex)
{
    if (LN_REQUIRE_RANGE(index, 0, vertexCount())) return;
    VertexMmdExtra* v = ((VertexMmdExtra*)requestVertexData(VBG_MmdExtra) + index);
    v->edgeWeight = edgeWeight;
    v->index = vertexIndex;
}

void MeshResource::setIndex(int index, int value)
{
	if (LN_REQUIRE_RANGE(index, 0, indexCount())) return;

	IndexBuffer* indexBuffer;
	requestBuffers(VBG_Basic, nullptr, &indexBuffer);
	indexBuffer->setIndex(index, value);
}

int MeshResource::index(int index)
{
    if (LN_REQUIRE_RANGE(index, 0, indexCount())) return 0;

    IndexBuffer* indexBuffer;
    requestBuffers(VBG_Basic, nullptr, &indexBuffer);
    return indexBuffer->index(index);
}

void MeshResource::setSection(int sectionIndex, int startIndex, int primitiveCount, int materialIndex)
{
	MeshSection& section = m_sections[sectionIndex];
	section.startIndex = startIndex;
	section.primitiveCount = primitiveCount;
	section.materialIndex = materialIndex;
}

void MeshResource::addSection(int startIndex, int primitiveCount, int materialIndex)
{
    MeshSection section;
    section.startIndex = startIndex;
    section.primitiveCount = primitiveCount;
    section.materialIndex = materialIndex;
    m_sections.add(section);
}

int MeshResource::realVertexCount(VertexBufferGroup group) const
{
	if (m_vertexBuffers[group]) {
		return m_vertexBuffers[group]->size() / sizeof(Vertex);
	}
	return 0;
}

int MeshResource::realIndexCount() const
{
	if (m_indexBuffer) {
		return m_indexBuffer->size();
	}
	return 0;
}

void MeshResource::requestBuffers(VertexBufferGroup group, VertexBuffer** outVertexBuffer, IndexBuffer** outIndexBuffer)
{
	// prepare vertex buffers
	if (!m_vertexBuffers[group]) {
		m_vertexBuffers[group] = ln::makeObject<VertexBuffer>(m_vertexCount * VertexStrideTable[group], m_usage);
	}

	// sync vertex buffers size
	for (int i = 0; i < m_vertexBuffers.size(); i++) {
		if (m_vertexBuffers[i] && realVertexCount((VertexBufferGroup)i) != m_vertexCount) {
			m_vertexBuffers[i]->resize(m_vertexCount * VertexStrideTable[i]);
		}
	}

	if (outVertexBuffer) {
		*outVertexBuffer = m_vertexBuffers[group];
	}

    // Create index buffer if requested
	if (outIndexBuffer) {
        // prepare index buffer
        if (!m_indexBuffer) {
            m_indexBuffer = ln::makeObject<IndexBuffer>(m_indexCount, detail::GraphicsResourceInternal::selectIndexBufferFormat(m_vertexCount), m_usage);
        }
        else if (realIndexCount() != m_indexCount) {
            m_indexBuffer->resize(m_indexCount);
        }

        // check index buffer format
        if (m_indexBuffer->format() == IndexBufferFormat::UInt16) {
            if (m_vertexCount > 0xFFFF) {
                m_indexBuffer->setFormat(IndexBufferFormat::UInt32);
            }
        }
		*outIndexBuffer = m_indexBuffer;
	}
}

void* MeshResource::requestVertexData(VertexBufferGroup group)
{
	VertexBuffer* vertexBuffer;
	requestBuffers(group, &vertexBuffer, nullptr);

	return vertexBuffer->map(MapMode::Write);
}

void* MeshResource::requestIndexData(IndexBufferFormat* outFormat)
{
	IndexBuffer* indexBuffer;
	requestBuffers(VBG_Basic, nullptr, &indexBuffer);

	if (outFormat) {
		*outFormat = indexBuffer->format();
	}
	return indexBuffer->map(MapMode::Write);
}

void* MeshResource::requestVertexDataForAdditional(int additionalVertexCount, VertexBufferGroup group)
{
	LN_CHECK(additionalVertexCount > 0);

	int begin = realVertexCount(group);
	int newCount = begin + additionalVertexCount;

	// resize all vertex buffers
	resizeVertexBuffer(newCount);

	VertexBuffer* vertexBuffer;
	requestBuffers(group, &vertexBuffer, nullptr);

	Vertex* data = (Vertex*)vertexBuffer->map(MapMode::Write);
	return data + begin;
}

void* MeshResource::requestIndexDataForAdditional(int additionalIndexCount, IndexBufferFormat* outFormat)
{
	int begin = realIndexCount();
	int newCount = begin + additionalIndexCount;

	// resize all vertex buffers
	resizeIndexBuffer(newCount);

	IndexBuffer* indexBuffer;
	requestBuffers(VBG_Basic, nullptr, &indexBuffer);

	if (outFormat) {
		*outFormat = indexBuffer->format();
	}

	void* data = indexBuffer->map(MapMode::Write);
	if (indexBuffer->format() == IndexBufferFormat::UInt16)
		return ((uint16_t*)data) + begin;
	else
		return ((uint32_t*)data) + begin;
}

void MeshResource::commitRenderData(int sectionIndex, MeshSection* outSection, VertexLayout** outDecl, VertexBuffer** outVBs, int* outVBCount, IndexBuffer** outIB)
{
	// Section
	*outSection = m_sections[sectionIndex];

	// VertexLayout
	{
		uint32_t flags = 0;
		if (m_vertexBuffers[VBG_Basic]) flags |= detail::PredefinedVertexLayoutFlags_Geometry;
		if (m_vertexBuffers[VBG_BlendWeights]) flags |= detail::PredefinedVertexLayoutFlags_BlendWeights;
		if (m_vertexBuffers[VBG_AdditionalUVs]) flags |= detail::PredefinedVertexLayoutFlags_AdditionalUV;
		if (m_vertexBuffers[VBG_SdefInfo]) flags |= detail::PredefinedVertexLayoutFlags_SdefInfo;
		if (m_vertexBuffers[VBG_MmdExtra]) flags |= detail::PredefinedVertexLayoutFlags_MmdExtra;
        if (LN_REQUIRE(flags != 0)) return;
		*outDecl = m_manager->getPredefinedVertexLayout((detail::PredefinedVertexLayoutFlags)flags);
	}

	// VertexBuffer
	int vbCount = 0;
	for (int i = 0; i < VBG_Count; ++i)
	{
		if (m_vertexBuffers[i])
		{
			outVBs[vbCount] = m_vertexBuffers[i];
			++vbCount;
		}
	}
	*outVBCount = vbCount;
	
	// IndexBuffer
	*outIB = m_indexBuffer;
}

bool MeshResource::isInitialEmpty() const
{
    for (auto& buf : m_vertexBuffers) {
        if (buf) {
            return false;
        }
    }
    return true;
}

//==============================================================================
// Mesh

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::init()
{
	Object::init();
}

void Mesh::init(const std::vector<SectionView>& sectionViews)
{
	init();

	for (auto& section : sectionViews) {
		int count = section.vertexBufferViews[0].count;
		for (size_t i = 1; i < section.vertexBufferViews.size(); i++) {
			if (count != section.vertexBufferViews[i].count) {
				// ひとつの section 内ではすべての頂点数が一致していることが前提
				LN_ERROR();
				return;
			}
		}
	}



}

//==============================================================================
// MeshContainer

MeshContainer::MeshContainer()
	: m_name()
	, m_lodResources()
{
}

MeshContainer::~MeshContainer()
{
}

void MeshContainer::init()
{
	Object::init();
	m_lodResources.resize(1);
}

void MeshContainer::setMeshResource(MeshResource* mesh)
{
    if (LN_REQUIRE(!mesh->m_ownerContainer)) return;
    m_lodResources[0] = mesh;
    mesh->m_ownerContainer = this;
}

MeshResource* MeshContainer::meshResource() const
{
	return m_lodResources[0];
}

MeshResource* MeshContainer::selectLODResource(float distance) const
{
	// TODO:
	return m_lodResources[0];
}

void MeshContainer::calculateBounds()
{
	MeshResource* mesh = meshResource();
	if (mesh)
	{
		VertexBuffer* vertexBuffer;
		mesh->requestBuffers(MeshResource::VBG_Basic, &vertexBuffer, nullptr);
		Vertex* data = (Vertex*)vertexBuffer->map(MapMode::Read);

		int count = mesh->vertexCount();
		if (count > 0)
		{
			Vector3 min = data[0].position;
			Vector3 max = data[0].position;
			for (int i = 1; i < count; i++)
			{
				if (data[i].position.x < min.x)
					min.x = data[i].position.x;

				if (data[i].position.y < min.y)
					min.y = data[i].position.y;

				if (data[i].position.z < min.z)
					min.z = data[i].position.z;

				if (data[i].position.x > max.x)
					max.x = data[i].position.x;

				if (data[i].position.y > max.y)
					max.y = data[i].position.y;

				if (data[i].position.z > max.z)
					max.z = data[i].position.z;
			}

			m_boundingBox = Box(min, max);
		}
		else
		{
			m_boundingBox = Box();
		}
	}
	else
	{
		m_boundingBox = Box();
	}
}

//==============================================================================
// StaticMeshModel

StaticMeshModel::StaticMeshModel()
    : m_type(detail::InternalMeshModelType::StaticMesh)
{
}

StaticMeshModel::StaticMeshModel(detail::InternalMeshModelType type)
    : m_type(type)
{
}

void StaticMeshModel::addMeshContainer(MeshContainer* meshContainer)
{
	if (LN_REQUIRE(meshContainer)) return;
	m_meshContainers.add(meshContainer);
}

void StaticMeshModel::addMaterial(AbstractMaterial* material)
{
	m_materials.add(material);
}

namespace detail {


} // namespace detail
} // namespace ln

