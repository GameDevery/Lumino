
#include "../Internal.h"
#include "MeshFactory.h"

LN_NAMESPACE_BEGIN
namespace detail {


//==============================================================================
// DirectXTK
//==============================================================================

#include <DirectXTK/TeapotData.inc>

static const Vector3 g_XMEpsilon( 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f );
static const Vector3 g_XMNegateX(-1.0f, 1.0f, 1.0f);
static const Vector3 g_XMNegateY(1.0f, -1.0f, 1.0f);
static const Vector3 g_XMNegateZ(1.0f, 1.0f, -1.0f);
static const Vector3 g_XMIdentityR1(0.0f, 1.0f, 0.0f);
static const Vector3 g_XMNegIdentityR1(0.0f,-1.0f, 0.0f);

namespace Bezier
{
	inline Vector3 XMVectorLess
	(
		Vector3 V1,
		Vector3 V2
	)
	{
		Vector3 Control;
		Control.x = (V1.x < V2.x) ? 1 : 0;
		Control.y = (V1.y < V2.y) ? 1 : 0;
		Control.z = (V1.z < V2.z) ? 1 : 0;
		return Control;
	}


	inline Vector3 XMVectorSelect
	(
		Vector3 V1,
		Vector3 V2,
		Vector3 Control
	)
	{
		Vector3 Result;
		Result.x = (Control.x == 0) ? V1.x : V2.x;
		Result.y = (Control.y == 0) ? V1.y : V2.y;
		Result.z = (Control.z == 0) ? V1.z : V2.z;
		return Result;
	}

	template<typename T>
	T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
	{
		return p1 * (1 - t) * (1 - t) * (1 - t) +
			p2 * 3 * t * (1 - t) * (1 - t) +
			p3 * 3 * t * t * (1 - t) +
			p4 * t * t * t;
	}

	template<typename T>
	T CubicTangent(T const& p1, T const& p2, T const& p3, T const& p4, float t)
	{
		return p1 * (-1 + 2 * t - t * t) +
			p2 * (1 - 4 * t + 3 * t * t) +
			p3 * (2 * t - 3 * t * t) +
			p4 * (t * t);
	}

	// Creates vertices for a patch that is tessellated at the specified level.
	// Calls the specified outputVertex function for each generated vertex,
	// passing the position, normal, and texture coordinate as parameters.
	template<typename TOutputFunc>
	void CreatePatchVertices(
		//_In_reads_(16) DirectX::XMVECTOR patch[16],
		const Vector3* patch,
		size_t tessellation,
		bool isMirrored,
		TOutputFunc outputVertex)
	{
		for (size_t i = 0; i <= tessellation; i++)
		{
			float u = (float)i / tessellation;

			for (size_t j = 0; j <= tessellation; j++)
			{
				float v = (float)j / tessellation;

				// Perform four horizontal bezier interpolations
				// between the control points of this patch.
				Vector3 p1 = CubicInterpolate(patch[0], patch[1], patch[2], patch[3], u);
				Vector3 p2 = CubicInterpolate(patch[4], patch[5], patch[6], patch[7], u);
				Vector3 p3 = CubicInterpolate(patch[8], patch[9], patch[10], patch[11], u);
				Vector3 p4 = CubicInterpolate(patch[12], patch[13], patch[14], patch[15], u);

				// Perform a vertical interpolation between the results of the
				// previous horizontal interpolations, to compute the position.
				Vector3 position = CubicInterpolate(p1, p2, p3, p4, v);

				// Perform another four bezier interpolations between the control
				// points, but this time vertically rather than horizontally.
				Vector3 q1 = CubicInterpolate(patch[0], patch[4], patch[8], patch[12], v);
				Vector3 q2 = CubicInterpolate(patch[1], patch[5], patch[9], patch[13], v);
				Vector3 q3 = CubicInterpolate(patch[2], patch[6], patch[10], patch[14], v);
				Vector3 q4 = CubicInterpolate(patch[3], patch[7], patch[11], patch[15], v);

				// Compute vertical and horizontal tangent vectors.
				Vector3 tangent1 = CubicTangent(p1, p2, p3, p4, v);
				Vector3 tangent2 = CubicTangent(q1, q2, q3, q4, u);

				// Cross the two tangent vectors to compute the normal.
				Vector3 normal = Vector3::Cross(tangent1, tangent2);

				if (!Vector3::NearEqual(normal, Vector3::Zero/*, g_XMEpsilon*/))
				{
					normal = Vector3::Normalize(normal);

					// If this patch is mirrored, we must invert the normal.
					if (isMirrored)
					{
						normal = -normal;
					}
				}
				else
				{
					// In a tidy and well constructed bezier patch, the preceding
					// normal computation will always work. But the classic teapot
					// model is not tidy or well constructed! At the top and bottom
					// of the teapot, it contains degenerate geometry where a patch
					// has several control points in the same place, which causes
					// the tangent computation to fail and produce a zero normal.
					// We 'fix' these cases by just hard-coding a normal that points
					// either straight up or straight down, depending on whether we
					// are on the top or bottom of the teapot. This is not a robust
					// solution for all possible degenerate bezier patches, but hey,
					// it's good enough to make the teapot work correctly!

					normal = XMVectorSelect(g_XMIdentityR1, g_XMNegIdentityR1, XMVectorLess(position, Vector3::Zero));
				}

				// Compute the texture coordinate.
				float mirroredU = isMirrored ? 1 - u : u;

				Vector2 textureCoordinate(mirroredU, v);

				// Output this vertex.
				outputVertex(position, normal, textureCoordinate);
			}
		}
	}


