
#pragma once
#include <stack>
#include "Common.h"
#include "ContextInterface.h"

LN_NAMESPACE_BEGIN
class Material;
class MeshResource;
class StaticMeshModel;
namespace detail { class PrimitiveRenderer; }
namespace detail { class MeshRendererProxy; }

class RenderingContext
	: public detail::ContextInterface
{
public:


	//--------------------------------------------------------------------------
	/** @name Render status. */
	/** @{ */
	
	/**
		@brief	�����_�����O�X�e�[�g��ݒ肵�܂��B
	*/
	void SetRenderState(const RenderState& state);

	/** �A���t�@�u�����h�̗L�� (default: false) */
	void SetAlphaBlendEnabled(bool enabled);
	/** �u�����f�B���O�̉��Z���@ (default: Add) */
	void SetBlendOp(BlendOp op);
	/** �u�����f�B���O�̌W�� (default: One) */
	void SetSourceBlend(BlendFactor blend);
	/** �u�����f�B���O�̌W�� (default: Zero) */
	void SetDestinationBlend(BlendFactor blend);


	void SetBlendMode(BlendMode mode);
	void SetCullingMode(CullingMode mode);
	void SetFillMode(FillMode mode);
	void SetAlphaTestEnabled(bool enabled);


	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�X�e�[�g���擾���܂��B
	*/
	const RenderState& GetRenderState() const;

	/**
		@brief	�[�x�e�X�g�y�уX�e���V���e�X�g�X�e�[�g��ݒ肵�܂��B
	*/
	void SetDepthStencilState(const DepthStencilState& state);

	void SetDepthTestEnabled(bool enabled);
	void SetDepthWriteEnabled(bool enabled);
	void SetDepthTestFunc(CompareFunc func);
	void SetStencilEnabled(bool enabled);
	void SetStencilFunc(CompareFunc func);
	void SetStencilReferenceValue(uint8_t value);
	void SetStencilFailOp(StencilOp op);
	void SetStencilDepthFailOp(StencilOp op);
	void SetStencilPassOp(StencilOp op);
	

	void ResetStates();


	

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
	void SetViewport(const RectI& rect);

	/**
		@brief	���ݐݒ肳��Ă���r���[�|�[�g���擾���܂��B
	*/
	const RectI& GetViewport() const;









	void SetShaderPass(ShaderPass* pass);

	ShaderPass* GetShaderPass() const;




	void PushState();
	void PopState();

	/** @} */

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);


	

	/**
		@brief	���_�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	void DrawLinePrimitive(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);

	void DrawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,
		const Vector3& position4, const Vector2& uv4, const Color& color4);

	void Flush();


	// �X�e�[�g�Œ�B
	void Blt(Texture* source, RenderTarget* dest);
	void Blt(Texture* source, RenderTarget* dest, const Matrix& transform);
	void Blt(Texture* source, RenderTarget* dest, Shader* shader);


LN_INTERNAL_ACCESS:
	RenderingContext();
	virtual ~RenderingContext();
	void Initialize(detail::GraphicsManager* manager);
	void InitializeFrame(RenderTarget* renderTarget);
	void DrawMesh(MeshResource* mesh, int startIndex, int triangleCount);
	void DrawMesh(MeshResource* mesh, int subset);
	void DrawMesh(StaticMeshModel* mesh, int subset);
	void DrawMesh(MeshResource* mesh, int startIndex, int triangleCount, Material* material);
	void DrawMesh(StaticMeshModel* mesh, int startIndex, int triangleCount, Material* material);
	void InheritStatus(RenderingContext* parent);

	detail::PrimitiveRenderer* GetPrimitiveRenderer() const { return m_primitiveRenderer; }	// TODO: test
	const detail::BasicContextState& GetCurrentContxtState() const { return m_state; }

protected:
	virtual bool OnCheckStateChanged() override;
	virtual void OnStateFlush(detail::IRendererPloxy* activeRenderer) override;
	virtual void OnPrimitiveFlush() override;
	virtual void OnShaderVariableModified(ShaderVariable* var) override;

private:
	void BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader);

	class RenderStateBlock2;

	detail::PrimitiveRenderer*				m_primitiveRenderer;
	detail::MeshRendererProxy*				m_meshRendererProxy;
	detail::BasicContextState				m_state;
	detail::BasicContextState				m_backendState;
	std::stack<detail::BasicContextState>	m_stateStack;
};

LN_NAMESPACE_END
