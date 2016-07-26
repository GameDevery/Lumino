﻿
#include "Internal.h"
#include "Device/ShaderVariableBase.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Mesh.h>
#include "GraphicsManager.h"

LN_NAMESPACE_BEGIN

class BoxMeshFactory
{
public:
	BoxMeshFactory(const Vector3& size)
	{
		m_size = size;
	}

	int GetVertexCount() const
	{
		return 8;
	}

	int GetIndexCount() const
	{
		return 36;
	}

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);

		// 手前 (Z-)
		outVertices[0].position.Set(minPos.x, maxPos.y, minPos.z);	// 左上
		outVertices[0].uv.Set(0.0f, 0.0f);
		outVertices[1].position.Set(minPos.x, minPos.y, minPos.z);	// 左下
		outVertices[1].uv.Set(0.0f, 1.0f);
		outVertices[2].position.Set(maxPos.x, maxPos.y, minPos.z);	// 右上
		outVertices[2].uv.Set(1.0f, 0.0f);
		outVertices[3].position.Set(maxPos.x, minPos.y, minPos.z);	// 右下
		outVertices[3].uv.Set(1.0f, 1.0f);
		// 奥 (Z+)
		outVertices[4].position.Set(minPos.x, maxPos.y, maxPos.z);	// 左上
		outVertices[4].uv.Set(1.0f, 0.0f);
		outVertices[5].position.Set(minPos.x, minPos.y, maxPos.z);	// 左下
		outVertices[5].uv.Set(1.0f, 1.0f);
		outVertices[6].position.Set(maxPos.x, maxPos.y, maxPos.z);	// 右上
		outVertices[6].uv.Set(0.0f, 0.0f);
		outVertices[7].position.Set(maxPos.x, minPos.y, maxPos.z);	// 右下
		outVertices[7].uv.Set(0.0f, 1.0f);

		for (int i = 0; i < 8; ++i) outVertices[i].color = Color::White;

		uint16_t indices[] =
		{
			// near
			0, 1, 2,
			2, 1, 3,
			// far
			6, 7, 4,
			4, 7, 5,
			// left
			4, 5, 0,
			0, 5, 1,
			// right
			2, 3, 6,
			6, 3, 7,
			// top
			4, 0, 6,
			6, 0, 2,
			// bottom (Z- が面として上方向)
			1, 5, 3,
			3, 5, 7,
		};
		memcpy(outIndices, indices, sizeof(indices));
	}

private:
	Vector3	m_size;
};



class TexUVBoxMeshFactory
{
public:
	TexUVBoxMeshFactory(const Vector3& size)
	{
		m_size = size;
	}

	int GetVertexCount() const
	{
		return 24;
	}

	int GetIndexCount() const
	{
		return 36;
	}

