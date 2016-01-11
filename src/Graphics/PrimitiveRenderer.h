
#pragma once
#include <Lumino/Graphics/Bitmap.h>
#include <Lumino/Graphics/Font.h>
#include <Lumino/Graphics/Painter.h>
#include "PainterEngine.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

namespace detail
{

enum class PrimitiveRendererMode
{
	LineList,
	TriangleList,
};

class PrimitiveRendererCore
	: public RefObject
{
public:
	struct DrawSquareData
	{
		Vector3 pos[4];
		Vector2 uv[4];
		ColorF color[4];
	};

	PrimitiveRendererCore();
	~PrimitiveRendererCore();
	void Initialize(GraphicsManager* manager);

	void SetState(const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize, bool useInternalShader, PrimitiveRendererMode mode);
	void DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);
	void DrawSquare(const DrawSquareData& data);
	void Flush();

private:
	void AddVertex(const Vector3& pos, const Vector2& uv, const ColorF& color);
	int GetCurrentVertexCount() const { return m_vertexCacheUsed / m_vertexStride; }

	// TODO ���_�錾�Ƃ��͊O��������炤�悤�ɂ�����
	struct Vertex
	{
	public:
		Vector3	position;
		Vector4	color;
		Vector2	uv;

		// ���_���C�A�E�g
		static VertexElement* Elements()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	ByteBuffer				m_vertexCache;
	size_t					m_vertexCacheUsed;
	size_t					m_vertexStride;
	CacheBuffer<uint16_t>	m_indexCache;
	Driver::ITexture*		m_foreTexture;
	PrimitiveRendererMode	m_mode;
	bool					m_useInternalShader;

	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderTechnique*	technique;
		Driver::IShaderPass*		pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varPixelStep;
		Driver::IShaderVariable*	varTexture;

	} m_shader;
};

class PrimitiveRenderer
	: public RefObject
{
public:
	PrimitiveRenderer();
	~PrimitiveRenderer();
	void Initialize(GraphicsManager* manager);

	void SetTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);
	void SetViewPixelSize(const Size& size);
	void SetUseInternalShader(bool useInternalShader);
	//void SetTexture(Texture* texture);

	void DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const ColorF& color1,
		const Vector3& position2, const Vector2& uv2, const ColorF& color2,
		const Vector3& position3, const Vector2& uv3, const ColorF& color3,
		const Vector3& position4, const Vector2& uv4, const ColorF& color4);

	void Flush();

private:
	void SetPrimitiveRendererMode(PrimitiveRendererMode mode);
	void CheckUpdateState();

	GraphicsManager*		m_manager;
	PrimitiveRendererCore*	m_core;
	Matrix					m_transform;
	Matrix					m_viewProj;
	Size					m_viewPixelSize;
	Texture*				m_texture;
	PrimitiveRendererMode	m_mode;
	bool					m_useInternalShader;
	bool					m_stateModified;
	bool					m_flushRequested;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END