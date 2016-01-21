
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Shader.h>
#include "Device/GraphicsDriverInterface.h"
#include "RendererImpl.h"
#include "RenderingCommand.h"
#include "PrimitiveRenderer.h"

#define LN_CALL_CORE_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		m_manager->GetPrimaryRenderingCommandList()->AddCommand<command>(m_core, __VA_ARGS__); \
	} \
	else { \
		m_core->func(__VA_ARGS__); \
	}

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//=============================================================================
struct PrimitiveRendererCore_SetStateCommand : public RenderingCommand
{
	PrimitiveRendererCore*	m_core;
	Matrix					m_transform;
	Matrix					m_viewProj;
	Size					m_viewPixelSize;
	bool					m_useInternalShader;
	PrimitiveRendererMode	m_mode;
	Driver::IShader*		m_userShader;
	void Create(PrimitiveRendererCore* core, const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize, bool useInternalShader, PrimitiveRendererMode mode, Driver::IShader* userShader)
	{
		m_core = core;
		m_transform = world;
		m_viewProj = viewProj;
		m_viewPixelSize = viewPixelSize;
		m_useInternalShader = useInternalShader;
		m_mode = mode;
		m_userShader = userShader;
		MarkGC(m_userShader);
	}
	void Execute()
	{
		m_core->SetState(m_transform, m_viewProj, m_viewPixelSize, m_useInternalShader, m_mode, m_userShader);
	}
};

//=============================================================================
struct PrimitiveRendererCore_DrawLine : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	Vector3 m_from;
	ColorF m_fromColor;
	Vector3 m_to;
	ColorF m_toColor;

	void Create(PrimitiveRendererCore* core, const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor)
	{
		m_core = core;
		m_from = from;
		m_fromColor = fromColor;
		m_to = to;
		m_toColor = toColor;
	}
	void Execute() { m_core->DrawLine(m_from, m_fromColor, m_to, m_toColor); }
};

//=============================================================================
struct PrimitiveRendererCore_DrawSquare : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	PrimitiveRendererCore::DrawSquareData	m_data;

	void Create(PrimitiveRendererCore* core, const PrimitiveRendererCore::DrawSquareData& data) { m_data = data; }
	void Execute() { m_core->DrawSquare(m_data); }
};

//=============================================================================
struct PrimitiveRendererCore_FlushCommand : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	void Create(PrimitiveRendererCore* core) { m_core = core; }
	void Execute() { m_core->Flush(); }
};

//=============================================================================
// PrimitiveRendererCore
//=============================================================================
	
