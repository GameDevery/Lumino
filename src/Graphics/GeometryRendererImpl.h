
#pragma once

#include <Lumino/Graphics/GeometryRenderer.h>
#include "Device/DeviceInterface.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{
/// GeometryRenderer �̎����N���X
class GeometryRendererImpl
	: public GeometryRenderer
{
public:
	GeometryRendererImpl(GraphicsManager* manager);
	virtual ~GeometryRendererImpl();
};

/// GeometryRenderer �̃R�}���h���s�N���X
class GeometryRendererProxy
{
public:
	GeometryRendererProxy(GraphicsManager* manager);
	virtual ~GeometryRendererProxy();
};

/// GeometryRenderer �̕`��N���X
///		���̃N���X�͕`��X���b�h�Ő�������A�e�@�\���Ă΂��B
///		���̃X���b�h����̓A�N�Z�X�ł��Ȃ��B
class GeometryRendererCore
	: public RefObject
{
public:
	GeometryRendererCore(Device::IGraphicsDevice* device);
	virtual ~GeometryRendererCore();

public:

	/// �`��p�X�̊J�n (�Ăяo���͔C�ӁB�Ăяo���Ȃ��ꍇ�ADraw�` �n�͂������_�o�b�t�@�� DrawPrimitive ���邾���ɂȂ�)
	void BeginPass();

	/// �`��p�X�̏I��
	void EndPass();

	/// ���W�ϊ��s��̐ݒ�
	void SetTransform(const Matrix& matrix);

	/// ���W�ϊ��s��̐ݒ�
	void SetViewProjTransform(const Matrix& matrix);

	/// �e�N�X�`���̐ݒ� (NULL �Ń_�~�[�e�N�X�`�����g�p����)
	void SetTexture(Device::ITexture* texture);

	/// �����̕`��
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);

	/// �l�p�`�̕`��
	void DrawSquare(
		float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
		float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
		float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
		float x4, float y4, float z4, float u4, float v4, const ColorF& c4);

	/// ��`�̕`��     
	void DrawRect(const RectF& destRect, const RectF& texUVRect, const ColorF& color);

public:

	/// �`��p�̒��_
	struct Vertex
	{
		Vector3		Position;   ///< ���W
		ColorF		Color;      ///< ���_�J���[
		Vector2		TexUV;      ///< �e�N�X�`�����W

		static VertexElement* GetLayout()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
	};

private:
	RefPtr<Device::IGraphicsDevice>	m_device;
	RefPtr<Device::IRenderer>		m_renderer;
	RefPtr<Device::ITexture>		m_dummyTexture;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
	Device::IShaderPass*			m_currentShaderPass;

	/// �V�F�[�_�֌W�̕ϐ����܂Ƃ߂��\����
	struct
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderVariable*    varWorldMatrix;
		Device::IShaderVariable*    varViewProjMatrix;
		Device::IShaderVariable*    varTexture;
		Device::IShaderTechnique*   techMainDraw;
		Device::IShaderPass*        passP0;

	} m_shaderParam;

public:

	struct BeginPassCommand : public RenderingCommand
	{
		GeometryRendererCore* m_renderer;
		void Create(GeometryRendererCore* renderer) { m_renderer = renderer; MarkGC(m_renderer); }
		void Execute() { m_renderer->BeginPass(); }
	};

	struct EndPassCommand : public RenderingCommand
	{
		GeometryRendererCore* m_renderer;
		void Create(GeometryRendererCore* renderer) { m_renderer = renderer; MarkGC(m_renderer); }
		void Execute() { m_renderer->EndPass(); }
	};

	struct SetTransformCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Matrix					m_transform;
		void Create(GeometryRendererCore* renderer, const Matrix& transform)
		{
			m_renderer = renderer;
			m_transform = transform;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetTransform(m_transform); }
	};

	struct SetViewProjTransformCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Matrix					m_transform;
		void Create(GeometryRendererCore* renderer, const Matrix& transform)
		{
			m_renderer = renderer;
			m_transform = transform;
			MarkGC(m_renderer);
		}
		void Execute(){ m_renderer->SetViewProjTransform(m_transform); }
	};

	struct SetTextureCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Device::ITexture*		m_texture;
		void Create(GeometryRendererCore* renderer, Device::ITexture* texture)
		{
			m_renderer = renderer;
			m_texture = texture;
			MarkGC(m_renderer);
			MarkGC(m_texture);
		}
		void Execute() { m_renderer->SetTexture(m_texture); }
	};

	struct DrawLineCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Vector3					m_from, m_to;
		ColorF					m_fromColor, m_toColor;
		void Create(GeometryRendererCore* renderer, const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
		{
			m_renderer = renderer;
			m_from = from;
			m_to = to;
			m_fromColor = fromColor;
			m_toColor = toColor;
			MarkGC(m_renderer);
		}
		virtual void Execute(){ m_renderer->DrawLine(m_from, m_to, m_fromColor, m_toColor); }
	};

	struct DrawSquareCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Vertex m_vertices[4];

		void Create(GeometryRendererCore* renderer,
			const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
		{
			m_renderer = renderer;
			m_vertices[0] = v1;
			m_vertices[1] = v2;
			m_vertices[2] = v3;
			m_vertices[3] = v4;
			MarkGC(m_renderer);
		}
		void Execute()
		{
			m_renderer->DrawSquare(
				m_vertices[0].Position.X, m_vertices[0].Position.Y, m_vertices[0].Position.Z, m_vertices[0].TexUV.X, m_vertices[0].TexUV.Y, m_vertices[0].Color,
				m_vertices[1].Position.X, m_vertices[1].Position.Y, m_vertices[1].Position.Z, m_vertices[1].TexUV.X, m_vertices[1].TexUV.Y, m_vertices[1].Color,
				m_vertices[2].Position.X, m_vertices[2].Position.Y, m_vertices[2].Position.Z, m_vertices[2].TexUV.X, m_vertices[2].TexUV.Y, m_vertices[2].Color,
				m_vertices[3].Position.X, m_vertices[3].Position.Y, m_vertices[3].Position.Z, m_vertices[3].TexUV.X, m_vertices[3].TexUV.Y, m_vertices[3].Color);
		}
	};

	struct DrawRectCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		RectF					m_destRect;
		RectF					m_texUVRect;
		ColorF					m_color;

		void Create(GeometryRendererCore* renderer, const RectF& destRect, const RectF& texUVRect, const ColorF& color)
		{
			m_renderer = renderer;
			m_destRect = destRect;
			m_texUVRect = texUVRect;
			m_color = color;
			MarkGC(m_renderer);
		}
		void Execute()
		{
			m_renderer->DrawRect(m_destRect, m_texUVRect, m_color);
		}
	};
};

} // namespace Graphics
} // namespace Lumino
