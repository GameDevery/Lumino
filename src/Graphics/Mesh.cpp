
#include "Internal.h"
#include "Device/ShaderVariableBase.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Mesh.h>

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
		outVertices[1].position.Set(minPos.x, minPos.y, minPos.z);	// 左下
		outVertices[2].position.Set(maxPos.x, maxPos.y, minPos.z);	// 右上
		outVertices[3].position.Set(maxPos.x, minPos.y, minPos.z);	// 右下
		// 奥 (Z+)
		outVertices[4].position.Set(minPos.x, maxPos.y, maxPos.z);	// 左上
		outVertices[5].position.Set(minPos.x, minPos.y, maxPos.z);	// 左下
		outVertices[6].position.Set(maxPos.x, maxPos.y, maxPos.z);	// 右上
		outVertices[7].position.Set(maxPos.x, minPos.y, maxPos.z);	// 右下

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






//==============================================================================
// Material
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Material3, Object);

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
}

//------------------------------------------------------------------------------
void Material3::SetIntParameter(const StringRef& name, int value)
{
	FindShaderValue(name)->SetInt(value);
}

//------------------------------------------------------------------------------
void Material3::SetFloatParameter(const StringRef& name, float value)
{
	FindShaderValue(name)->SetFloat(value);
}

//------------------------------------------------------------------------------
void Material3::SetVectorParameter(const StringRef& name, const Vector4& value)
{
	FindShaderValue(name)->SetVector(value);
}

//------------------------------------------------------------------------------
void Material3::SetMatrixParameter(const StringRef& name, const Matrix& value)
{
	FindShaderValue(name)->SetMatrix(value);
}

//------------------------------------------------------------------------------
void Material3::SetTextureParameter(const StringRef& name, Texture* value)
{
	FindShaderValue(name)->SetDeviceTexture((value) ? value->GetDeviceObject() : nullptr);
}

//------------------------------------------------------------------------------
void Material3::LinkVariables()
{
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
	, m_triangleCount(0)
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
}

//------------------------------------------------------------------------------
void StaticMeshModel::CreateBox(const Vector3& size)
{
	BoxMeshFactory factory(size);
	m_vertexDeclaration = RefPtr<VertexDeclaration>::MakeRef();
	m_vertexDeclaration->Initialize(m_manager);
	m_vertexBuffer = RefPtr<VertexBuffer>::MakeRef();
	m_vertexBuffer->Initialize(m_manager, sizeof(Vertex) * factory.GetVertexCount(), nullptr, DeviceResourceUsage_Static);
	m_indexBuffer = RefPtr<IndexBuffer>::MakeRef();
	m_indexBuffer->Initialize(m_manager, factory.GetIndexCount(), nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Static);

	m_vertexDeclaration->AddVertexElement(0, VertexElementType_Float3, VertexElementUsage_Position, 0);
	m_vertexDeclaration->AddVertexElement(0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0);
	m_vertexDeclaration->AddVertexElement(0, VertexElementType_Float3, VertexElementUsage_Normal, 0);
	m_vertexDeclaration->AddVertexElement(0, VertexElementType_Float4, VertexElementUsage_Color, 0);

	ScopedVertexBufferLock lock1(m_vertexBuffer);
	ScopedIndexBufferLock lock2(m_indexBuffer);
	factory.Generate((Vertex*)lock1.GetData(), (uint16_t*)lock2.GetData());

	m_triangleCount = factory.GetIndexCount() / 3;
}

//------------------------------------------------------------------------------
void StaticMeshModel::Draw(GraphicsContext* g)
{

}

LN_NAMESPACE_END