	static void SetV(Vertex* vertex, float x, float y, float z, float u, float v)
	{
		vertex->position.Set(x, y, z);
		vertex->uv.Set(u, v);
		vertex->color = Color::White;
	}
	static void SetI(uint16_t* index, uint16_t begin)
	{
		index[0] = begin + 0;
		index[1] = begin + 1;
		index[2] = begin + 2;
		index[3] = begin + 2;
		index[4] = begin + 1;
		index[5] = begin + 3;
	}

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);
		Vertex* v = outVertices;
		uint16_t* i = outIndices;

		// 手前 (Z-)
		SetV(v, minPos.x, maxPos.y, minPos.z, 0.0f, 0.0f); ++v;	// ┏
		SetV(v, minPos.x, minPos.y, minPos.z, 0.0f, 1.0f); ++v;	// ┗
		SetV(v, maxPos.x, maxPos.y, minPos.z, 1.0f, 0.0f); ++v;	// ┓
		SetV(v, maxPos.x, minPos.y, minPos.z, 1.0f, 1.0f); ++v;	// ┛
		SetI(i, 0); i += 6;

		// 奥 (Z+)
		SetV(v, maxPos.x, maxPos.y, maxPos.z, 0.0f, 0.0f); ++v;	// ┏
		SetV(v, maxPos.x, minPos.y, maxPos.z, 0.0f, 1.0f); ++v;	// ┗
		SetV(v, minPos.x, maxPos.y, maxPos.z, 1.0f, 0.0f); ++v;	// ┓
		SetV(v, minPos.x, minPos.y, maxPos.z, 1.0f, 1.0f); ++v;	// ┛
		SetI(i, 4); i += 6;

		// 左 (X-)
		SetV(v, minPos.x, maxPos.y, maxPos.z, 0.0f, 0.0f); ++v;	// ┏
		SetV(v, minPos.x, minPos.y, maxPos.z, 0.0f, 1.0f); ++v;	// ┗
		SetV(v, minPos.x, maxPos.y, minPos.z, 1.0f, 0.0f); ++v;	// ┓
		SetV(v, minPos.x, minPos.y, minPos.z, 1.0f, 1.0f); ++v;	// ┛
		SetI(i, 8); i += 6;

		// 右 (X+)
		SetV(v, maxPos.x, maxPos.y, minPos.z, 0.0f, 0.0f); ++v;	// ┏
		SetV(v, maxPos.x, minPos.y, minPos.z, 0.0f, 1.0f); ++v;	// ┗
		SetV(v, maxPos.x, maxPos.y, maxPos.z, 1.0f, 0.0f); ++v;	// ┓
		SetV(v, maxPos.x, minPos.y, maxPos.z, 1.0f, 1.0f); ++v;	// ┛
		SetI(i, 12); i += 6;

		// 下 (Y-)(Z- がUVの上方向)
		SetV(v, minPos.x, minPos.y, minPos.z, 0.0f, 0.0f); ++v;	// ┏
		SetV(v, minPos.x, minPos.y, maxPos.z, 0.0f, 1.0f); ++v;	// ┗
		SetV(v, maxPos.x, minPos.y, minPos.z, 1.0f, 0.0f); ++v;	// ┓
		SetV(v, maxPos.x, minPos.y, maxPos.z, 1.0f, 1.0f); ++v;	// ┛
		SetI(i, 16); i += 6;

		// 上 (Y+)(Z+ がUVの上方向)
		SetV(v, minPos.x, maxPos.y, maxPos.z, 0.0f, 0.0f); ++v;	// ┏
		SetV(v, minPos.x, maxPos.y, minPos.z, 0.0f, 1.0f); ++v;	// ┗
		SetV(v, maxPos.x, maxPos.y, maxPos.z, 1.0f, 0.0f); ++v;	// ┓
		SetV(v, maxPos.x, maxPos.y, minPos.z, 1.0f, 1.0f); ++v;	// ┛
		SetI(i, 20);
	}

private:
	Vector3	m_size;
};

// top と bottom は TriangleFan
class SphereMeshFactory
{
public:
	SphereMeshFactory()
		: m_radius(0)
		, m_slices(0)
		, m_stacks(0)
	{
	}
	SphereMeshFactory(float radius, int slices, int stacks)
	{
		Initialize(radius, slices, stacks);
	}

	void Initialize(float radius, int slices, int stacks)
	{
		//if (LN_CHECKEQ_ARG(radius <= 0.0f)) return;
		//if (LN_CHECKEQ_ARG(slices <= 1)) return;
		//if (LN_CHECKEQ_ARG(stacks <= 1)) return;
		m_radius = radius;
		m_slices = slices;
		m_stacks = stacks;
		MakeSinCosTable();
	}

	// Squashed

	int GetVertexCount() const
	{
		return (m_slices + 1) * (m_stacks + 1);
		//return 2 + m_slices * (m_stacks - 1);	// (top と bottom の 2 点) + リングの頂点数 * 重ねる数
	}

	int GetIndexCount() const
	{
		return m_slices * m_stacks * 6;
		//return 2 * m_slices + (m_stacks - 2) * (2 * m_slices);
	}

	//typedef uint16_t Face[3];
	//typedef uint16_t QuadFace[6];

	struct SinCos
	{
		float	sin;
		float	cos;
	};

