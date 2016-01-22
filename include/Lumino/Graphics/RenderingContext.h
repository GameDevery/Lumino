
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Color.h"

LN_NAMESPACE_BEGIN
class SpriteRenderer;
namespace detail { class PrimitiveRenderer; }

/**
	@brief		
*/
class RenderingContext2
	: public RefObject
	, public detail::IContext
{
public:
	static const int MaxMultiRenderTargets = 4;

	// TODO: Scene�Ȃǂœ����I�Ɏg�p������̂Ƌ��p�ɂ��Ȃ��B�X�e�[�g���ǂ��ŕς�邩�킩��Â炢�B���邢�͂����� push/pop ����B
	static RenderingContext2* GetContext();

	/**
		@brief	�����_�����O�X�e�[�g��ݒ肵�܂��B
	*/
	void SetRenderState(const RenderState& state);

	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�X�e�[�g���擾���܂��B
	*/
	const RenderState& GetRenderState() const;

	/**
		@brief	�[�x�e�X�g�y�уX�e���V���e�X�g�X�e�[�g��ݒ肵�܂��B
	*/
	void SetDepthStencilState(const DepthStencilState& state);
	
	/**
		@brief	�[�x�e�X�g�y�уX�e���V���e�X�g�X�e�[�g���擾���܂��B
	*/
	const DepthStencilState& GetDepthStencilState() const;

	/**
		@brief	�����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	*/
	void SetRenderTarget(int index, Texture* texture);

	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�^�[�Q�b�g���擾���܂��B
	*/
	Texture* GetRenderTarget(int index) const;

	/**
		@brief	�[�x�o�b�t�@��ݒ肵�܂��B
	*/
	void SetDepthBuffer(Texture* depthBuffer);

	/**
		@brief	���ݐݒ肳��Ă���[�x�o�b�t�@���擾���܂��B
	*/
	Texture* GetDepthBuffer() const;

	/**
		@brief	�r���[�|�[�g��ݒ肵�܂��B
	*/
	void SetViewport(const Rect& rect);

	/**
		@brief	���ݐݒ肳��Ă���r���[�|�[�g���擾���܂��B
	*/
	const Rect& GetViewport() const;

	/**
		@brief	���_�o�b�t�@��ݒ肵�܂��B
	*/
	void SetVertexBuffer(VertexBuffer* vertexBuffer);

	/**
		@brief	�C���f�b�N�X�o�b�t�@��ݒ肵�܂��B
	*/
	void SetIndexBuffer(IndexBuffer* indexBuffer);

	void SetShaderPass(ShaderPass* pass);

	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�^�[�Q�b�g�A�[�x�o�b�t�@���N���A���܂��B
	*/
	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief	���ݐݒ肳��Ă��钸�_�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	���ݐݒ肳��Ă��钸�_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	// pass �� null �̂Ƃ��Ɏg����f�t�H���g�V�F�[�_�p
	void SetViewProjection(const Matrix& view, const Matrix& proj);

	void DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const ColorF& color1,
		const Vector3& position2, const Vector2& uv2, const ColorF& color2,
		const Vector3& position3, const Vector2& uv3, const ColorF& color3,
		const Vector3& position4, const Vector2& uv4, const ColorF& color4);

	void Flush();

LN_INTERNAL_ACCESS:
	RenderingContext2();
	virtual ~RenderingContext2();
	void Initialize(GraphicsManager* manager);
	void CheckFlushRendererState();
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;

private:
	GraphicsManager*			m_manager;
	Details::Renderer*			m_ploxy;
	detail::ContextState		m_state;

	detail::IRendererPloxy*		m_activeRendererPloxy;
	SpriteRenderer*				m_spriteRenderer;
	detail::PrimitiveRenderer*	m_primitiveRenderer;

	//bool						m_stateModified;
};

LN_NAMESPACE_END