	// Creates indices for a patch that is tessellated at the specified level.
	// Calls the specified outputIndex function for each generated index value.
	template<typename TOutputFunc>
	void CreatePatchIndices(size_t tessellation, bool isMirrored, TOutputFunc outputIndex)
	{
		size_t stride = tessellation + 1;

		for (size_t i = 0; i < tessellation; i++)
		{
			for (size_t j = 0; j < tessellation; j++)
			{
				// Make a list of six index values (two triangles).
				std::array<size_t, 6> indices =
				{
					i * stride + j,
					(i + 1) * stride + j,
					(i + 1) * stride + j + 1,

					i * stride + j,
					(i + 1) * stride + j + 1,
					i * stride + j + 1,
				};

				// If this patch is mirrored, reverse indices to fix the winding order.
				if (isMirrored)
				{
					std::reverse(indices.begin(), indices.end());
				}

				// Output these index values.
				std::for_each(indices.begin(), indices.end(), outputIndex);
			}
		}
	}

} // namespace Bezier

//==============================================================================
// TeapotMeshFactory
//==============================================================================

void TeapotMeshFactory::ComputeTeapot(float size, size_t tessellation/*, bool rhcoords*/)
{
	if (tessellation < 1)
		throw std::out_of_range("tesselation parameter out of range");

	Vector3 scaleVector = Vector3::Replicate(size);

	Vector3 scaleNegateX = scaleVector * g_XMNegateX;
	Vector3 scaleNegateZ = scaleVector * g_XMNegateZ;
	Vector3 scaleNegateXZ = scaleVector * g_XMNegateX * g_XMNegateZ;

	for (int i = 0; i < sizeof(TeapotPatches) / sizeof(TeapotPatches[0]); i++)
	{
		const TeapotPatch& patch = TeapotPatches[i];

		// Because the teapot is symmetrical from left to right, we only store
		// data for one side, then tessellate each patch twice, mirroring in X.
		// �e�B�[�|�b�g�͍�����E�ɑΏ̂ł��邽�߁A�Б��݂̂̃f�[�^��ۑ����A�e�p�b�`��2��e�b�Z���[�V��������X�Ń~���[�����O���܂��B
		TessellatePatch(patch, tessellation, scaleVector, false);
		TessellatePatch(patch, tessellation, scaleNegateX, true);

		if (patch.mirrorZ)
		{
			// Some parts of the teapot (the body, lid, and rim, but not the
			// handle or spout) are also symmetrical from front to back, so
			// we tessellate them four times, mirroring in Z as well as X.
			// �e�B�[�|�b�g�̈ꕔ�i�{�f�B�A�t�^�A�����A�������n���h����X�p�E�g�ł͂���܂���j���O�����ɑΏ̂ł��̂ŁA������4��e�b�Z���[�V�������AZ��X���~���[�����O���܂��B
			TessellatePatch(patch, tessellation, scaleNegateZ, true);
			TessellatePatch(patch, tessellation, scaleNegateXZ, false);
		}
	}

	// Built RH above
	//if (!rhcoords)
	//	ReverseWinding(indices, vertices);
}

//------------------------------------------------------------------------------
void TeapotMeshFactory::TessellatePatch(const TeapotPatch& patch, size_t tessellation, const Vector3& scale, bool isMirrored)
{
	// Look up the 16 control points for this patch.
	Vector3 controlPoints[16];

	for (int i = 0; i < 16; i++)
	{
		controlPoints[i] = TeapotControlPoints[patch.indices[i]] * scale;
	}

	// Create the index data.
	size_t vbase = m_vbPos - m_vbBegin;
	Bezier::CreatePatchIndices(tessellation, isMirrored, [&](size_t index)
	{
		AddIndex(vbase + index);
	});

	// Create the vertex data.
	Bezier::CreatePatchVertices(controlPoints, tessellation, isMirrored, [&](
		const Vector3& position,
		const Vector3& normal,
		const Vector2& textureCoordinate)
	{
		AddVertex(position, normal, textureCoordinate);
	});
}

//------------------------------------------------------------------------------
static int GetPatchBaseCalls()
{
	int baseCall = 0;

	for (int i = 0; i < sizeof(TeapotPatches) / sizeof(TeapotPatches[0]); i++)
	{
		const TeapotPatch& patch = TeapotPatches[i];
		baseCall += 2;
		if (patch.mirrorZ)
		{
			baseCall += 2;
		}
	}

	return baseCall;
}

//------------------------------------------------------------------------------
TeapotMeshFactory::TeapotMeshFactory()
	: m_size(0.0f)
	, m_tessellation(0)
	, m_vbBegin(nullptr)
	, m_vbPos(nullptr)
	, m_ibPos(nullptr)
	, m_beginIndex(0)
{
}

//------------------------------------------------------------------------------
void TeapotMeshFactory::Initialize(float size, int tessellation, const Color& color, const Matrix& transform)
{
	LN_FAIL_CHECK_ARG(tessellation >= 1) return;
	m_size = size;
	m_tessellation = tessellation;
	MeshFactoryBase::Initialize(color, transform);
}

//------------------------------------------------------------------------------
int TeapotMeshFactory::GetVertexCount() const
{
	int tess = m_tessellation + 1;
	return GetPatchBaseCalls() * (tess * tess);
}

//------------------------------------------------------------------------------
int TeapotMeshFactory::GetIndexCount() const
{
	return GetPatchBaseCalls() * (m_tessellation * m_tessellation) * 6;
}

//------------------------------------------------------------------------------
void TeapotMeshFactory::Generate(Vertex* outVertices, uint16_t* outIndices, uint16_t beginIndex)
{
	m_vbBegin = outVertices;
	m_vbPos = m_vbBegin;
	m_ibPos = outIndices;
	m_beginIndex = beginIndex;

	ComputeTeapot(m_size, m_tessellation/*, rhcoords*/);
}

//------------------------------------------------------------------------------
void TeapotMeshFactory::AddVertex(const Vector3& pos, const Vector3& normal, const Vector2& texUV)
{
	m_vbPos->position = pos;
	m_vbPos->normal = normal;
	m_vbPos->uv = texUV;
	m_vbPos->color = m_color;
	m_vbPos++;
}

//------------------------------------------------------------------------------
void TeapotMeshFactory::AddIndex(uint16_t index)
{
	*m_ibPos = m_beginIndex + index;
	m_ibPos++;
}

} // namespace detail
LN_NAMESPACE_END
