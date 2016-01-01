/*
	[2015/12/3]
		Arc
		Polygon
		PolyLine
		Ellipse
		Pie
		Point
		Rect
		RoundRect
		Text
		Image
		Path

		�EPath �ȊO�͔ėp�I�Ȗʒ�����s���������O�Œ��_����Ă��܂����ق��������B
		�E�X�g���[�N�ƃA���`�G�C���A�X�͖ʒ��肵�Ȃ���΂Ȃ�Ȃ��̂ŁA�O���̒��_�͑f�����Q�Ƃł���悤�ɂ���K�v������B

		�A�E�g���C���̃|�C���g���X�g�쐬
		(Path�ȊO)���_���X�g�쐬
		(Path)�ʒ���E���_���X�g�쐬
		Fill���_�o�b�t�@�쐬
		�`��
		Stroke���_�o�b�t�@�쐬
		�`��

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
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Graphics/GeometryRenderer.h>


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

struct DrawingBasicVertex
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

//=============================================================================
// DrawingCommands
//=============================================================================
enum class DrawingCommandType : uint32_t
{
	DrawLine = 0,
	DrawRectangle,
};
struct DrawingCommands_DrawLine
{
	DrawingCommandType	type;
	Vector3				from;
	Vector3				to;
	ColorF				fromColor;
	ColorF				toColor;
};
struct DrawingCommands_DrawRectangle
{
	DrawingCommandType	type;
	RectF				rect;
	ColorF				color;
};


//=============================================================================
// PrimitiveCache
//=============================================================================
class PrimitiveCache
{
public:
	PrimitiveCache();
	virtual ~PrimitiveCache();

	void Clear();
	void DrawSimpleLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);
	void DrawRectangle(const RectF& rect, const RectF& srcUVRect, const ColorF& color);

	void ApplyBuffers(Driver::IVertexBuffer* vb, Driver::IIndexBuffer* ib);
	int GetIndexCount() const { return m_indexCache.GetCount(); }
private:

	CacheBuffer<DrawingBasicVertex>	m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveCache::PrimitiveCache()
{
	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveCache::~PrimitiveCache()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::Clear()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::DrawSimpleLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	DrawingBasicVertex v;
	v.Position = from;
	v.Color = fromColor;
	m_vertexCache.Add(v);
	v.Position = to;
	v.Color = toColor;
	m_vertexCache.Add(v);

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::DrawRectangle(const RectF& rect, const RectF& srcUVRect, const ColorF& color)
{
	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.GetTop();
	float uvWidth = srcUVRect.Width;
	float uvHeight = srcUVRect.Height;

	DrawingBasicVertex v;
	v.Color = color;
	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 1);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 1);	// �E��
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 2);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 2);	// �E��
	m_vertexCache.Add(v);

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveCache::ApplyBuffers(Driver::IVertexBuffer* vb, Driver::IIndexBuffer* ib)
{
	vb->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	ib->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
}



//=============================================================================
// DrawingContextImpl
//=============================================================================
class DrawingContextImpl
	: public RefObject
{
public:

	struct BrushData
	{
		BrushType	Type;

		union
		{
			struct
			{
				float				Color[4];	///< RGBA

			} SolidColorBrush;

			struct
			{
				Driver::ITexture*	Texture;
				int					SourceRect[4];	///< XYWH
				BrushWrapMode		WrapMode;

			} TextureBrush;

			struct
			{
				Driver::ITexture*	Texture;
				int					SourceRect[4];	///< XYWH
				int					InnerSourceRect[4];	///< XYWH
				BrushWrapMode		WrapMode;
				int					FrameThicness;

			} FrameTextureBrush;
		};

		void Set(ln::Brush* brush)
		{
			if (brush == nullptr)
			{
				Type = BrushType_Unknown;	// no set
			}
			else
			{
				Type = brush->GetType();
				if (Type == BrushType_SolidColor)
				{
					auto t = static_cast<ColorBrush*>(brush);
					const ColorF& c = t->GetColor();
					SolidColorBrush.Color[0] = c.R;		// TODO: POD �^���܂Ƃ߂Ē�`�����ق��������C������
					SolidColorBrush.Color[1] = c.G;
					SolidColorBrush.Color[2] = c.B;
					SolidColorBrush.Color[3] = c.A;
				}
				else if (Type == BrushType_Texture)
				{
					auto t = static_cast<ln::TextureBrush*>(brush);
					TextureBrush.Texture = (t->GetTexture() != nullptr) ? t->GetTexture()->GetDeviceObject() : nullptr;
					const Rect& r = t->GetSourceRect();
					TextureBrush.SourceRect[0] = r.X;		// TODO: POD �^���܂Ƃ߂Ē�`�����ق��������C������
					TextureBrush.SourceRect[1] = r.Y;
					TextureBrush.SourceRect[2] = r.Width;
					TextureBrush.SourceRect[3] = r.Height;
					TextureBrush.WrapMode = t->GetWrapMode();
				}
				else if (Type == BrushType_FrameTexture)
				{
					auto t = static_cast<ln::FrameTextureBrush*>(brush);
					FrameTextureBrush.Texture = (t->GetTexture() != nullptr) ? t->GetTexture()->GetDeviceObject() : nullptr;
					const Rect& r = t->GetSourceRect();
					const Rect& r2 = t->GetInnerAreaSourceRect();
					FrameTextureBrush.SourceRect[0] = r.X;		// TODO: POD �^���܂Ƃ߂Ē�`�����ق��������C������
					FrameTextureBrush.SourceRect[1] = r.Y;
					FrameTextureBrush.SourceRect[2] = r.Width;
					FrameTextureBrush.SourceRect[3] = r.Height;
					FrameTextureBrush.InnerSourceRect[0] = r2.X;		// TODO: POD �^���܂Ƃ߂Ē�`�����ق��������C������
					FrameTextureBrush.InnerSourceRect[1] = r2.Y;
					FrameTextureBrush.InnerSourceRect[2] = r2.Width;
					FrameTextureBrush.InnerSourceRect[3] = r2.Height;
					FrameTextureBrush.WrapMode = t->GetWrapMode();
					FrameTextureBrush.FrameThicness = t->GetThickness();
				}
				else {
					LN_THROW(0, NotImplementedException);
				}
			}
		}

		void Copy(const BrushData& data)
		{
			memcpy(this, &data, sizeof(BrushData));
			if (Type == BrushType_SolidColor) {
				//ForeColor.R = Brush.SolidColorBrush.Color[0];
				//ForeColor.G = Brush.SolidColorBrush.Color[1];
				//ForeColor.B = Brush.SolidColorBrush.Color[2];
				//ForeColor.A = Brush.SolidColorBrush.Color[3];
			}
			else if (Type == BrushType_Texture) {
				LN_SAFE_ADDREF(TextureBrush.Texture);
			}
			else if (Type == BrushType_FrameTexture) {
				LN_SAFE_ADDREF(FrameTextureBrush.Texture);
			}
		}
	};

	struct PenData
	{
		BrushData	brush;
		float		thickness;

		void Set(ln::Pen* pen)
		{
			if (pen != nullptr)
			{
				brush.Set(pen->GetBrush());
				thickness = pen->GetThickness();
			}
			else
			{
				brush.Set(nullptr);
				thickness = 0.0f;
			}
		}
		void Copy(const PenData& data)
		{
			brush.Copy(data.brush);
			thickness = data.thickness;
		}
	};

	// ���̃N���X�͕`��R�}���h�̈����ƂȂ�N���X�BRefPtr �Ƃ��̓����o�ɒu���Ȃ����ƁB
	struct DrawingState
	{
		BrushData	Brush;
		PenData		pen;
		float		Opacity;
		//ColorF		ForeColor;		///< ��Z����F�BSolidColorBrush �̎��͂��̐F�ɂȂ�B����� Opacity �̏�Z���ʁB
		ToneF		Tone;

		DrawingState()
		{
			pen.brush.Type = BrushType_Unknown;
		}

		// Painter ���ō��APainterEngineState ���R�}���h���X�g�ɏ悹��Ƃ��Ɏg���B
		void Create(const detail::DrawingState& state)
		{
			// �u���V�f�[�^
			Brush.Set(state.brush);
			pen.Set(state.pen);

			// �s�����x
			Opacity = state.opacity;

			// �F��
			Tone = state.tone;
		}

		void Copy(const DrawingState& state)
		{
			ReleaseObjects();

			// �u���V�f�[�^
			Brush.Copy(state.Brush);
			pen.Copy(state.pen);

			// �s�����x
			Opacity = state.Opacity;

			// �F��
			Tone = state.Tone;
		}

		void ReleaseObjects()
		{
			if (Brush.Type == BrushType_Texture) {
				LN_SAFE_RELEASE(Brush.TextureBrush.Texture);
			}
			else if (Brush.Type == BrushType_FrameTexture) {
				LN_SAFE_RELEASE(Brush.FrameTextureBrush.Texture);
			}
		}
	};

public:
	DrawingContextImpl(GraphicsManager* manager);
	~DrawingContextImpl();

	void SetViewProjection(const Matrix& view, const Matrix& proj);
	void SetState(const DrawingState& state);
	void DoCommandList(const void* commandBuffer, size_t size);
	void Flush();


public:
	struct SetViewProjectionCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		Matrix m_view;
		Matrix m_proj;

		void Create(DrawingContextImpl* impl, const Matrix& view, const Matrix& proj)
		{
			m_impl = impl;
			m_view = view;
			m_proj = proj;
		}
		void Execute() { m_impl->SetViewProjection(m_view, m_proj); }
	};

	struct SetStateCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		DrawingState m_state;

		void Create(DrawingContextImpl* impl, const DrawingState& state)
		{
			m_impl = impl;
			m_state.Copy(state);

			if (m_state.Brush.Type == BrushType_Texture) {
				MarkGC(m_state.Brush.TextureBrush.Texture);
			}
			if (m_state.Brush.Type == BrushType_FrameTexture) {
				MarkGC(m_state.Brush.FrameTextureBrush.Texture);
			}
		}
		void Execute() { m_impl->SetState(m_state); }
	};

	struct DoCommandListCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		DataHandle m_commandBuffer;
		size_t	m_size;
		void Create(DrawingContextImpl* impl, const void* commandBuffer, size_t size)
		{
			m_impl = impl;
			m_commandBuffer = AllocExtData(size, commandBuffer);
			m_size = size;
		}
		void Execute() { m_impl->DoCommandList(GetExtData(m_commandBuffer), m_size); }
	};

	struct FlushCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		void Create(DrawingContextImpl* impl) { m_impl = impl; }
		void Execute() { m_impl->Flush(); }
	};

private:

	enum class PathPointAttr
	{
		MoveTo,
		LineTo,
	};

	enum class PathType
	{
		Line,
		Rectangle,
		Path,	// ExpandFill �Ȃǂ����s���Ėʒ�����s��
	};

	struct PathPoint
	{
		PathPointAttr	attr;
		Vector3			point;
		ColorF			color;
	};

	struct StrokePoint
	{
		Vector3			point;
		ColorF			color;
	};

	struct Path
	{
		PathType		type;
		int				firstIndex;		// ���̃p�X�̊J�n�_������ m_pathPoints ��̃C���f�b�N�X
		int				pointCount;		// ���̃p�X���\������ m_pathPoints ��̓_�̐�
		bool			closed;			// �p�X�����Ă��邩 (false �͐���)
	};

	void AddPath(PathType type);
	Path* GetCurrentPath();
	void AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color);
	void ClosePath();

	void PutMoveTo(const Vector3& pt, const ColorF& color);
	void PutLineTo(const Vector3& pt, const ColorF& color);

	void ExpandStroke();
	void AddVertex(const Vector3& point, const ColorF& color);

	GraphicsManager*		m_manager;
	//PrimitiveCache			m_primitiveCache;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	Matrix					m_view;
	Matrix					m_proj;
	DrawingState			m_currentState;

	CacheBuffer<PathPoint>	m_pathPoints;
	CacheBuffer<Path>		m_pathes;

	CacheBuffer<StrokePoint>		m_strokePoints;

	CacheBuffer<DrawingBasicVertex>	m_vertexCache;	// ���_�o�b�t�@�{�̂����T�C�Y���邱�ƂɂȂ�Ɨ]�v�Ɏ��Ԃ�������̂ŁA�܂��͂����ɍ���Ă���R�s�[����
	CacheBuffer<uint16_t>			m_indexCache;

	// DrawingContext3D
	struct
	{
		Driver::IShader*			shader;
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
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
	m_pathPoints.Reserve(1024);
	m_pathes.Reserve(1024);
	m_strokePoints.Reserve(1024);
	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);

	Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();

	const int DefaultFaceCount = 1024;

	m_vertexBuffer = device->CreateVertexBuffer(DrawingBasicVertex::Elements(), DrawingBasicVertex::ElementCount, DefaultFaceCount * 4, NULL, DeviceResourceUsage_Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

	//-----------------------------------------------------
	// �V�F�[�_ (DrawingContext3D)
	const unsigned char code[] =
	{
#include "Resource/DrawingContext3D.fx.h"
	};
	const size_t codeLen = LN_ARRAY_SIZE_OF(code);

	ShaderCompileResult result;
	m_shader3D.shader = device->CreateShader(code, codeLen, &result);
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
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
	LN_SAFE_RELEASE(m_shader3D.shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	m_view = view;
	m_proj = proj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::SetState(const DrawingState& state)
{
	m_currentState.Copy(state);
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

				// �p�X����� (Stroke ����邽�߂Ɏg��)
				AddPath(PathType::Line);

				StrokePoint pt;
				pt.point = cmd->from; pt.color = cmd->fromColor;
				m_strokePoints.Add(pt);
				pt.point = cmd->to; pt.color = cmd->toColor;
				m_strokePoints.Add(pt);

				GetCurrentPath()->pointCount = 2;
				ClosePath();

				//PutMoveTo(cmd->from, cmd->fromColor);
				//PutLineTo(cmd->to, cmd->toColor);
				//ClosePath();

				//// ���_�o�b�t�@
				//DrawingBasicVertex v;
				//v.Position = cmd->from; v.Color = cmd->fromColor;
				//m_vertexCache.Add(v);
				//v.Position = cmd->to; v.Color = cmd->toColor;
				//m_vertexCache.Add(v);
				//
				//// �C���f�b�N�X�o�b�t�@
				//uint16_t i = m_vertexCache.GetCount();
				//m_indexCache.Add(i + 0);
				//m_indexCache.Add(i + 1);

				pos += sizeof(DrawingCommands_DrawLine);
				break;
			}
			case DrawingCommandType::DrawRectangle:
			{
				const DrawingCommands_DrawRectangle* cmd = (const DrawingCommands_DrawRectangle*)pos;

				AddPath(PathType::Rectangle);

				const RectF& rect = cmd->rect;
				StrokePoint pt;
				pt.color = cmd->color;
				pt.point.Set(rect.GetLeft(), rect.GetTop(), 0); m_strokePoints.Add(pt);
				pt.point.Set(rect.GetRight(), rect.GetTop(), 0); m_strokePoints.Add(pt);
				pt.point.Set(rect.GetLeft(), rect.GetBottom(), 0); m_strokePoints.Add(pt);
				pt.point.Set(rect.GetRight(), rect.GetBottom(), 0); m_strokePoints.Add(pt);

				GetCurrentPath()->pointCount = 4;
				ClosePath();

				pos += sizeof(DrawingCommands_DrawRectangle);
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
	if (m_pathes.GetCount() == 0) { return; }


	Driver::IRenderer* renderer = m_manager->GetGraphicsDevice()->GetRenderer();
	m_shader3D.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader3D.varViewProjMatrix->SetMatrix(m_view * m_proj);

	
	if (m_currentState.pen.brush.Type != BrushType_Unknown && m_currentState.pen.thickness == 0.0f)
	{
		ExpandStroke();

		//m_primitiveCache.ApplyBuffers(m_vertexBuffer, m_indexBuffer);
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

		renderer->SetVertexBuffer(m_vertexBuffer);
		renderer->SetIndexBuffer(m_indexBuffer);
		m_shader3D.varTexture->SetTexture(m_manager->GetDummyTexture());
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitiveIndexed(PrimitiveType_LineList, 0, m_indexCache.GetCount() / 2);

		m_pathes.Clear();
		m_pathPoints.Clear();
		m_strokePoints.Clear();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddPath(PathType type)
{
	Path path;
	path.type = type;
	path.firstIndex = m_pathPoints.GetCount();
	path.pointCount = 0;
	path.closed = false;
	m_pathes.Add(path);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color)
{
	PathPoint pt;
	pt.attr = attr;
	pt.point = point;
	pt.color = color;
	m_pathPoints.Add(pt);
	GetCurrentPath()->pointCount++;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ClosePath()
{
	GetCurrentPath()->closed = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::Path* DrawingContextImpl::GetCurrentPath()
{
	return &m_pathes.GetLast();
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContextImpl::PutMoveTo(const Vector3& pt, const ColorF& color)
//{
//	AddPathPoint(PathPointAttr::MoveTo, pt, color);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContextImpl::PutLineTo(const Vector3& pt, const ColorF& color)
//{
//	AddPathPoint(PathPointAttr::LineTo, pt, color);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ExpandStroke()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);

		switch (path.type)
		{
		case PathType::Line:
			for (int iPt = path.firstIndex; iPt < path.firstIndex+path.pointCount; ++iPt)
			{
				const StrokePoint& pt = m_strokePoints.GetAt(iPt);

				DrawingBasicVertex v;
				v.Position = pt.point;
				v.Color = pt.color;
				m_vertexCache.Add(v);

				uint16_t i = m_indexCache.GetCount();
				m_indexCache.Add(i);
			}
			break;
		case PathType::Rectangle:
		{
			const StrokePoint* pts = &m_strokePoints.GetAt(path.firstIndex);

			DrawingBasicVertex v;
			v.Position = pts[0].point; v.Color = pts[0].color; m_vertexCache.Add(v);
			v.Position = pts[1].point; v.Color = pts[1].color; m_vertexCache.Add(v);
			v.Position = pts[2].point; v.Color = pts[2].color; m_vertexCache.Add(v);
			v.Position = pts[3].point; v.Color = pts[3].color; m_vertexCache.Add(v);

			uint16_t i = m_indexCache.GetCount();
			m_indexCache.Add(i + 0);
			m_indexCache.Add(i + 1);
			m_indexCache.Add(i + 1);
			m_indexCache.Add(i + 3);
			m_indexCache.Add(i + 3);
			m_indexCache.Add(i + 2);
			m_indexCache.Add(i + 2);
			m_indexCache.Add(i + 0);
			break;
		}

		case PathType::Path:
			LN_THROW(0, NotImplementedException);
			break;
		}
	}
}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContextImpl::AddVertex(const Vector3& point, const ColorF& color)
//{
//	DrawingBasicVertex v;
//	v.Position = point;
//	v.Color = color;
//	m_vertexCache.Add(v);
//
//	uint16_t i = m_vertexCache.GetCount();
//	m_indexCache.Add(i);
//}


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
	m_internal = LN_NEW	DrawingContextImpl(m_manager);
	m_commandsBuffer.Resize(1024);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	LN_CALL_COMMAND(SetViewProjection, DrawingContextImpl::SetViewProjectionCommand, view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTransform(const Matrix& matrix)
{
	m_currentState.transform = matrix;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetBrush(Brush* brush)
{
	m_currentState.brush = brush;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetPen(Pen* pen)
{
	m_currentState.pen = pen;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetOpacity(float opacity)
{
	m_currentState.opacity = opacity;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTone(const ToneF& tone)
{
	m_currentState.tone = tone;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetFont(Font* font)
{
	m_currentState.font = font;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	SetDrawingClassInternal(detail::DrawingClass::LineList);
	CheckFlush();

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
	DrawLine(from, to, color, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	CheckFlush();

	DrawingCommands_DrawRectangle cmd;
	cmd.type = DrawingCommandType::DrawRectangle;
	cmd.rect = rect;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
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
	DrawingContextImpl::DrawingState state;
	state.Create(m_currentState);
	LN_CALL_COMMAND(SetState, DrawingContextImpl::SetStateCommand, state);

	LN_CALL_COMMAND(DoCommandList, DrawingContextImpl::DoCommandListCommand, m_commandsBuffer.GetConstData(), m_commandsUsingByte);
	m_commandsUsingByte = 0;

	LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetDrawingClassInternal(detail::DrawingClass dc)
{
	if (m_currentDrawingClass != dc) {
		m_currentDrawingClass = dc;
		m_stateModified = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::CheckFlush()
{
	if (m_stateModified) {
		FlushInternal();
		m_stateModified = false;
	}

	// �R�}���h�o�b�t�@�̐擪�ɋ敪�����Ă���
	AddCommand(&m_currentDrawingClass, sizeof(uint32_t));
}




//=============================================================================
// GraphicsContext
/*		GraphicsContext �� JavaFX �̃N���X�B
*/
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::GraphicsContext(GraphicsManager* manager)
	: m_currentRenderer(RendererType::None)
	, m_spriteRenderer(nullptr)
{
	Renderer = manager->GetRenderer();
	GeometryRenderer = GeometryRenderer::Create(manager);
	m_drawingContext.Initialize(manager);
	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::~GraphicsContext()
{
	LN_SAFE_RELEASE(GeometryRenderer);
	LN_SAFE_RELEASE(m_spriteRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext* GraphicsContext::BeginDrawingContext()
{
	if (m_currentRenderer != RendererType::DrawingContext)
	{
		Flush();
		m_currentRenderer = RendererType::DrawingContext;
	}
	return &m_drawingContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer* GraphicsContext::BeginSpriteRendering()
{
	if (m_currentRenderer != RendererType::DrawingContext)
	{
		Flush();
		m_currentRenderer = RendererType::SpriteRenderer;
	}
	return m_spriteRenderer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Flush()
{
	m_drawingContext.Flush();
	m_spriteRenderer->Flush();
}


LN_NAMESPACE_END