	void Generate(Vertex* outVertices, uint16_t* outIndices)
	{
		Vertex* v = outVertices;
		uint16_t* i = (uint16_t*)outIndices;

		float stackUVStep = 1.0f / (m_stacks + 1);
		float stackUV = 0.0f;

		float sliceUVStep = 1.0f / (m_slices + 1);

		// XY 平面上の step
		float theta_step = Math::PI / m_stacks;
		float theta = 0.0f;//theta_step;

		// Z+ を起点とし、X- 方向へ回っていく

		// rings (Vertex)
		for (int iStack = 0; iStack < m_stacks + 1; ++iStack)
		{
			float sin_theta = sinf(theta);
			float cos_theta = cosf(theta);
			float sliceUV = 0.0f;

			for (int iSlice = 0; iSlice < m_slices + 1; ++iSlice)
			{
				// top
				if (iStack == 0)
				{
					v->normal.x = 0.0f;
					v->normal.y = 1.0f;
					v->normal.z = 0.0f;
				}
				// bottom
				else if (iStack == m_stacks)
				{
					v->normal.x = 0.0f;
					v->normal.y = -1.0f;
					v->normal.z = 0.0f;
				}
				// between
				else
				{
					v->normal.x = sin_theta * m_sincosTable[iSlice].cos;
					v->normal.y = cos_theta;
					v->normal.z = sin_theta * m_sincosTable[iSlice].sin;
				}
				v->position = v->normal * m_radius;
				v->uv.x = sliceUV;
				v->uv.y = stackUV;
				++v;

				sliceUV += sliceUVStep;
			}

			stackUV += stackUVStep;
			theta += theta_step;
		}

		// faces
		for (int iStack = 0; iStack < m_stacks; ++iStack)
		{
			for (int iSlice = 0; iSlice < m_slices; ++iSlice)
			{
				int p1 = (iSlice + 0) + (iStack + 0) * (m_slices + 1);	// ┏
				int p2 = (iSlice + 0) + (iStack + 1) * (m_slices + 1);	// ┗
				int p3 = (iSlice + 1) + (iStack + 0) * (m_slices + 1);	// ┓
				int p4 = (iSlice + 1) + (iStack + 1) * (m_slices + 1);	// ┛
				i[0] = p1;
				i[1] = p2;
				i[2] = p3;
				i[3] = p3;
				i[4] = p2;
				i[5] = p4;
				i += 6;
			}
		}

#if 0
		Face* faces = (Face*)outIndices;

		float theta_step = Math::PI / m_stacks;
		float theta = theta_step;	// XY 平面上の step

		int vertex = 0;
		int face = 0;

		// top
		vertices[vertex].normal.x = 0.0f;
		vertices[vertex].normal.y = 0.0f;
		vertices[vertex].normal.z = 1.0f;
		vertices[vertex].position.x = 0.0f;
		vertices[vertex].position.y = 0.0f;
		vertices[vertex].position.z = m_radius;
		vertex++;

		// top faces (triangle fan)
		for (int iSlice = 0; iSlice < m_slices; ++iSlice)
		{
			faces[face][0] = 0;
			faces[face][1] = iSlice + 1;
			faces[face][2] = iSlice;
			face++;
		}

		// rings
		for (int iStack = 0; iStack < m_stacks - 1; ++iStack)
		{
			float sin_theta = sinf(theta);
			float cos_theta = cosf(theta);

			for (int iSlice = 0; iSlice < m_slices; ++iSlice)
			{
				// vertex
				vertices[vertex].normal.x = sin_theta * m_sincosTable[iSlice].cos;
				vertices[vertex].normal.y = sin_theta * m_sincosTable[iSlice].sin;
				vertices[vertex].normal.z = cos_theta;
				vertices[vertex].position.x = m_radius * sin_theta * m_sincosTable[iSlice].cos;
				vertices[vertex].position.y = m_radius * sin_theta * m_sincosTable[iSlice].sin;
				vertices[vertex].position.z = m_radius * cos_theta;
				vertex++;

				// faces
				if (iSlice > 0)
				{
					faces[face][0] = vertex_index(m_slices, iSlice - 1, iStack - 1);
					faces[face][1] = vertex_index(m_slices, iSlice, iStack - 1);
					faces[face][2] = vertex_index(m_slices, iSlice - 1, iStack);
					face++;

					faces[face][0] = vertex_index(m_slices, iSlice, iStack - 1);
					faces[face][1] = vertex_index(m_slices, iSlice, iStack);
					faces[face][2] = vertex_index(m_slices, iSlice - 1, iStack);
					face++;
				}
			}
		}
#endif
	}

