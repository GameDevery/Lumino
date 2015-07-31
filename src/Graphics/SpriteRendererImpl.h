
#pragma once
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "Device/DeviceInterface.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{

class SpriteRendererImpl
    : public RefObject
{
public:
	SpriteRendererImpl(GraphicsManager* manager, int maxSpriteCount);
	virtual ~SpriteRendererImpl();

public:

	/// ���W�ϊ��s��̐ݒ�
    void SetTransform( const Matrix& matrix );

	/// �r���[�A�v���W�F�N�V�����s��̐ݒ� (view �̓r���{�[�h�̌v�Z�ŕK�v�ɂȂ�̂ŁAview proj �ŕ�����)
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	/// �r���[�T�C�Y�̐ݒ�
	void SetViewPixelSize(const Size& size);

	/// �����_�����O�X���[�g�̐ݒ�
	void SetRenderState(const RenderState& state);

	/// �\�[�g���@�̐ݒ�
	void SetSortMode(uint32_t flags, SortingDistanceBasis basis);

	/// �`�惊�N�G�X�g
    void DrawRequest2D(
        const Vector3& position,
		const Vector3& center,
		const Vector2& size,
        Device::ITexture* texture,
		const RectF& srcRect,
		const ColorF* colorTable);

	/// �`�惊�N�G�X�g
	void DrawRequest3D(
        const Vector3& position,
        const Vector3& center,
        const Vector2& size,
		Device::ITexture* texture,
		const RectF& srcRect,
		const ColorF* colorTable,    // 4 ���_���BNULL �̏ꍇ�͔�
		AxisDirection front);

	/// �o�b�`��������Ă���X�v���C�g�̕`��
	void Flash();

	/// �`�悹���Ƀo�b�`�����ׂăN���A����
	void Clear();

	GraphicsManager* GetManager() const { return m_manager; }

private:

	/// �`�惊�N�G�X�g
	void DrawRequest3DInternal(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Device::ITexture* texture,
		const RectF& srcRect,
		const ColorF* colorTable,
		AxisDirection front,
		bool is3D);

	/// �o�b�`�����p�X�v���C�g���_
	struct BatchSpriteVertex
	{
		Vector3		Position;   ///< ���W
		ColorF		Color;      ///< ���_�J���[
		Vector2		TexUV;      ///< �e�N�X�`�����W

		/// ���_���C�A�E�g
		static Graphics::VertexElement* Elements()
		{
			static Graphics::VertexElement elements[] =
			{
				{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Position, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_Color, 0 },
				{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	struct BatchSpriteData
	{
		BatchSpriteVertex	Vertices[4];  ///< ���W�ϊ��ςݒ��_ ([0].Position.z ���\�[�g�̊�ɂȂ�)
		Device::ITexture*	Texture;
		int					RenderStateIndex;
		int					Priority;       ///< �D��x (�傫�������ォ��`�悳��� =��O)
		float				Depth;          ///< �\�[�g�Ɏg���� Z �l (�傫���قǉ���)
		bool				Visible;
		bool				Assigned;       ///< SpriteRenderer::getBatchSpriteData() �Ŏ擾���ꂽ�ꍇ�� true �ɂȂ�

	};

    struct Attribute
    {
        int StartIndex;
        int PrimitiveNum;
		Device::ITexture*   Texture;
		int RenderStateIndex;
    };

	class SpriteCmpDepthBackToFront;		///< Z �l�̑傫�������珬�������փ\�[�g�����r
	class SpriteCmpDepthFrontToBack;		///< Z �l�̏�����������傫�����փ\�[�g�����r
	class SpriteCmpTexDepthBackToFront;		///< Z �l�̑傫�������珬�������փ\�[�g�����r (�e�N�X�`���D��)
	class SpriteCmpTexDepthFrontToBack;		///< Z �l�̏�����������傫�����փ\�[�g�����r (�e�N�X�`���D��)

private:
	typedef Array<BatchSpriteData>	SpriteRequestList;
	typedef Array<RenderState>		RenderStateList;
	typedef Array<Attribute>		AttributeList;

	RefPtr<GraphicsManager>			m_manager;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
    int								m_maxSprites;
	SpriteRequestList				m_spriteRequestList;
	int								m_spriteRequestListUsedCount;
	Array<int>						m_spriteIndexList;			///< Flash() ���Ŏg�p����Bm_spriteRequestList ���\�[�g����͍̂\���̃R�s�[�𔺂����ߑ��x�ʂŐS�z�B�Ȃ̂ŃC���f�b�N�X���\�[�g����B
	RenderStateList					m_renderStateList;
	int								m_currentRenderStateIndex;	///< ���� Draw �łɓK�p����� RenderState ������ m_renderStateList ���̃C���f�b�N�X
	AttributeList					m_attributeList;

	Matrix							m_transformMatrix;
	Vector3							m_viewDirection;			///< �r���{�[�h�̌v�Z�Ɏg�p����
	Matrix							m_viewInverseMatrix;		///< �r���{�[�h�̌v�Z�Ɏg�p����
	Vector3							m_viewPosition;				///< �J�����ʒu����Ƃ����\�[�g�Ŏg�p����
	Matrix							m_viewProjMatrix;
	Vector2							m_viewPixelSize;

	uint32_t						m_spriteSortMode;			///< �\�[�g���@ (SpriteSortMode)
	SortingDistanceBasis			m_sortingBasis;				///< �\�[�g�
   
	/// �V�F�[�_�֌W�̕ϐ����܂Ƃ߂��\����
    struct 
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderVariable*    varViewProjMatrix;
		Device::IShaderVariable*    varViewPixelSize;
		Device::IShaderVariable*    varTexture;
		Device::IShaderTechnique*   techMainDraw;

    } m_shader;

public:

	struct SetTransformCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Matrix	m_transform;

		void Create(SpriteRendererImpl* renderer, const Matrix& transform)
		{
			m_renderer = renderer;
			m_transform = transform;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetTransform(m_transform); }
	};

	struct SetViewProjMatrixCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Matrix	m_view;
		Matrix	m_proj;

		void Create(SpriteRendererImpl* renderer, const Matrix& view, const Matrix& proj)
		{
			m_renderer = renderer;
			m_view = view;
			m_proj = proj;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetViewProjMatrix(m_view, m_proj); }
	};

	struct SetViewPixelSizeCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Size	m_size;

		void Create(SpriteRendererImpl* renderer, const Size& size)
		{
			m_renderer = renderer;
			m_size = size;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetViewPixelSize(m_size); }
	};

	struct SetRenderStateCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		RenderState	m_state;

		void Create(SpriteRendererImpl* renderer, const RenderState& state)
		{
			m_renderer = renderer;
			m_state = state;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetRenderState(m_state); }
	};

	struct SetSortModeCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		uint32_t				m_flags;
		SortingDistanceBasis	m_basis;

		void Create(SpriteRendererImpl* renderer, uint32_t flags, SortingDistanceBasis basis)
		{
			m_renderer = renderer;
			m_flags = flags;
			m_basis = basis;
			MarkGC(m_renderer);
		}
		void Execute()
		{
			m_renderer->SetSortMode(m_flags, m_basis);
		}
	};

	struct DrawRequest2DCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Device::ITexture* m_texture;
		RectF m_srcRect;
		ColorF m_colorTable[4];

		void Create(
			SpriteRendererImpl* renderer, 
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Device::ITexture* texture,
			const RectF& srcRect,
			const ColorF* colorTable)
		{
			m_renderer = renderer;
			m_position = position;
			m_center = center;
			m_size = size;
			m_texture = texture;
			m_srcRect = srcRect;
			memcpy(m_colorTable, colorTable, sizeof(ColorF) * 4);
			MarkGC(m_renderer);
			MarkGC(m_texture);
		}
		void Execute()
		{
			m_renderer->DrawRequest2D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable);
		}
	};

	struct DrawRequest3DCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Device::ITexture* m_texture;
		RectF m_srcRect;
		ColorF m_colorTable[4];
		AxisDirection m_front;

		void Create(
			SpriteRendererImpl* renderer,
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Device::ITexture* texture,
			const RectF& srcRect,
			const ColorF* colorTable,
			AxisDirection front)
		{
			m_renderer = renderer;
			m_position = position;
			m_center = center;
			m_size = size;
			m_texture = texture;
			m_srcRect = srcRect;
			memcpy(m_colorTable, colorTable, sizeof(ColorF) * 4);
			m_front = front;
			MarkGC(m_renderer);
			MarkGC(m_texture);
		}
		void Execute()
		{
			m_renderer->DrawRequest3D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable, m_front);
		}
	};

	struct FlashCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;

		void Create(SpriteRendererImpl* renderer)
		{
			m_renderer = renderer;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->Flash(); }
	};

	struct ClearCommand : public RenderingCommand
	{
		SpriteRendererImpl*	m_renderer;

		void Create(SpriteRendererImpl* renderer)
		{
			m_renderer = renderer;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->Clear(); }
	};
};

} // namespace Graphics
} // namespace Lumino
