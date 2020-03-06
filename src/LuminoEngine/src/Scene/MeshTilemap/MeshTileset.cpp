﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>

namespace ln {

//==============================================================================
// AutoMeshTileset

AutoMeshTileset::AutoMeshTileset()
{
}

void AutoMeshTileset::init()
{
	Object::init();
}

//==============================================================================
// MeshTileset

LN_OBJECT_IMPLEMENT(MeshTileset, Object) {}


const MeshTileset::AutoTileInfo MeshTileset::AutoTileTable[48] =
{
	// Block tiles.
	/*[0]*/  {1,1,1,1},{2,1,1,1},{3,3,1,1},{1,2,1,1}, {2,2,1,1},{4,1,4,1},{5,3,4,1},{4,2,4,1},
	/*[8]*/  {1,4,1,4},{2,4,1,4},{3,5,1,4},{4,4,4,4}, {5,5,4,4},{1,1,2,1},{2,1,2,1},{3,3,2,1},
	/*[16]*/ {1,2,2,1},{2,2,2,1},{4,3,4,1},{1,4,2,4}, {2,4,2,4},{3,5,2,4},{1,1,3,3},{2,1,3,3},
	/*[24]*/ {3,3,3,3},{1,2,3,3},{2,2,3,3},{4,1,5,3}, {5,3,5,3},{4,2,5,3},{1,4,3,5},{2,4,3,5},
	/*[32]*/ {3,5,3,5},{4,4,5,5},{5,5,5,5},{1,1,1,2}, {2,1,1,2},{3,3,1,2},{1,2,1,2},{2,2,1,2},
	/*[40]*/ {4,1,4,2},{5,3,4,2},{4,2,4,2},{1,1,2,2}, {2,1,2,2},{3,3,2,2},{1,2,2,2},{2,2,2,2},
};

// SubTileId([0] is invalid) => PhysicalTileIndex
static int g_AutoTileSourcePosTable_TkoolXP[4][6] =	// TkoolXP and Wolf
{
	// [top-left]
	{ -1, 26, 4, 14, 24, 12 },
	// [top-right]
	{ -1, 27, 5, 15, 29, 17 },
	// [bottom-left]
	{ -1, 32, 10, 44, 30, 42 },
	// [bottom-light]
	{ -1, 33, 11, 45, 35, 47 },
};


MeshTileset::MeshTileset()
{
}

void MeshTileset::init()
{
    Object::init();

	auto tex = Texture2D::load(u"autotile1");
	int subtilePixelSize = 16;
	int hc = 6;
	int vc = 8;
	//auto subTileUVSize = Vector2(static_cast<float>(hc) / tex->width(), static_cast<float>(vc) / tex->height());
	auto subTileUVSize = Vector2(1.0f / static_cast<float>(hc), 1.0f / static_cast<float>(vc));

	m_mesh = makeObject<Mesh>((4 * 4 * 48) * 6, (6 * 4 * 48) * 6);

	// ZMinus を、指定方向に向けるための変換行列
	const auto finalOffset = Vector3(0.5, 0.5, 0.5);
	Matrix faceTransforms[6] = {
		Matrix::makeRotationY(Math::PI / 2) * Matrix::makeTranslation(Vector3(-0.5, 0, 0) + finalOffset),
		Matrix::makeRotationY(-Math::PI / 2) * Matrix::makeTranslation(Vector3(0.5, 0, 0) + finalOffset),
		Matrix::makeRotationZ(Math::PI) * Matrix::makeRotationX(-Math::PI / 2) * Matrix::makeTranslation(Vector3(0, -0.5, 0) + finalOffset),
		Matrix::makeRotationX(Math::PI / 2) * Matrix::makeTranslation(Vector3(0, 0.5, 0) + finalOffset),
		Matrix::Identity * Matrix::makeTranslation(Vector3(0, 0, -0.5) + finalOffset),
		Matrix::makeRotationY(Math::PI) * Matrix::makeTranslation(Vector3(0, 0, 0.5) + finalOffset),
	};

	int offsetV = 0;
	int offsetI = 0;

	for (int iFaceDir = 0; iFaceDir < 6; iFaceDir++) {
		const Matrix& transform = faceTransforms[iFaceDir];

		for (int i = 0; i < 48; i++) {
			const auto& info = AutoTileTable[i];
			int startIndex = offsetI;
			Vector3 pysOffsets[4] = { { -0.5, +0.5, 0.0 }, { 0.0, +0.5, 0.0 }, { -0.5, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } };

			// [top-left], [top-right], [bottom-left], [bottom-light]
			for (int iCorner = 0; iCorner < 4; iCorner++) {
				int index_tl = g_AutoTileSourcePosTable_TkoolXP[iCorner][info.subtiles[iCorner]];
				auto uv = Vector2(subTileUVSize.x * (index_tl % hc), subTileUVSize.y * (index_tl / hc));
				auto p1 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0, 0, 0), transform);
				auto p2 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0.5, 0, 0), transform);
				auto p3 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0, -0.5, 0), transform);
				auto p4 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0.5, -0.5, 0), transform);
				m_mesh->setVertex(offsetV + 0, Vertex{ p1, Vector3::UnitZ, Vector2(uv.x, uv.y), Color::White });
				m_mesh->setVertex(offsetV + 1, Vertex{ p2, Vector3::UnitZ, Vector2(uv.x + subTileUVSize.x, uv.y), Color::White });
				m_mesh->setVertex(offsetV + 2, Vertex{ p3, Vector3::UnitZ, Vector2(uv.x, uv.y + subTileUVSize.y), Color::White });
				m_mesh->setVertex(offsetV + 3, Vertex{ p4, Vector3::UnitZ, Vector2(uv.x + subTileUVSize.x, uv.y + subTileUVSize.y), Color::White });
				m_mesh->setIndex(offsetI + 0, offsetV + 0);
				m_mesh->setIndex(offsetI + 1, offsetV + 1);
				m_mesh->setIndex(offsetI + 2, offsetV + 2);
				m_mesh->setIndex(offsetI + 3, offsetV + 2);
				m_mesh->setIndex(offsetI + 4, offsetV + 1);
				m_mesh->setIndex(offsetI + 5, offsetV + 3);
				offsetV += 4;
				offsetI += 6;
			}

			m_mesh->addSection(startIndex, 8, 0, PrimitiveTopology::TriangleList);
		}
	}

	m_material = Material::create(tex);
}

void MeshTileset::drawTile(RenderingContext* context, const detail::MeshTile& tile) const
{

	context->setMaterial(m_material);

	for (int d = 0; d < 6; d++) {
		if (tile.id[d] >= 1) {
			context->drawMesh(m_mesh, (d * 48) + tile.id[d]);
		}
	}
}

} // namespace ln