	static uint16_t vertex_index(UINT slices, int slice, int stack)
	{
		return stack*slices + slice + 1;
	}

	void MakeSinCosTable()
	{
		float phi_start = Math::PI / 2.0f;
		float phi_step = -2.0f * Math::PI / m_slices;
		m_sincosTable.Resize(m_slices + 1);

		float angle = phi_start;
		for (int i = 0; i < m_slices; ++i)
		{
			m_sincosTable[i].sin = std::sinf(angle);
			m_sincosTable[i].cos = std::cosf(angle);
			angle -= phi_step;
		}

		m_sincosTable[m_slices] = m_sincosTable[0];
	}

private:
	float	m_radius;
	int		m_slices;
	int		m_stacks;

	Array<SinCos>	m_sincosTable;
};

//==============================================================================
// MaterialList3
//==============================================================================

//------------------------------------------------------------------------------
MaterialList3::MaterialList3()
{
}

//------------------------------------------------------------------------------
MaterialList3::~MaterialList3()
{
}

//==============================================================================
// Material
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Material3, Object);

const String Material3::DiffuseParameter(_T("Diffuse"));
const String Material3::AmbientParameter(_T("Ambient"));
const String Material3::SpecularParameter(_T("Specular"));
const String Material3::EmissiveParameter(_T("Emissive"));
const String Material3::PowerParameter(_T("Power"));
const String Material3::MaterialTextureParameter(_T("MaterialTexture"));

//------------------------------------------------------------------------------
MaterialPtr Material3::Create()
{
	auto ptr = MaterialPtr::MakeRef();
	return ptr;
}

//------------------------------------------------------------------------------
Material3::Material3()
	: m_shader(nullptr)
	, m_valueList()
	, m_modifiedForMaterialInstance(false)
{
}

//------------------------------------------------------------------------------
Material3::~Material3()
{
}

