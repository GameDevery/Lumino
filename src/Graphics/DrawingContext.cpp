/*
	[2015/12/2] �^�C�����O�`��ɂ���
		�f�t�H���g�ł͗L���ɂ��Ȃ��B�V�F�[�_�͕�����B
		�^�C���z�u����u���V���w�肳�ꂽ�ꍇ�A�`�惂�[�h��؂�ւ���B
		��ɗL���ɂ���ɂ͂��܂�ɂ����G������B


	[2015/12/2] �R�}���h�̃L���b�V���ɂ���
		����X�e�[�g�Ԃ� Primitive �`��R�}���h�͈ꊇ�Ŏ��s�ł���B
		�܂Ƃ߂ăR�}���h���X�g�ɑ���Ƃ����̂����A�o�b�t�@�͒N�����́H
		�� �X�e�[�g�̐؂�ւ��͊���ƕp�ɂ��Ǝv����B
		�ނ��� new �̐��������Ă��܂���������Ȃ��B
		memcpy �ŃR�}���h���X�g�̈ꎞ�o�b�t�@�փR�s�[����ق����S�̓I�Ɍ��ăp�t�H�[�}���X�ǂ������B
		sizeof(Matrix) ��1�R�}���h���̕��σT�C�Y�Ƃ��� 10000 �q memcpy ����Ƃ��� 30us ���炢������B
		1000 �Ȃ� 3us�B
		���ۂɂ͂���ȂɎg��Ȃ����낤���Amemcpy �Ŗ��Ȃ��͂��B


	[2015/12/2] �N���X��
		���΂炭�ԋ󂢂āA����ς� DrawingContext �̂ق��������Ǝv���B
		���̃��C�u�����͂��Ȃ�჌�x����API�����J����B
		�����Ȃ��Ă���� DirectX11 �� RenderingContext �̈Ӗ��������Ȃ��Ă���B
		�����x�����Ă��Ƃō��ʉ�����Ȃ� DrawingContext�B
		


	���O�̌��l�^�͂��̂����肩��B
	https://developer.mozilla.org/ja/docs/Web/API/RenderingContext

	���̃��W���[���� Scene �� GUI �ŋ��L����邪�A
	DrawingContext �� Scene ���� GUI ���B
	���C�u�����Ƃ��Ă� Scene �̕����Ӗ����傫���BDraw ��� Render ���ȁA�ƁB


	Scene �ł́A���炩���� SceneNode �̃V�F�[�_�����[�U�[�V�F�[�_�Ƃ���
	�ݒ肵�����̂� OnRender() �ɓn���B



	���[�U�[�V�F�[�_�ɗv�����钸�_�錾�́A
	�EPos0
	�ETexUV0
	�EColor0

	���[�U�[�V�F�[�_�ɗv������p�����[�^�́A
	�EVIEWPORTPIXELSIZE
	�ELNRC_TONE
		UI_TONE�ANODE_TONE �Ƃ��̂ق������������H
	��Tone �̌v�Z�̓��[�U�[�V�F�[�_�ŐF�����܂�����ɍs��Ȃ���΂Ȃ�Ȃ��B
	�i��Z�����ŕ\���ł��Ȃ��̂Œ��_�錾�����ł͕s���j
	#include "lumino.fx" �Ƃ����āA�s�N�Z���V�F�[�_�� LNUI_PostPixelShader �݂����Ȋ֐���
	���[�U�[�ɌĂ�ł��炤�K�v������B

	�`�������3�B
	�E�v���~�e�B�u�̂�
	�E�g�ݍ��݃V�F�[�_���g��
	�E���[�U�[�V�F�[�_���g��

	RenderingContext
	BasicRenderingContext
	UIRenderingContext
	SceneNodeRenderingContext
*/

#include "Internal.h"
#include <algorithm>
#include "PainterEngine.h"	// CacheBuffer
#include "RenderingCommand.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/DrawingContext.h>

LN_NAMESPACE_BEGIN


//class DrawingPrimitiveCommandList
//{
//public:
//
//protected:
//	DrawingContext();
//	virtual ~DrawingContext();
//	void Initialize(GraphicsManager* manager);
//
//private:
//	GraphicsManager*	m_manager;
//};


//=============================================================================
// DrawingCommands
//=============================================================================
enum class DrawingCommandType : uint32_t
{
	DrawLine = 0,
};
struct DrawingCommands_DrawLine
{
	DrawingCommandType	type;
	Vector3				from;
	Vector3				to;
	ColorF				fromColor;
	ColorF				toColor;
};


//=============================================================================
// PrimitiveRenderer
//=============================================================================
class PrimitiveRenderer
{
public:
	PrimitiveRenderer();
	virtual ~PrimitiveRenderer();

	void Clear();
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);

private:
	struct PainterVertex
	{
	public:

		Vector3	Position;			///< �ʒu
		Vector4	Color;				///< ���_�J���[
		Vector4	UVOffset;		///< �e�N�X�`��UV (�]������UV���)
		Vector2	UVTileUnit;		///< �e�N�X�`��UV (�^�C�����O��Ԃ̂ǂ��ɂ��邩)

		/// ���_���C�A�E�g
		static VertexElement* Elements()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 1 },
			};
			return elements;
		}
		static const int ElementCount = 4;
	};

	CacheBuffer<PainterVertex>		m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRenderer::PrimitiveRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRenderer::~PrimitiveRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::Clear()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	//PainterVertex v;
	//v.Position = from;
	//v.Color = fromColor;
	//m_vertexCache.Add(v);
	//v.Position = to;
	//v.Color = toColor;
	//m_vertexCache.Add(v);
}




