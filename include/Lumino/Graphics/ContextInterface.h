
#pragma once
#include "Common.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

namespace detail
{
class RenderTargetTextureCache;

LN_ENUM_FLAGS(ContextStateFlags)
{
	None = 0x00,
	CommonState = 0x01,
	ShaderPass = 0x02,		// ����ȊO�̃X�e�[�g�ɔ�ׂĊ��ƕp�ɂɕύX�����̂Ōʂɕ�����
	All = 0xFFFF,
};
LN_ENUM_FLAGS_DECLARE(ContextStateFlags);

struct ContextState
{


	static const int MaxMultiRenderTargets = 4;	// TODO

	RenderState				renderState;
	DepthStencilState		depthStencilState;
	Texture*				depthBuffer = nullptr;
	Rect					viewport;
	//VertexBuffer*			vertexBuffer = nullptr;
	//IndexBuffer*			indexBuffer = nullptr;

	ContextStateFlags		modifiedFlags = ContextStateFlags::None;

	~ContextState();


	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	void Reset()
	{
		renderState = RenderState();
		depthStencilState = DepthStencilState();
		depthBuffer = nullptr;
		viewport = Rect();
		//vertexBuffer = nullptr;
		//indexBuffer = nullptr;
	}

private:
	std::array<Texture*, MaxMultiRenderTargets>	m_renderTargets = {};
	Shader*		m_ownerShader = nullptr;
	ShaderPass*	m_shaderPass = nullptr;
};


class IRendererPloxy// : public RefObject
{
public:
	virtual void Flush() = 0;
	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};

class HiLevelRendererCore : public RefObject
{
public:

	void ActivateFront(IRendererPloxy* renderer)
	{
		if (renderer != m_rendererFront)
		{
			if (m_rendererFront != nullptr)
			{
				m_rendererFront->Flush();
			}
			m_rendererFront = renderer;
		}
	}

private:
	IRendererPloxy*	m_rendererFront = nullptr;
};




class IContext
{
public:
	
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

	///**
	//	@brief	���_�o�b�t�@��ݒ肵�܂��B
	//*/
	//void SetVertexBuffer(VertexBuffer* vertexBuffer);

	///**
	//	@brief	�C���f�b�N�X�o�b�t�@��ݒ肵�܂��B
	//*/
	//void SetIndexBuffer(IndexBuffer* indexBuffer);

	void SetShaderPass(ShaderPass* pass);

	ShaderPass* GetShaderPass() const;

protected:
	IContext();
	virtual ~IContext();
	void Initialize(GraphicsManager* manager);
	const detail::ContextState& GetContextState() const { return m_state; }

	virtual void OnStateFlushRequested() = 0;
	virtual void OnPrimitiveFlushRequested();

	friend class ShaderVariable;
	void OnStateChanging();
	void OnDrawing(detail::IRendererPloxy* rendererPloxy);

	friend class GraphicsManager;
	virtual void OnActivated();
	virtual void OnDeactivated();

//LN_INTERNAL_ACCESS:
//	virtual ShaderPass* GetShaderPass() const = 0;

private:
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);

	GraphicsManager*		m_manager;
	detail::ContextState	m_state;
	detail::IRendererPloxy*	m_activeRendererPloxy;
};

} // namespace detail


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