//------------------------------------------------------------------------------
void Material3::SetShader(Shader* shader)
{
	m_shader = shader;
	LinkVariables();
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material3::SetIntParameter(const StringRef& name, int value)
{
	FindShaderValue(name)->SetInt(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material3::SetFloatParameter(const StringRef& name, float value)
{
	FindShaderValue(name)->SetFloat(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material3::SetVectorParameter(const StringRef& name, const Vector4& value)
{
	FindShaderValue(name)->SetVector(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material3::SetMatrixParameter(const StringRef& name, const Matrix& value)
{
	FindShaderValue(name)->SetMatrix(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material3::SetTextureParameter(const StringRef& name, Texture* value)
{
	FindShaderValue(name)->SetManagedTexture(value);
	m_modifiedForMaterialInstance = true;
}

//------------------------------------------------------------------------------
void Material3::SetColorParameter(const StringRef& name, const Color& value)
{
	SetVectorParameter(name, value);
}

//------------------------------------------------------------------------------
void Material3::SetColorParameter(const StringRef& name, float r, float g, float b, float a)
{
	SetColorParameter(name, Color(r, g, b, a));
}

//------------------------------------------------------------------------------
void Material3::LinkVariables()
{
#if 0
	m_valueList.Clear();
	m_linkedVariableList.Clear();

	if (m_shader != nullptr)
	{
		for (ShaderVariable* v : m_shader->GetVariables())
		{
			if (v->GetType() != ShaderVariableType_Unknown &&
				v->GetType() != ShaderVariableType_String)
			{
				// Unknown と String 型は無視。String 型は読み取り専用で、Material としては持っておく必要ない。
			}
			else
			{
				// 名前と値の対応表
				auto var = std::make_shared<ShaderValue>(v->GetShaderValue());	// 初期値
				m_valueList.Add(v->GetName(), var);

				// 変数と値のペア
				ValuePair pair = { v, var };
				m_linkedVariableList.Add(pair);
			}
		}
	}
#endif
}

//------------------------------------------------------------------------------
ShaderValue* Material3::FindShaderValue(const StringRef& name)
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		v = std::make_shared<ShaderValue>();
		m_valueList.Add(name, v);
	}
	return v.get();
}

//------------------------------------------------------------------------------
ShaderValue* Material3::FindShaderValueConst(const StringRef& name) const
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		return nullptr;
	}
	return v.get();
}


//==============================================================================
// StaticMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshModel, Object);

//------------------------------------------------------------------------------
StaticMeshModel::StaticMeshModel()
	: m_manager(nullptr)
	, m_vertexDeclaration()
	, m_vertexBuffer()
	, m_indexBuffer()
	, m_materials()
	, m_attributes()
{
}

//------------------------------------------------------------------------------
StaticMeshModel::~StaticMeshModel()
{
}

//------------------------------------------------------------------------------
void StaticMeshModel::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;

	m_materials = RefPtr<MaterialList3>::MakeRef();
}

//------------------------------------------------------------------------------
void StaticMeshModel::CreateBox(const Vector3& size)
{
	TexUVBoxMeshFactory factory(size);
	m_vertexDeclaration = m_manager->GetDefaultVertexDeclaration();

	m_vertexBuffer = RefPtr<VertexBuffer>::MakeRef();
	m_indexBuffer = RefPtr<IndexBuffer>::MakeRef();
	m_vertexBuffer->Initialize(m_manager, sizeof(Vertex) * factory.GetVertexCount(), nullptr, DeviceResourceUsage_Static);
	m_indexBuffer->Initialize(m_manager, factory.GetIndexCount(), nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Static);

	ScopedVertexBufferLock lock1(m_vertexBuffer);
	ScopedIndexBufferLock lock2(m_indexBuffer);
	factory.Generate((Vertex*)lock1.GetData(), (uint16_t*)lock2.GetData());

	SetMaterialCount(1);
	m_attributes[0].MaterialIndex = 0;
	m_attributes[0].StartIndex = 0;
	m_attributes[0].PrimitiveNum = factory.GetIndexCount() / 3;
}

//------------------------------------------------------------------------------
void StaticMeshModel::CreateSphere(float radius, int slices, int stacks)
{
	SphereMeshFactory factory(radius, slices, stacks);
	m_vertexDeclaration = m_manager->GetDefaultVertexDeclaration();

	m_vertexBuffer = RefPtr<VertexBuffer>::MakeRef();
	m_indexBuffer = RefPtr<IndexBuffer>::MakeRef();
	m_vertexBuffer->Initialize(m_manager, sizeof(Vertex) * factory.GetVertexCount(), nullptr, DeviceResourceUsage_Static);
	m_indexBuffer->Initialize(m_manager, factory.GetIndexCount(), nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Static);

	ScopedVertexBufferLock lock1(m_vertexBuffer);
	ScopedIndexBufferLock lock2(m_indexBuffer);
	factory.Generate((Vertex*)lock1.GetData(), (uint16_t*)lock2.GetData());

	SetMaterialCount(1);
	m_attributes[0].MaterialIndex = 0;
	m_attributes[0].StartIndex = 0;
	m_attributes[0].PrimitiveNum = factory.GetIndexCount() / 3;
}

//------------------------------------------------------------------------------
void StaticMeshModel::SetMaterialCount(int count)
{
	int oldCount = m_materials->GetCount();
	m_materials->Resize(count);
	m_attributes.Resize(count);
	if (oldCount < count)
	{
		for (int i = oldCount; i < count; ++i)
		{
			auto m = RefPtr<Material3>::MakeRef();
			m_materials->SetAt(i, m);
		}
	}
}

//------------------------------------------------------------------------------
void StaticMeshModel::Draw(GraphicsContext* g)
{

}

LN_NAMESPACE_END