//=============================================================================
// DrawingContextImpl
//=============================================================================
class DrawingContextImpl
	: public RefObject
{
public:
	DrawingContextImpl(GraphicsManager* manager);
	~DrawingContextImpl();

	void DoCommandList(const void* commandBuffer, size_t size);
	void Flush();

public:
	struct DoCommandListCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		DataHandle m_commandBuffer;
		size_t	m_size;
		void Create(DrawingContextImpl* engine, const void* commandBuffer, size_t size)
		{
			m_impl = engine;
			m_commandBuffer = AllocExtData(size, commandBuffer);
			m_size = size;
		}
		void Execute() { m_impl->DoCommandList(GetExtData(m_commandBuffer), m_size); }
	};

	struct FlushCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		void Create(DrawingContextImpl* engine) { m_impl = engine; }
		void Execute() { m_impl->Flush(); }
	};

private:
	GraphicsManager* m_manager;
	PrimitiveRenderer	m_primitiveRenderer;

	// DrawingContext3D
	struct
	{
		RefPtr<Driver::IShader>		shader;
		Driver::IShaderVariable*    varWorldMatrix;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varTexture;
		Driver::IShaderTechnique*   techMainDraw;
		Driver::IShaderPass*        passP0;

	} m_shader3D;
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::DrawingContextImpl(GraphicsManager* manager)
	: m_manager(manager)
{
	Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();

	//-----------------------------------------------------
	// �V�F�[�_ (DrawingContext3D)
	const unsigned char code[] =
	{
#include "Resource/DrawingContext3D.fx.h"
	};
	const size_t codeLen = LN_ARRAY_SIZE_OF(code);

	ShaderCompileResult result;
	m_shader3D.shader.Attach(device->CreateShader(code, codeLen, &result), false);
	if (result.Level != ShaderCompileResultLevel_Success) {
		printf(result.Message.c_str());	// TODO:��
	}

	m_shader3D.varWorldMatrix = m_shader3D.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader3D.varViewProjMatrix = m_shader3D.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader3D.varTexture = m_shader3D.shader->GetVariableByName(_T("g_texture"));
	m_shader3D.techMainDraw = m_shader3D.shader->GetTechnique(0);
	m_shader3D.passP0 = m_shader3D.techMainDraw->GetPass(0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::~DrawingContextImpl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::DoCommandList(const void* commandBuffer, size_t size)
{
	const byte_t* pos = (const byte_t*)commandBuffer;
	const byte_t* end = pos + size;
	detail::DrawingClass c = *((const detail::DrawingClass*)pos);
	pos += sizeof(detail::DrawingClass);

	while (pos < end)
	{
		DrawingCommandType type = *((const DrawingCommandType*)pos);
		switch (type)
		{
			case DrawingCommandType::DrawLine:
			{
				const DrawingCommands_DrawLine* cmd = (const DrawingCommands_DrawLine*)pos;
				m_primitiveRenderer.DrawLine(cmd->from, cmd->to, cmd->fromColor, cmd->toColor);
				pos += sizeof(DrawingCommands_DrawLine);
				break;
			}
			default:
				LN_THROW(0, InvalidOperationException);
				return;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::Flush()
{

}


//=============================================================================
// DrawingContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::DrawingContext()
	: m_manager(nullptr)
	, m_commandsUsingByte(0)
	, m_internal(nullptr)
	, m_currentDrawingClass(detail::DrawingClass::Unknown)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_internal);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_internal = LN_NEW	DrawingContextImpl();
	m_commandsBuffer.Resize(1024);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	DrawingCommands_DrawLine cmd;
	cmd.type = DrawingCommandType::DrawLine;
	cmd.from = from;
	cmd.to = to;
	cmd.fromColor = fromColor;
	cmd.toColor = toColor;
	AddCommand(&cmd, sizeof(cmd));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& color)
{
	CheckFlush(m_currentDrawingClass);
	DrawLine(from, to, color, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::Flush()
{
	FlushInternal();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::AddCommand(const void* command, size_t size)
{
	// �o�b�t�@������Ȃ���Ίg������
	if (m_commandsUsingByte + size > m_commandsBuffer.GetSize()) {
		size_t newSize = m_commandsBuffer.GetSize() + std::max(m_commandsBuffer.GetSize(), size);	// �Œ�ł� size �����g������
		m_commandsBuffer.Resize(newSize, false);
	}

	// �f�[�^�R�s�[
	if (command != NULL)
	{
		byte_t* ptr = &(m_commandsBuffer.GetData()[m_commandsUsingByte]);
		size_t bufSize = m_commandsBuffer.GetSize() - m_commandsUsingByte;
		memcpy_s(ptr, bufSize, command, size);
	}

	// �o�b�t�@�g�p�ʂ𑝂₷
	m_commandsUsingByte += size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::FlushInternal()
{
	LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::CheckFlush(detail::DrawingClass newDrawingClass)
{
	if (m_currentDrawingClass != newDrawingClass &&
		m_currentDrawingClass != detail::DrawingClass::Unknown) {
		FlushInternal();
	}
	m_currentDrawingClass = newDrawingClass;

	// �R�}���h�o�b�t�@�̐擪�ɋ敪�����Ă���
	AddCommand(&m_currentDrawingClass, sizeof(uint32_t));
}

LN_NAMESPACE_END
