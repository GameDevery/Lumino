
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Color.h"
#include "ContextInterface.h"

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

	//void SetBlendMode(BlendMode mode);



	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�^�[�Q�b�g�A�[�x�o�b�t�@���N���A���܂��B
	*/
	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief	���ݐݒ肳��Ă��钸�_�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitive(VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	���ݐݒ肳��Ă��钸�_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitiveIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	// pass �� null �̂Ƃ��Ɏg����f�t�H���g�V�F�[�_�p
	void SetViewProjection(const Matrix& view, const Matrix& proj);

	void DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const ColorF& color1,
		const Vector3& position2, const Vector2& uv2, const ColorF& color2,
		const Vector3& position3, const Vector2& uv3, const ColorF& color3,
		const Vector3& position4, const Vector2& uv4, const ColorF& color4);


	// �X�e�[�g�Œ�B
	void Blt(Texture* source, RenderTarget* dest);
	void Blt(Texture* source, RenderTarget* dest, Shader* shader);


	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }

LN_INTERNAL_ACCESS:
	RenderingContext2();
	virtual ~RenderingContext2();
	void Initialize(GraphicsManager* manager);




	virtual void OnStateFlushRequested();

private:
	GraphicsManager*			m_manager;

	SpriteRenderer*				m_spriteRenderer;
	detail::PrimitiveRenderer*	m_primitiveRenderer;

	//bool						m_stateModified;
};

LN_NAMESPACE_END
