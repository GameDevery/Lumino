/*
	�S�̍\��

		RenderingContext	Renderer	
							PrimitiveRenderer
							SpriteRenderer		
		GraphicsContext		Renderer
							GeometryRenderer
							TextRenderer


		�ŏ�ʂ� RenderingContext �� GraphicsContext�B����ȊO�̓��[�U�[�⑼�̃��W���[���Ɍ��J���Ȃ��B
		RenderingContext �� GraphicsContext �����X�e�[�g�͔r���B
		�������ł�1�� IRenderer ���g�����A�݂��̃X�e�[�g�ύX�͉e�����Ȃ��B

		�܂��A���������̖��� GraphicsContext �� Renderer �𓯎��Ɏg�������Ƃ������Ƃɂ���B
		�Ⴆ�΁AScene �̒��� GUI ��`�������Ƃ��B
		�����łȂ��Ă��A�ŏ��� GraphicsContext �� Renderer �̋@�\���������Ă��܂����Ƃ��Ă������A
		���ꂾ�Ƃ����Ԃł����g���Ȃ��֐������܂�ɂ������Ȃ邵�A�N���X�̋K�͂��傫���Ȃ�B
		�܂胆�[�U�[���C��t���Ȃ���΂Ȃ�Ȃ����Ƃ������Ă��܂��B

		Context �́AIRenderer �ɐݒ�ł���S�Ă̐ݒ��ێ�����B���ƃV�F�[�_�B
		Context �� Draw�` �� Flush �ŁA�A�N�e�B�u�ȃR���e�L�X�g�̐؂�ւ����s���B
		���̂Ƃ��Â��R���e�L�X�g�� Flush ���A�V�����R���e�L�X�g�̃X�e�[�g��S�ēK�p����B
		�؂�ւ��ʒm�� Context ���x���ɒʒm����B��܂��Ă���e�� Renderer �� Flush ���ĂԂ��߁B

		[2015/1/22]
			Renderer == RenderingContext �ł͂Ȃ��̂��HRenderingContext �͕K�v�Ȃ̂��H�Ǝv����������Ȃ��B
			Renderer �ɂ܂Ƃ߂Ă��܂��ƁAGraphicsContext �� Renderer �������x���̃N���X�ƂȂ�B
			GraphicsContext �͕`���̃����_�����O�^�[�Q�b�g���X�e�[�g�Ƃ��Ď����Ȃ���΂Ȃ�Ȃ����߁ARenderer ���܂���K�v������B
			Context �̒��ɕʂ� Context ���ł��Ă��܂��Ƃ������ƁB���ɕ��G�B


	[2015/1/21] SceneGraph �����ɒ჌�x�� Renderer �͌��J����H
		
		���������e�� Renderer ���B���Ă���̂́A���[�U�[�̒m��Ȃ��Ƃ���ŃX�e�[�g���ς����
		�����̂킩��ɂ������ɂȂ�Ȃ��悤�ɂ��邽�߁B


	[2015/1/21] �J�����g�� Shader ���Ǘ�����̂͒N�H
		�~GraphicsContext�B
		�~�X�� Renderer �ɊǗ�������̂͂��܂��낵���Ȃ��C������B
		�~�~PrimitiveRenderer �͂܂� SetShader �Ƃ������Ă������Ǝv�����ǁA
		�~�~GeometryRenderer �� WPF �݂����ɃV�X�e���ɂ���ĕK�����s����� Shader �������āA
		�~���̌�Ń��[�U�[�� Shader ��������������Ȃ��B
		�~�i�܂��A���[�U�[ Shader �̒��� GeometryRenderer �p�̊֐����Ă�ł�����Ă��������j

		�E�E�E�ł��A�X��Rendere�ɔC���Ă��܂��Ă����������H
		Activate �̂Ƃ� GraphicsContext ���ʓ|����K�v�Ȃ��Ƃ������Ƃ����B
		�Ƃ肠�����������̕����ŁB


	[2015/1/11] PrimitiveRenderer �Ƃ͂Ȃ���ʂ���H
		PrimitiveRenderer �͒P���Ȍ`��������E��ʂɕ`�悷�邽�߂Ɏg�p����B

	[2015/1/3] �ʒ���ɂ���
		�Ƃ肠�����˂���p�X�͑z�肵�Ȃ��B����͌y�ʂȑg�ݍ��݃��[�h�Ƃ���B
		�{���ɂ����Ƃ����̂��g�������v�]����΁AExpandFill ������ poly2tri �Œu�������郂�[�h�����B

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
#include <Lumino/Graphics/Shader.h>
#include "TextRenderer.h"
#include "PrimitiveRenderer.h"
#include "RendererImpl.h"

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
	Vector2	UVOffset;		///< �e�N�X�`��UV (�]������UV���)
	Vector4	UVTileUnit;		///< �e�N�X�`��UV (�^�C�����O��Ԃ̂ǂ��ɂ��邩)

							/// ���_���C�A�E�g
	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 1 },
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
	MoveTo,
	LineTo,
	BezierCurveTo,
	DrawPoint,
	DrawLine,
	DrawTriangle,
	DrawRectangle,
	//DrawEllipse,
	ClosePath,

};
struct DrawingCommands_MoveTo
{
	DrawingCommandType	type;
	Vector3				point;
	ColorF				color;
};
struct DrawingCommands_LineTo
{
	DrawingCommandType	type;
	Vector3				point;
	ColorF				color;
};
struct DrawingCommands_BezierCurveTo
{
	DrawingCommandType	type;
	Vector3				cp1;
	Vector3				cp2;
	Vector3				endPoint;
	ColorF				color;
};
struct DrawingCommands_DrawPoint
{
	DrawingCommandType	type;
	Vector3				point;
	ColorF				color;
};
struct DrawingCommands_DrawLine
{
	DrawingCommandType	type;
	Vector3				from;
	Vector3				to;
	ColorF				fromColor;
	ColorF				toColor;
};
struct DrawingCommands_DrawTriangle
{
	DrawingCommandType	type;
	Vector3				p1;
	Vector3				p2;
	Vector3				p3;
	ColorF				p1Color;
	ColorF				p2Color;
	ColorF				p3Color;
};
struct DrawingCommands_DrawRectangle
{
	DrawingCommandType	type;
	RectF				rect;
	ColorF				color;
};
//struct DrawingCommands_DrawEllipse
//{
//	DrawingCommandType	type;
//	Vector3				center;
//	Vector2				radius;
//};
struct DrawingCommands_ClosePath
{
	DrawingCommandType	type;
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

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void PrimitiveCache::DrawSimpleLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
//{
//	DrawingBasicVertex v;
//	v.Position = from;
//	v.Color = fromColor;
//	m_vertexCache.Add(v);
//	v.Position = to;
//	v.Color = toColor;
//	m_vertexCache.Add(v);
//
//	uint16_t i = m_vertexCache.GetCount();
//	m_indexCache.Add(i + 0);
//	m_indexCache.Add(i + 1);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void PrimitiveCache::DrawRectangle(const RectF& rect, const RectF& srcUVRect, const ColorF& color)
//{
//	float lu = srcUVRect.GetLeft();
//	float tv = srcUVRect.GetTop();
//	float uvWidth = srcUVRect.Width;
//	float uvHeight = srcUVRect.Height;
//
//	DrawingBasicVertex v;
//	v.Color = color;
//	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 1);	// ����
//	m_vertexCache.Add(v);
//	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 1);	// �E��
//	m_vertexCache.Add(v);
//	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(1, 2);	// ����
//	m_vertexCache.Add(v);
//	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); //v.UVTileUnit.Set(2, 2);	// �E��
//	m_vertexCache.Add(v);
//
//	uint16_t i = m_vertexCache.GetCount();
//	m_indexCache.Add(i + 0);
//	m_indexCache.Add(i + 1);
//	m_indexCache.Add(i + 2);
//	m_indexCache.Add(i + 2);
//	m_indexCache.Add(i + 1);
//	m_indexCache.Add(i + 3);
//}

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

		Driver::ITexture* SelectTexutre(Driver::ITexture* defaultTexture)
		{
			if (Type == BrushType_Texture) {
				return TextureBrush.Texture;
			}
			else if (Type == BrushType_FrameTexture) {
				return FrameTextureBrush.Texture;
			}
			return defaultTexture;
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
		detail::DrawingClass	drawingClass;
		BrushData	Brush;
		PenData		pen;
		float		Opacity;
		ColorF		ForeColor;		///< ��Z����F�BSolidColorBrush �̎��͂��̐F�ɂȂ�B����� Opacity �̏�Z���ʁB
		ToneF		Tone;

		DrawingState()
			: drawingClass(detail::DrawingClass::PointList)
			, ForeColor(ColorF::White)
			, Opacity(1.0f)
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
			drawingClass = state.drawingClass;

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

		void UpdateCurrentForeColor()
		{
			if (Brush.Type == BrushType_SolidColor)
			{
				ForeColor.R = Brush.SolidColorBrush.Color[0];
				ForeColor.G = Brush.SolidColorBrush.Color[1];
				ForeColor.B = Brush.SolidColorBrush.Color[2];
				ForeColor.A = Opacity * Brush.SolidColorBrush.Color[3];
			}
			else
			{
				ForeColor.Set(1, 1, 1, Opacity);
			}
		}
	};

public:
	DrawingContextImpl(GraphicsManager* manager);
	~DrawingContextImpl();

	void SetViewProjection(const Matrix& view, const Matrix& proj);
	void SetState(const DrawingState& state);
	void DoCommandList(const void* commandBuffer, size_t size, detail::DrawingClass drawingClass);
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
		detail::DrawingClass m_dc;
		void Create(DrawingContextImpl* impl, const void* commandBuffer, size_t size, detail::DrawingClass dc)
		{
			m_impl = impl;
			m_commandBuffer = AllocExtData(size, commandBuffer);
			m_size = size;
			m_dc = dc;
		}
		void Execute() { m_impl->DoCommandList(GetExtData(m_commandBuffer), m_size, m_dc); }
	};

	struct FlushCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		void Create(DrawingContextImpl* impl) { m_impl = impl; }
		void Execute() { m_impl->Flush(); }
	};

private:

	//enum class PathPointAttr
	//{
	//	MoveTo,
	//	LineTo,
	//};

	
	enum class PathType
	{
		Point,
		Line,		// Stroke �̂�
		Rectangle,	// TODO: ����Ȃ�����
		Path,	// ExpandFill �Ȃǂ����s���Ėʒ�����s��
	};

	//struct PathPoint
	//{
	//	PathPointAttr	attr;
	//	Vector3			point;
	//	ColorF			color;
	//};

	struct BasePoint
	{
		Vector3			point;
		ColorF			color;
		Vector3			dir;			// ���̓_�ւ̌���
		float			len;			// ���̓_�Ƃ̋���
		Vector3			extrusionDir;	// �����o������ (dir�ɑ΂��鍶�����B�����𔽓]����ƉE�����ւ̉����o���ʂɂȂ�)

		Vector2			userUV;
		bool			userUVUsed = false;
		

		//Vector3			distance;	// ���̓_�Ƃ̋���
		//Vector3			left;		// �������B�܂�A�X�g���[�N�̉����o������
		//float			len;
		BasePoint(const Vector3& p, const ColorF& c) : point(p), color(c) {}
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
	void AddBasePoint(const Vector3& point, const ColorF& color);
	void AddBasePoint(const Vector3& point, const ColorF& color, const Vector2& userUV, bool userUVUsed);
	//void AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color);
	void EndPath(const Vector3* lastPoint, const ColorF* lastColor, bool pathClose);

	void ExpandPoints();

	void ExpandFill();

	//void PutMoveTo(const Vector3& pt, const ColorF& color);
	//void PutLineTo(const Vector3& pt, const ColorF& color);

	void ExpandStroke(bool vertexOnly);
	void MakeJoint_Bevel(const BasePoint* p0, const BasePoint* p1);

	void AddVertex(const Vector3& point, const ColorF& color);

	static float Normalize(float *x, float* y)
	{
		float d = sqrtf((*x)*(*x) + (*y)*(*y));
		if (d > 1e-6f) {
			float id = 1.0f / d;
			*x *= id;
			*y *= id;
		}
		return d;
	}

	GraphicsManager*		m_manager;
	//PrimitiveCache			m_primitiveCache;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	Matrix					m_view;
	Matrix					m_proj;
	DrawingState			m_currentState;

	//CacheBuffer<PathPoint>	m_pathPoints;
	CacheBuffer<Path>		m_pathes;
	bool					m_pathCreating;

	CacheBuffer<BasePoint>	m_basePoints;

	CacheBuffer<DrawingBasicVertex>	m_vertexCache;	// ���_�o�b�t�@�{�̂����T�C�Y���邱�ƂɂȂ�Ɨ]�v�Ɏ��Ԃ�������̂ŁA�܂��͂����ɍ���Ă���R�s�[����
	CacheBuffer<uint16_t>			m_indexCache;
	

	// DrawingContext3D
	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderVariable*    varWorldMatrix;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varPixelStep;
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
	, m_pathCreating(false)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
	//m_pathPoints.Reserve(1024);
	m_pathes.Reserve(1024);
	m_basePoints.Reserve(1024);
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
	m_shader3D.varPixelStep = m_shader3D.shader->GetVariableByName(_T("g_pixelStep"));
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
	m_currentState.UpdateCurrentForeColor();
}

static float BezierCurve(float x1, float x2, float x3, float x4, float t)
{
	float tp = 1.f - t;
	return t*t*t*x4 + 3.f*t*t*tp*x3 + 3.f*t*tp*tp*x2 + tp*tp*tp*x1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::DoCommandList(const void* commandBuffer, size_t size, detail::DrawingClass drawingClass)
{
	const byte_t* pos = (const byte_t*)commandBuffer;
	const byte_t* end = pos + size;
	m_currentState.drawingClass = drawingClass;

	const Vector3* lastPoint = nullptr;
	const ColorF* lastColor = nullptr;
	m_pathCreating = false;

	while (pos < end)
	{
		DrawingCommandType type = *((const DrawingCommandType*)pos);

		if (drawingClass == detail::DrawingClass::PathStroke)
		{
			switch (type)
			{
				////////////////////////////////////////////////////////////
				case DrawingCommandType::MoveTo:
				{
					AddPath(PathType::Path);
					auto* cmd = (const DrawingCommands_MoveTo*)pos;
					lastPoint = &cmd->point;
					lastColor = &cmd->color;
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::LineTo:
				{
					auto* cmd = (const DrawingCommands_LineTo*)pos;
					if (!GetCurrentPath()->closed)
					{
						AddBasePoint(*lastPoint, *lastColor);
						lastPoint = &cmd->point;
						lastColor = &cmd->color;
					}
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::BezierCurveTo:
				{
					auto* cmd = (const DrawingCommands_BezierCurveTo*)pos;
					if (!GetCurrentPath()->closed)
					{
						// 
						float lenMax = ((*lastPoint) - cmd->endPoint).GetLength() * 2;
						int divCount = lenMax / 10;// TODO:pixel
						for (float i = 0; i < divCount; i += 1.f)
						{
							float t = i / divCount;
							AddBasePoint(
								Vector3(
									BezierCurve(lastPoint->X, cmd->cp1.X, cmd->cp2.X, cmd->endPoint.X, t),
									BezierCurve(lastPoint->Y, cmd->cp1.Y, cmd->cp2.Y, cmd->endPoint.Y, t),
									cmd->endPoint.Z),	// TODO
								ColorF::Lerp(*lastColor, cmd->color, t));
						}
						lastPoint = &cmd->endPoint;
						lastColor = &cmd->color;
					}
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::ClosePath:
				{
					auto* cmd = (const DrawingCommands_ClosePath*)pos;
					EndPath(lastPoint, lastColor, true);
					pos += sizeof(*cmd);
					break;
				}
				////////////////////////////////////////////////////////////
				default:
					LN_THROW(0, InvalidOperationException);
					return;
			}
		}
		else
		{
			switch (type)
			{
				////////////////////////////////////////////////////////////
				case DrawingCommandType::DrawPoint:
				{
					// Point �͘A������ DrawingCommandType::DrawPoint ��S��1�� Path �ɂ܂Ƃ߂�
					AddPath(PathType::Point);
					do
					{
						auto* cmd = (const DrawingCommands_DrawPoint*)pos;
						AddBasePoint(cmd->point, cmd->color);
						pos += sizeof(DrawingCommands_DrawPoint);

					} while (pos < end && *((const DrawingCommandType*)pos) == DrawingCommandType::DrawPoint);

					EndPath(nullptr, nullptr, false);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::DrawLine:
				{
					auto* cmd = (const DrawingCommands_DrawLine*)pos;

					AddPath(PathType::Line);
					AddBasePoint(cmd->from, cmd->fromColor);
					AddBasePoint(cmd->to, cmd->toColor);
					EndPath(nullptr, nullptr, false);

					pos += sizeof(DrawingCommands_DrawLine);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::DrawTriangle:
				{
					auto* cmd = (const DrawingCommands_DrawTriangle*)pos;
					AddPath(PathType::Path);
					AddBasePoint(cmd->p1, cmd->p1Color);
					AddBasePoint(cmd->p2, cmd->p2Color);
					AddBasePoint(cmd->p3, cmd->p3Color);
					EndPath(nullptr, nullptr, true);
					pos += sizeof(DrawingCommands_DrawTriangle);
					break;
				}
				////////////////////////////////////////////////////////////
				case DrawingCommandType::DrawRectangle:
				{
					auto* cmd = (const DrawingCommands_DrawRectangle*)pos;
					const RectF& rect = cmd->rect;

					AddPath(PathType::Rectangle);
					AddBasePoint(Vector3(rect.GetLeft(), rect.GetTop(), 0), cmd->color);
					AddBasePoint(Vector3(rect.GetRight(), rect.GetTop(), 0), cmd->color);
					AddBasePoint(Vector3(rect.GetRight(), rect.GetBottom(), 0), cmd->color);
					AddBasePoint(Vector3(rect.GetLeft(), rect.GetBottom(), 0), cmd->color);
					EndPath(nullptr, nullptr, true);

					pos += sizeof(DrawingCommands_DrawRectangle);
					break;
				}
				default:
					LN_THROW(0, InvalidOperationException);
					return;
			}
		}
	}

	EndPath(lastPoint, lastColor, false);
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
	if (m_shader3D.varPixelStep != nullptr)
	{
		const Size& size = m_manager->GetRenderer()->GetRenderTarget(0)->GetSize();
		m_shader3D.varPixelStep->SetVector(Vector4(0.5f / size.Width, 0.5f / size.Height, 0, 0));
	}

	if (m_currentState.drawingClass == detail::DrawingClass::PathStroke)
	{
		ExpandStroke(false);	// TODO test
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		renderer->SetVertexBuffer(m_vertexBuffer);
		m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, m_vertexCache.GetCount()-2);
	}
	// PointList
	else if (m_currentState.drawingClass == detail::DrawingClass::PointList)
	{
		ExpandPoints();
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		renderer->SetVertexBuffer(m_vertexBuffer);
		m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitive(PrimitiveType_PointList, 0, m_vertexCache.GetCount());
	}
	else
	{
		ExpandFill();

		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

		renderer->SetVertexBuffer(m_vertexBuffer);
		renderer->SetIndexBuffer(m_indexBuffer);
		m_shader3D.varTexture->SetTexture(m_currentState.Brush.SelectTexutre(m_manager->GetDummyTexture()));
		m_shader3D.passP0->Apply();
		renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
	}

#if 0
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

	}
#endif
	m_pathes.Clear();
	//m_pathPoints.Clear();
	m_basePoints.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddPath(PathType type)
{
	if (!m_pathCreating)
	{
		Path path;
		path.type = type;
		path.firstIndex = m_basePoints.GetCount();//m_pathPoints.GetCount();
		path.pointCount = 0;
		path.closed = false;
		m_pathes.Add(path);
		m_pathCreating = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void DrawingContextImpl::AddPathPoint(PathPointAttr attr, const Vector3& point, const ColorF& color)
//{
//	PathPoint pt;
//	pt.attr = attr;
//	pt.point = point;
//	pt.color = color;
//	m_pathPoints.Add(pt);
//	GetCurrentPath()->pointCount++;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddBasePoint(const Vector3& point, const ColorF& color)
{
	BasePoint pt(point, color);
	pt.color.R *= m_currentState.ForeColor.R;
	pt.color.G *= m_currentState.ForeColor.G;
	pt.color.B *= m_currentState.ForeColor.B;
	pt.color.A *= m_currentState.ForeColor.A;
	m_basePoints.Add(pt);
	GetCurrentPath()->pointCount++;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::AddBasePoint(const Vector3& point, const ColorF& color, const Vector2& userUV, bool userUVUsed)
{
	BasePoint pt(point, color);
	pt.color.R *= m_currentState.ForeColor.R;
	pt.color.G *= m_currentState.ForeColor.G;
	pt.color.B *= m_currentState.ForeColor.B;
	pt.color.A *= m_currentState.ForeColor.A;
	pt.userUV = userUV;
	pt.userUVUsed = userUVUsed;
	m_basePoints.Add(pt);
	GetCurrentPath()->pointCount++;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::EndPath(const Vector3* lastPoint, const ColorF* lastColor, bool pathClose)
{
	if (m_pathCreating)
	{
		if (lastPoint != nullptr && lastColor != nullptr) {
			AddBasePoint(*lastPoint, *lastColor);
		}
		Path* curPath = GetCurrentPath();
		curPath->closed = pathClose;
		m_pathCreating = false;

		// �e���_�ɂ��āA�����Ȃǂ̏������B
		// �I�[�̓_�͐擪�̓_�Ƃ̋����ɂȂ�B
		if (curPath->pointCount >= 2)
		{
			BasePoint* p0 = &m_basePoints.GetAt(curPath->pointCount - 1);
			BasePoint* p1 = &m_basePoints.GetAt(curPath->firstIndex);
			for (int i = 0; i < curPath->pointCount; ++i)
			{
				p0->dir.X = p1->point.X - p0->point.X;
				p0->dir.Y = p1->point.Y - p0->point.Y;
				p0->len = Normalize(&p0->dir.X, &p0->dir.Y);	// dir �𐳋K��������Œ�����Ԃ�

				//p0->distance.X = p1->point.X - p0->point.X;
				//p0->distance.Y = p1->point.Y - p0->point.Y;
				//p0->left = Vector3::Cross(p0->distance, Vector3::UnitZ);
				//p0->left.Normalize();
				//p0->len = p0->distance.GetLength();
				p0 = p1++;
			}

			// �����o�����������߂�
			p0 = &m_basePoints.GetAt(curPath->pointCount - 1);
			p1 = &m_basePoints.GetAt(curPath->firstIndex);
			for (int i = 0; i < curPath->pointCount; ++i)
			{
				//p1->extrusion.X = (p0->dir.Y + p1->dir.Y);
				//p1->extrusion.Y = (p0->dir.X + p1->dir.X);
				//Normalize(&p1->extrusion.X, &p1->extrusion.Y);
				p1->extrusionDir = Vector3::Cross(p1->dir, Vector3::UnitZ);
				p0 = p1++;
			}
		}
	}
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
void DrawingContextImpl::ExpandPoints()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);

		// ���_�o�b�t�@�����
		for (int i = 0; i < path.pointCount; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			DrawingBasicVertex v;
			v.Position = pt.point;
			v.Color = pt.color;
			m_vertexCache.Add(v);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ExpandFill()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);

		int count = path.pointCount;
		if (count < 3) continue;

		// ���E�̋�`�����߂Ă���
		Vector2 posMin(FLT_MAX, FLT_MAX);
		Vector2 posMax(FLT_MIN, FLT_MIN);
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			posMin.X = std::min(posMin.X, pt.point.X);
			posMin.Y = std::min(posMin.Y, pt.point.Y);
			posMax.X = std::max(posMax.X, pt.point.X);
			posMax.Y = std::max(posMax.Y, pt.point.Y);
		}
		Vector2 uvSpan = posMax - posMin;
		if (uvSpan.X == 0.0f || uvSpan.Y == 0.0) continue;	// �ׂꂽ�p�X�B�`���K�v�͂Ȃ�
		uvSpan = 1.0f / uvSpan;

		// ���_�o�b�t�@�����
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex+i);
			DrawingBasicVertex v;
			v.Position = pt.point;
			v.Color = pt.color;
			v.UVOffset.X = (pt.point.X - posMin.X) * uvSpan.X;
			v.UVOffset.Y = (pt.point.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);
		}

		// �C���f�b�N�X�o�b�t�@����� (���v���)
		int ib = path.firstIndex;
		int i0 = 0;
		int i1 = 1;
		int i2 = count - 1;
		for (int iPt = 0; iPt < count - 2; ++iPt)
		{
			m_indexCache.Add(ib + i0);
			m_indexCache.Add(ib + i1);
			m_indexCache.Add(ib + i2);

			if (iPt & 1) {	// ���
				i0 = i2;
				--i2;
			}
			else {	// ������
				i0 = i1;
				++i1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::ExpandStroke(bool vertexOnly)
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	{
		const Path& path = m_pathes.GetAt(iPath);
		int count = path.pointCount;

#if 0
		// ���E�̋�`�����߂Ă���
		Vector2 posMin(FLT_MAX, FLT_MAX);
		Vector2 posMax(FLT_MIN, FLT_MIN);
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);
			posMin.X = std::min(posMin.X, pt.point.X);
			posMin.Y = std::min(posMin.Y, pt.point.Y);
			posMax.X = std::max(posMax.X, pt.point.X);
			posMax.Y = std::max(posMax.Y, pt.point.Y);
		}
		Vector2 uvSpan = posMax - posMin;
		if (uvSpan.X == 0.0f || uvSpan.Y == 0.0) {
			// �p�X���Ԃ�Ă���B���A1�_�����`���Ƃ����蓾��B���̏ꍇuv��(0,0) �Ƃ���B
		}
		else {
			uvSpan = 1.0f / uvSpan;
		}
#endif

		// �p�X�����Ă���� p0 ��[last]�Ap1��[0] �Ƃ��Ĉȉ��̏������J�n
		// �p�X�����Ă��Ȃ���� p0 ��[0]�Ap1��[1] �Ƃ��Ĉȉ��̏������J�n
		BasePoint* p0 = &m_basePoints.GetAt(path.firstIndex + path.pointCount - 1);
		BasePoint* p1 = &m_basePoints.GetAt(path.firstIndex);
		if (!path.closed)
		{
			p0 = p1++;
			--count;	// ���[�v����-1
		}
		else {
			// �p�X�����Ă��鎞�́A�ŏ��� last->begin �֐���������邱�ƂɂȂ�B
		}

		// ���_�o�b�t�@�����
		for (int i = 0; i < count; ++i)
		{
			const BasePoint& pt = *p0;//m_basePoints.GetAt(path.firstIndex + i);
			DrawingBasicVertex v;

			//MakeJoint_Bevel(p0, p1);

			//v.Position = pt.point - (pt.left * 5);	// ����
			//v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);
			//v.Position = p0->point + (p0->left * 5);	// �O��
			//v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);


			//v.Position = pt.point + (pt.extrusionDir * 5);	// �O��
			//v.Color = pt.color;
			////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);

			// �����̎��_����

			v.Position = pt.point - (pt.extrusionDir * 5);	// ����
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);

			v.Position = pt.point + (pt.extrusionDir * 5);	// �O��
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);

			v.Position = p1->point - (p0->extrusionDir * 5);	// ����
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);

			v.Position = p1->point + (p0->extrusionDir * 5);	// �O��
			v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			m_vertexCache.Add(v);


			
			/*if (i == count - 1)
			{

			}
			else *//*if (!path.closed && i == count - 2)
			{
			}
			else*/
			{
				// p1 �� p2 �ւ̐ڍ����s��

				v.Position = p1->point - (p0->extrusionDir * 5);	// ����
				v.Color = pt.color;
				//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
				//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
				m_vertexCache.Add(v);

				v.Position = p1->point + (p1->extrusionDir * 5);	// �O��
				v.Color = pt.color;
				//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
				//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
				m_vertexCache.Add(v);
			}


			//v.Position = p1->point + (p0->left * 5);	// �O��
			//v.Color = pt.color;
			//v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
			//v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
			//m_vertexCache.Add(v);

			/*
				���j
				p1 �̓_��łBp0�͂����ł���Ă��� (�I�_�͂܂�����)
				�܂�Ƃ������Ap0��p1�̐��������̂�1���[�v�B
				p1�������̒��S�ŁAp0�́u1�O�̓_�v
				�ڍ��_�̏�����p0�̕����ɑ΂��čs���B
			*/


			p0 = p1++;
		}

		// �p�X�����Ă���� end->begin �ւ�����
		//if (path.closed)
		//{
		//	const BasePoint& pt = m_basePoints.GetAt(path.firstIndex + i);

		//	DrawingBasicVertex v;
		//	v.Position = pt.point;
		//	v.Color = pt.color;
		//	v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
		//	v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
		//	m_vertexCache.Add(v);

		//	v.Position = pt.point + (pt.left * 5);
		//	v.Color = pt.color;
		//	v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
		//	v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
		//	m_vertexCache.Add(v);
		//}
	}

	//for (int iPath = 0; iPath < m_pathes.GetCount(); ++iPath)
	//{
	//	const Path& path = m_pathes.GetAt(iPath);

	//	switch (path.type)
	//	{
	//	case PathType::Line:
	//		for (int iPt = path.firstIndex; iPt < path.firstIndex+path.pointCount; ++iPt)
	//		{
	//			const BasePoint& pt = m_basePoints.GetAt(iPt);

	//			DrawingBasicVertex v;
	//			v.Position = pt.point;
	//			v.Color = pt.color;
	//			m_vertexCache.Add(v);

	//			uint16_t i = m_indexCache.GetCount();
	//			m_indexCache.Add(i);
	//		}
	//		break;
	//	case PathType::Rectangle:
	//	{
	//		const BasePoint* pts = &m_basePoints.GetAt(path.firstIndex);

	//		DrawingBasicVertex v;
	//		v.Position = pts[0].point; v.Color = pts[0].color; m_vertexCache.Add(v);
	//		v.Position = pts[1].point; v.Color = pts[1].color; m_vertexCache.Add(v);
	//		v.Position = pts[2].point; v.Color = pts[2].color; m_vertexCache.Add(v);
	//		v.Position = pts[3].point; v.Color = pts[3].color; m_vertexCache.Add(v);

	//		uint16_t i = m_indexCache.GetCount();
	//		m_indexCache.Add(i + 0);
	//		m_indexCache.Add(i + 1);
	//		m_indexCache.Add(i + 1);
	//		m_indexCache.Add(i + 3);
	//		m_indexCache.Add(i + 3);
	//		m_indexCache.Add(i + 2);
	//		m_indexCache.Add(i + 2);
	//		m_indexCache.Add(i + 0);
	//		break;
	//	}

	//	case PathType::Path:
	//		LN_THROW(0, NotImplementedException);
	//		break;
	//	}
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::MakeJoint_Bevel(const BasePoint* p0, const BasePoint* p1)
{
	//DrawingBasicVertex v;
	//v.Position = p0->point - (p0->left * 5);	// ����
	//v.Color = p0->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);

	//v.Position = p0->point + (p0->left * 5);	// �O��
	//v.Color = p0->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);

	//v.Position = p1->point - (p0->left * 5);	// ����
	//v.Color = p1->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);

	//v.Position = p1->point + (p0->left * 5);	// �O��
	//v.Color = p1->color;
	////v.UVOffset.X = (v.Position.X - posMin.X) * uvSpan.X;
	////v.UVOffset.Y = (v.Position.Y - posMin.Y) * uvSpan.Y;
	//m_vertexCache.Add(v);
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
	, m_flushRequested(false)
	, m_internalTextureBrush(nullptr)
{
	m_internalTextureBrush = LN_NEW TextureBrush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_internalTextureBrush);
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
void DrawingContext::SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize)
{
	m_uvParPixel.X = 1.0f / viewPixelSize.Width;
	m_uvParPixel.Y = 1.0f / viewPixelSize.Height;
	m_invViewProj = Matrix::Inverse(view * proj);
	m_viewOnePixelOffset = Vector2::TransformCoord(m_uvParPixel, m_invViewProj);
	LN_CALL_COMMAND(SetViewProjection, DrawingContextImpl::SetViewProjectionCommand, view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTransform(const Matrix& matrix)
{
	if (m_currentState.transform != matrix)
	{
		FlushInternal();
		m_currentState.transform = matrix;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetBrush(Brush* brush)
{
	if (m_currentState.brush != brush)
	{
		FlushInternal();
		m_currentState.brush = brush;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetPen(Pen* pen)
{
	if (m_currentState.pen != pen)
	{
		FlushInternal();
		m_currentState.pen = pen;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetOpacity(float opacity)
{
	// TODO: opacity ��Flush����Ȃ��Ǝv���B���ڒ��_�F�ɏ�Z���Ă��������������H
	if (m_currentState.opacity != opacity)
	{
		FlushInternal();
		m_currentState.opacity = opacity;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetTone(const ToneF& tone)
{
	if (m_currentState.tone != tone)
	{
		FlushInternal();
		m_currentState.tone = tone;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetFont(Font* font)
{
	if (m_currentState.font != font)
	{
		FlushInternal();
		m_currentState.font = font;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::MoveTo(const Vector3& point, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_MoveTo cmd;
	cmd.type = DrawingCommandType::MoveTo;
	cmd.point = point;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::LineTo(const Vector3& point, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_LineTo cmd;
	cmd.type = DrawingCommandType::LineTo;
	cmd.point = point;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_BezierCurveTo cmd;
	cmd.type = DrawingCommandType::BezierCurveTo;
	cmd.cp1 = cp1;
	cmd.cp2 = cp2;
	cmd.endPoint = endPt;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::ClosePath()
{
	SetDrawingClassInternal(detail::DrawingClass::PathStroke);
	DrawingCommands_ClosePath cmd;
	cmd.type = DrawingCommandType::ClosePath;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawPoint(const Vector3& point, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::PointList);
	DrawingCommands_DrawPoint cmd;
	cmd.type = DrawingCommandType::DrawPoint;
	cmd.point = point;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	SetDrawingClassInternal(detail::DrawingClass::LineList);
	//CheckFlush();

	DrawingCommands_DrawLine cmd;
	cmd.type = DrawingCommandType::DrawLine;
	cmd.from = from;
	cmd.to = to;
	cmd.fromColor = fromColor;
	cmd.toColor = toColor;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
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
void DrawingContext::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	DrawingCommands_DrawTriangle cmd;
	cmd.type = DrawingCommandType::DrawTriangle;
	cmd.p1 = p1; cmd.p1Color = p1Color;
	cmd.p2 = p2; cmd.p2Color = p2Color;
	cmd.p3 = p3; cmd.p3Color = p3Color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	SetDrawingClassInternal(detail::DrawingClass::TriangleList);
	//CheckFlush();

	DrawingCommands_DrawRectangle cmd;
	cmd.type = DrawingCommandType::DrawRectangle;
	cmd.rect = rect;
	cmd.color = color;
	AddCommand(&cmd, sizeof(cmd));
	m_flushRequested = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
{
	bool flish = false;
	if (texture != m_internalTextureBrush->GetTexture())
	{
		m_internalTextureBrush->SetTexture(texture);
		flish = true;
	}
	else if (m_currentState.brush != m_internalTextureBrush)
	{
		flish = true;
	}
	if (flish)
	{
		FlushInternal();
		m_currentState.brush = m_internalTextureBrush;
	}
	m_internalTextureBrush->SetSourceRect(srcRect);
	DrawRectangle(rect, color);
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
	if (m_flushRequested)
	{
		DrawingContextImpl::DrawingState state;
		state.Create(m_currentState);
		LN_CALL_COMMAND(SetState, DrawingContextImpl::SetStateCommand, state);

		LN_CALL_COMMAND(DoCommandList, DrawingContextImpl::DoCommandListCommand, m_commandsBuffer.GetConstData(), m_commandsUsingByte, m_currentDrawingClass);
		m_commandsUsingByte = 0;

		LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);

		m_flushRequested = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::SetDrawingClassInternal(detail::DrawingClass dc)
{
	if (m_currentDrawingClass != dc)
	{
		FlushInternal();
		m_currentDrawingClass = dc;
		// �R�}���h�o�b�t�@�̐擪�ɋ敪�����Ă���
		//AddCommand(&m_currentDrawingClass, sizeof(uint32_t));
	}
}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void DrawingContext::CheckFlush()
//{
//		FlushInternal();
//	}
//
//}




//=============================================================================
// GraphicsContext
/*		GraphicsContext �� JavaFX �̃N���X�B
*/
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext* GraphicsContext::GetContext()
{
	return GraphicsManager::Instance->GetGraphicsContext();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::GraphicsContext(GraphicsManager* manager)
	: m_currentRenderer(RendererType::None)
	, m_spriteRenderer(nullptr)
	, m_textRenderer(nullptr)
{
	Renderer = manager->GetRenderer();
	m_drawingContext.Initialize(manager);
	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:

	m_textRenderer = LN_NEW detail::TextRenderer();
	m_textRenderer->Initialize(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::~GraphicsContext()
{
	LN_SAFE_RELEASE(m_spriteRenderer);
	LN_SAFE_RELEASE(m_textRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Set2DRenderingMode(float minZ, float maxZ)
{
	const Size& size = Renderer->GetRenderTarget(0)->GetSize();
	Matrix proj = Matrix::Perspective2DLH(size.Width, size.Height, minZ, maxZ);
	m_drawingContext.SetViewProjection(Matrix::Identity, proj, size);
	m_spriteRenderer->SetViewProjMatrix(Matrix::Identity, proj);
	m_textRenderer->SetViewProjMatrix(proj);
	m_textRenderer->SetViewPixelSize(size);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::SetBrush(Brush* brush)
{
	m_drawingContext.SetBrush(brush);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::SetOpacity(float opacity)
{
	m_drawingContext.SetOpacity(opacity);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::MoveTo(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.MoveTo(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::LineTo(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.LineTo(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.BezierCurveTo(cp1, cp2, endPt, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::ClosePath()
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.ClosePath();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawPoint(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawPoint(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawRectangle(rect, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawEllipse(center, radius);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::DrawingContext);
	m_drawingContext.DrawTexture(rect, texture, srcRect, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawText(const PointF& position, const StringRef& text)
{
	TryChangeRenderingClass(RendererType::TextRenderer);
	m_textRenderer->DrawString(text.GetBegin(), text.GetLength(), position);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Flush()
{
	m_drawingContext.Flush();
	m_spriteRenderer->Flush();
	m_textRenderer->Flush();
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
void GraphicsContext::TryChangeRenderingClass(RendererType dc)
{
	if (dc != m_currentRenderer)
	{
		Flush();
		m_currentRenderer = dc;
	}

}

LN_NAMESPACE_END
