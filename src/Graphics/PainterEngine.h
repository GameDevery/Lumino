
#pragma once

#include "Device/DeviceInterface.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>

namespace Lumino
{
namespace Graphics
{

template<typename T>
class CacheBuffer
{
public:
	CacheBuffer()
		: m_buffer()
		, m_capacity(0)
		, m_count(0)
	{
	}

	void Reserve(int count)
	{
		m_buffer.Resize(sizeof(T) * count, false);
		m_capacity = count;
	}

	void Add(const T& value)
	{
		if (m_count >= m_capacity) {
			Reserve(m_capacity * 2);
		}
		memcpy(&m_buffer[sizeof(T) * m_count], &value, sizeof(T));
		m_count++;
	}

	void Clear()
	{
		m_count = 0;
	}

	int GetCount() const { return m_count; }
	byte_t* GetBuffer() { return m_buffer.GetData(); }
	size_t GetBufferUsedByteCount() { return m_count * sizeof(T); }


private:
	ByteBuffer	m_buffer;
	int			m_capacity;
	int			m_count;
};

/// PainterEngine
///		���̃N���X�� Brush ���Q�Ƃ��Ȃ��悤�ɂ��邱�ƁB
class PainterEngine
	: public RefObject
{
public:
	PainterEngine();
	virtual ~PainterEngine();

public:
	void Create(GraphicsManager* manager);
	//void PushTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);
	void SetViewPixelSize(const Size& size);


	void DrawFillRectangle(const RectF& rect, Device::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode);

	// srcTexture �� NULL �Ȃ�_�~�[�e�N�X�`�����g����
	/// srcRect �̓T�C�Y�� INT_MAX �ł���ΑS�̂�]�����邱�Ƃ�����
	void DrawFrameRectangle(const RectF& rect, float frameWidth, Device::ITexture* srcTexture, const Rect& srcRect);

private:
	void InternalDrawRectangleStretch(const RectF& rect, const RectF& srcUVRect);
	void InternalDrawRectangleTiling(const RectF& rect, const Rect& srcRect, const RectF& srcUVRect, Device::ITexture* srcTexture);

private:
	struct PainterVertex
	{
	public:

		Vector3	Position;			///< �ʒu
		Vector4	Color;				///< ���_�J���[
		Vector4	UVOffset;		///< �e�N�X�`��UV (�]������UV���)
		Vector2	UVTileUnit;		///< �e�N�X�`��UV (�^�C�����O��Ԃ̂ǂ��ɂ��邩)

		/// ���_���C�A�E�g
		static Graphics::VertexElement* Elements()
		{
			static Graphics::VertexElement elements[] =
			{
				{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Position, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_Color, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 0 },
				{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 1 },
			};
			return elements;
		}
		static const int ElementCount = 4;
	};

	
	Device::IRenderer*				m_renderer;
	CacheBuffer<PainterVertex>		m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
	RefPtr<Device::ITexture>		m_dummyTexture;

	struct
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderTechnique*	Technique;
		Device::IShaderPass*		Pass;
		Device::IShaderVariable*	varWorldMatrix;
		Device::IShaderVariable*	varViewProjMatrix;
		Device::IShaderVariable*	varTexture;
		Device::IShaderVariable*	varViewportSize;	///< DX9 HLSL �p�̃s�N�Z���I�t�Z�b�g�v�Z�p�BGLSL �ł͍œK���ɂ������邱�Ƃ�����

	} m_shader;
};

} // namespace Graphics
} // namespace Lumino
