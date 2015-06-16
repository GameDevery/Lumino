
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
				{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 1 },
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
	typedef ArrayList<BatchSpriteData>	SpriteRequestList;
	typedef ArrayList<RenderState>		RenderStateList;
	typedef ArrayList<Attribute>		AttributeList;

	RefPtr<GraphicsManager>			m_manager;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
    int								m_maxSprites;
	SpriteRequestList				m_spriteRequestList;
	int								m_spriteRequestListUsedCount;
	ArrayList<int>					m_spriteIndexList;			///< Flash() ���Ŏg�p����Bm_spriteRequestList ���\�[�g����͍̂\���̃R�s�[�𔺂����ߑ��x�ʂŐS�z�B�Ȃ̂ŃC���f�b�N�X���\�[�g����B
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
	class SpriteRendererCommand : public RenderingCommand
	{
	protected:
		SpriteRendererImpl*	m_renderer;
		
		virtual ~SpriteRendererCommand()
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class SetTransformCommand : public SpriteRendererCommand
	{
		Matrix	m_transform;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const Matrix& transform)
		{
			HandleCast<SetTransformCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetTransformCommand>(cmd)->m_transform = transform;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetTransform(m_transform);
		}
	};

	class SetViewProjMatrixCommand : public SpriteRendererCommand
	{
		Matrix	m_view;
		Matrix	m_proj;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const Matrix& view, const Matrix& proj)
		{
			HandleCast<SetViewProjMatrixCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetViewProjMatrixCommand>(cmd)->m_view = view;
			HandleCast<SetViewProjMatrixCommand>(cmd)->m_proj = proj;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetViewProjMatrix(m_view, m_proj);
		}
	};

	class SetViewPixelSizeCommand : public SpriteRendererCommand
	{
		Size	m_size;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const Size& size)
		{
			HandleCast<SetViewPixelSizeCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetViewPixelSizeCommand>(cmd)->m_size = size;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetViewPixelSize(m_size);
		}
	};

	class SetRenderStateCommand : public SpriteRendererCommand
	{
		RenderState	m_state;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, const RenderState& state)
		{
			HandleCast<SetRenderStateCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetRenderStateCommand>(cmd)->m_state = state;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetRenderState(m_state);
		}
	};

	class SetSortModeCommand : public SpriteRendererCommand
	{
		uint32_t				m_flags;
		SortingDistanceBasis	m_basis;
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer, uint32_t flags, SortingDistanceBasis basis)
		{
			HandleCast<SetSortModeCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetSortModeCommand>(cmd)->m_flags = flags;
			HandleCast<SetSortModeCommand>(cmd)->m_basis = basis;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetSortMode(m_flags, m_basis);
		}
	};

	class DrawRequest2DCommand : public SpriteRendererCommand
	{
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Device::ITexture* m_texture;
		RectF m_srcRect;
		ColorF m_colorTable[4];
	public:
		static void Create(
			CmdInfo& cmd,
			SpriteRendererImpl* renderer, 
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Device::ITexture* texture,
			const RectF& srcRect,
			const ColorF* colorTable)
		{
			HandleCast<DrawRequest2DCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawRequest2DCommand>(cmd)->m_position = position;
			HandleCast<DrawRequest2DCommand>(cmd)->m_center = center;
			HandleCast<DrawRequest2DCommand>(cmd)->m_size = size;
			HandleCast<DrawRequest2DCommand>(cmd)->m_texture = texture;
			HandleCast<DrawRequest2DCommand>(cmd)->m_srcRect = srcRect;
			memcpy(HandleCast<DrawRequest2DCommand>(cmd)->m_colorTable, colorTable, sizeof(ColorF));
			LN_SAFE_ADDREF(renderer);
			LN_SAFE_ADDREF(texture);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawRequest2D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable);
		}
		virtual ~DrawRequest2DCommand()
		{
			LN_SAFE_RELEASE(m_texture);
		}
	};

	class DrawRequest3DCommand : public SpriteRendererCommand
	{
		Vector3 m_position;
		Vector3 m_center;
		Vector2 m_size;
		Device::ITexture* m_texture;
		RectF m_srcRect;
		ColorF m_colorTable[4];
		AxisDirection m_front;
	public:
		static void Create(
			CmdInfo& cmd,
			SpriteRendererImpl* renderer,
			const Vector3& position,
			const Vector3& center,
			const Vector2& size,
			Device::ITexture* texture,
			const RectF& srcRect,
			const ColorF* colorTable,
			AxisDirection front)
		{
			HandleCast<DrawRequest3DCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawRequest3DCommand>(cmd)->m_position = position;
			HandleCast<DrawRequest3DCommand>(cmd)->m_center = center;
			HandleCast<DrawRequest3DCommand>(cmd)->m_size = size;
			HandleCast<DrawRequest3DCommand>(cmd)->m_texture = texture;
			HandleCast<DrawRequest3DCommand>(cmd)->m_srcRect = srcRect;
			memcpy(HandleCast<DrawRequest3DCommand>(cmd)->m_colorTable, colorTable, sizeof(ColorF));
			HandleCast<DrawRequest3DCommand>(cmd)->m_front = front;
			LN_SAFE_ADDREF(renderer);
			LN_SAFE_ADDREF(texture);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawRequest3D(m_position, m_center, m_size, m_texture, m_srcRect, m_colorTable, m_front);
		}
		virtual ~DrawRequest3DCommand()
		{
			LN_SAFE_RELEASE(m_texture);
		}
	};

	class FlashCommand : public SpriteRendererCommand
	{
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer)
		{
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->Flash();
		}
	};

	class ClearCommand : public SpriteRendererCommand
	{
	public:
		static void Create(CmdInfo& cmd, SpriteRendererImpl* renderer)
		{
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->Clear();
		}
	};
};

} // namespace Graphics
} // namespace Lumino
