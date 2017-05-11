
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Graphics/Brush.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

struct FrameRectRendererState
{
	//Matrix						worldTransform;
	Matrix						viewProjTransform;
	BrushImageDrawMode			imageDrawMode;
	ThicknessF					borderThickness;
	RectI						srcRect;
	BrushWrapMode				wrapMode;
	RefPtr<Driver::ITexture>	texture;
};

class FrameRectRendererCore
	: public RefObject
{
public:
	FrameRectRendererCore();
	~FrameRectRendererCore();
	void Initialize(GraphicsManager* manager);

	void SetState(const FrameRectRendererState& state);
	void Draw(const Matrix& transform, const Rect& rect);

private:
	void RequestBuffers(int faceCount);

	void PutRectangleStretch(const Rect& rect, const Rect& srcUVRect);
	void PutRectangleTiling(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture);
	void PutRectangle(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture, BrushWrapMode wrapMode);
	void PutFrameRectangle(const Rect& rect, const ThicknessF& borderThickness, Driver::ITexture* srcTexture, RectI srcRect, BrushWrapMode wrapMode);

	// TODO ���_�錾�Ƃ��͊O��������炤�悤�ɂ�����
	struct Vertex
	{
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

	GraphicsManager*			m_manager;
	Driver::IRenderer*			m_renderer;
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
	Driver::IVertexBuffer*		m_vertexBuffer;
	Driver::IIndexBuffer*		m_indexBuffer;
	CacheBuffer<Vertex>			m_vertexCache;
	CacheBuffer<uint16_t>		m_indexCache;
	FrameRectRendererState		m_state;

	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderTechnique*	technique;
		Driver::IShaderPass*		pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varTone;
		Driver::IShaderVariable*	varTexture;
		Driver::IShaderVariable*	varGlyphMaskSampler;
		Driver::IShaderVariable*	varViewportSize;	///< DX9 HLSL �p�̃s�N�Z���I�t�Z�b�g�v�Z�p�BGLSL �ł͍œK���ɂ������邱�Ƃ�����

	} m_shader;
};

class FrameRectRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	FrameRectRenderer();
	~FrameRectRenderer();
	void Initialize(GraphicsManager* manager);

	void SetViewInfo(const Matrix& viewProj);
	void SetState(Brush* brush);

	void Draw(const Matrix& transform, const Rect& rect);

	virtual bool IsStandaloneShader() const { return true; }
	virtual void Flush() override;
	virtual void OnActivated() { }
	virtual void OnDeactivated() { Flush(); }
	virtual void OnSetState(const DrawElementBatch* state);

private:
	void SetState(Brush* brush, const Matrix& world, const Matrix& viewProj);

	GraphicsManager*		m_manager;
	FrameRectRendererCore*	m_core;

	Matrix					m_viewProj;
	RefPtr<Brush>	m_brush;

	//FrameRectRendererState	m_state;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