static const byte_t g_PrimitiveRenderer_fx_Data[] =
{
#include "Resource/PrimitiveRenderer.fx.h"
};
static const size_t g_PrimitiveRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_PrimitiveRenderer_fx_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRendererCore::PrimitiveRendererCore()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_vertexCacheUsed(0)
	, m_foreTexture(nullptr)
	, m_mode(PrimitiveRendererMode::TriangleList)
	, m_userShader(nullptr)
	, m_useInternalShader(true)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRendererCore::~PrimitiveRendererCore()
{
	LN_SAFE_RELEASE(m_userShader);
	LN_SAFE_RELEASE(m_shader.shader);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRendererCore::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	const int DefaultFaceCount = 2048;

	auto* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();
	m_vertexBuffer = device->CreateVertexBuffer(Vertex::Elements(), Vertex::ElementCount, DefaultFaceCount * 4, nullptr, DeviceResourceUsage_Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

	m_vertexCache.Resize(DefaultFaceCount * 4);
	m_vertexCacheUsed = 0;
	m_indexCache.Reserve(DefaultFaceCount * 6);

	ShaderCompileResult r;
	m_shader.shader = device->CreateShader(g_PrimitiveRenderer_fx_Data, g_PrimitiveRenderer_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);
	m_shader.technique = m_shader.shader->GetTechnique(0);
	m_shader.pass = m_shader.technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.shader->GetVariableByName(_T("g_texture"));
	m_shader.varPixelStep = m_shader.shader->GetVariableByName(_T("g_pixelStep"));

	m_vertexStride = sizeof(Vertex);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRendererCore::SetState(const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize, bool useInternalShader, PrimitiveRendererMode mode, Driver::IShader* userShader)
{
	m_shader.varWorldMatrix->SetMatrix(world);
	m_shader.varViewProjMatrix->SetMatrix(viewProj);
	m_shader.varPixelStep->SetVector(Vector4(0.5f / viewPixelSize.Width, 0.5f / viewPixelSize.Height, 0, 0));
	m_useInternalShader = useInternalShader;
	m_mode = mode;
	m_userShader = userShader;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRendererCore::DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor)
{
	AddVertex(from, Vector2::Zero, fromColor);
	AddVertex(to, Vector2::Zero, toColor);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRendererCore::DrawSquare(const DrawSquareData& data)
{
	uint16_t i = GetCurrentVertexCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);
	m_indexCache.Add(i + 3);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);

	AddVertex(data.pos[0], data.uv[0], data.color[0]);
	AddVertex(data.pos[1], data.uv[1], data.color[1]);
	AddVertex(data.pos[2], data.uv[2], data.color[2]);
	AddVertex(data.pos[3], data.uv[3], data.color[3]);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRendererCore::Flush()
{
	// �`�悷��
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetConstData(), m_vertexCacheUsed);
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	m_renderer->SetVertexBuffer(m_vertexBuffer);
	m_renderer->SetIndexBuffer(m_indexBuffer);

	// ���[�U�[�V�F�[�_���g���ꍇ
	if (m_userShader != nullptr)
	{
		int techCount = m_userShader->GetTechniqueCount();
		for (int iTech = 0; iTech < techCount; ++iTech)
		{
			Driver::IShaderTechnique* tech = m_userShader->GetTechnique(iTech);
			int passCount = tech->GetPassCount();
			for (int iPass = 0; iPass < passCount; ++iPass)
			{
				tech->GetPass(iPass)->Apply();

				if (m_mode == PrimitiveRendererMode::TriangleList)
				{
					m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
				}
				else if (m_mode == PrimitiveRendererMode::LineList)
				{
					m_renderer->DrawPrimitive(PrimitiveType_LineList, 0, GetCurrentVertexCount() / 2);
				}
			}
		}
	}
	// ���[�U�[�V�F�[�_���g��Ȃ��ꍇ
	else
	{
		if (m_useInternalShader)
		{
			Driver::ITexture* srcTexture = m_foreTexture;
			if (srcTexture == nullptr) {
				srcTexture = m_manager->GetDummyTexture();
			}
			m_shader.varTexture->SetTexture(srcTexture);
			m_shader.pass->Apply();
		}

		if (m_mode == PrimitiveRendererMode::TriangleList)
		{
			m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
		}
		else if (m_mode == PrimitiveRendererMode::LineList)
		{
			m_renderer->DrawPrimitive(PrimitiveType_LineList, 0, GetCurrentVertexCount() / 2);
		}
	}

	// �L���b�V���N���A
	m_vertexCacheUsed = 0;
	m_indexCache.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRendererCore::AddVertex(const Vector3& pos, const Vector2& uv, const ColorF& color)
{
	uint32_t size = sizeof(Vertex);
	if (m_vertexCacheUsed + size >= m_vertexCache.GetSize()) {
		m_vertexCache.Resize(m_vertexCache.GetSize() * 2);
	}

	byte_t* data = m_vertexCache.GetData();

	Vertex* v = (Vertex*)&data[m_vertexCacheUsed];
	v->position = pos;
	v->uv = uv;
	v->color = color;

	m_vertexCacheUsed += size;
}

//=============================================================================
// PrimitiveRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRenderer::PrimitiveRenderer()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_userShader(nullptr)
	, m_useInternalShader(true)
	, m_stateModified(false)
	, m_flushRequested(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRenderer::~PrimitiveRenderer()
{
	LN_SAFE_RELEASE(m_userShader);
	LN_SAFE_RELEASE(m_core);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;

	m_core = LN_NEW PrimitiveRendererCore();
	m_core->Initialize(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::SetTransform(const Matrix& matrix)
{
	m_transform = matrix;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::SetViewProjMatrix(const Matrix& matrix)
{
	m_viewProj = matrix;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::SetViewPixelSize(const Size& size)
{
	m_viewPixelSize = size;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::SetUseInternalShader(bool useInternalShader)
{
	m_useInternalShader = useInternalShader;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void PrimitiveRenderer::SetTexture(Texture* texture)
//{
//	LN_REFOBJ_SET(m_texture, texture);
//	m_stateModified = true;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor)
{
	SetPrimitiveRendererMode(PrimitiveRendererMode::LineList);
	CheckUpdateState();
	LN_CALL_CORE_COMMAND(DrawLine, PrimitiveRendererCore_DrawLine, from, fromColor, to, toColor);
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::DrawSquare(
	const Vector3& position1, const Vector2& uv1, const ColorF& color1,
	const Vector3& position2, const Vector2& uv2, const ColorF& color2,
	const Vector3& position3, const Vector2& uv3, const ColorF& color3,
	const Vector3& position4, const Vector2& uv4, const ColorF& color4)
{
	SetPrimitiveRendererMode(PrimitiveRendererMode::TriangleList);
	CheckUpdateState();
	PrimitiveRendererCore::DrawSquareData data;
	data.pos[0] = position1; data.uv[0] = uv1; data.color[0] = color1;
	data.pos[1] = position2; data.uv[1] = uv2; data.color[1] = color2;
	data.pos[2] = position3; data.uv[2] = uv3; data.color[2] = color3;
	data.pos[3] = position4; data.uv[3] = uv4; data.color[3] = color4;
	LN_CALL_CORE_COMMAND(DrawSquare, PrimitiveRendererCore_DrawSquare, data);
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::Flush()
{
	if (m_flushRequested)
	{
		LN_CALL_CORE_COMMAND(Flush, PrimitiveRendererCore_FlushCommand);
		m_flushRequested = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::SetPrimitiveRendererMode(PrimitiveRendererMode mode)
{
	if (mode != m_mode)
	{
		m_mode = mode;
		m_stateModified = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::CheckUpdateState()
{
	if (m_stateModified)
	{
		Flush();
		LN_CALL_CORE_COMMAND(SetState, PrimitiveRendererCore_SetStateCommand, m_transform, m_viewProj, m_viewPixelSize, m_useInternalShader, m_mode, (m_userShader != nullptr) ? m_userShader->m_deviceObj : nullptr);
		m_stateModified = false;
	}
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
