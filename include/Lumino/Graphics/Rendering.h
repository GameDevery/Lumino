
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Shader.h"
#include "Texture.h"

LN_NAMESPACE_BEGIN
class DrawList;

namespace detail {
class IRendererPloxy;
class SpriteRenderer;

class InternalContext
{
public:
	InternalContext();
	void Initialize(detail::GraphicsManager* manager);
	Details::Renderer* BeginBaseRenderer();
	detail::SpriteRenderer* BeginSpriteRenderer();

private:
	void SwitchActiveRenderer(detail::IRendererPloxy* renderer);

	detail::IRendererPloxy*			m_current;
	Details::Renderer*				m_baseRenderer;
	RefPtr<detail::SpriteRenderer>	m_spriteRenderer;

	friend class DrawList;
};

class DrawElement
{
public:
	Matrix	transform;
	int		batchIndex;

	DrawElement();
	virtual ~DrawElement();

	virtual void Execute(InternalContext* context) = 0;
};

class DrawElementBatch
{
public:
	static const int MaxMultiRenderTargets = 4;

	struct ShaderValuePair
	{
		ShaderVariable*		variable;
		ShaderValue			value;
	};

	bool Equal(const DrawElementBatch& obj) const;

	intptr_t				m_rendererId;

	// render state
	bool					m_alphaBlendEnabled;
	BlendMode				m_blendMode;
	CullingMode				m_cullingMode;
	bool					m_alphaTestEnabled;

	// depth stencil
	bool					m_depthTestEnabled;
	bool					m_depthWriteEnabled;

	// shader
	RefPtr<Shader>			m_shader;
	List<ShaderValuePair>	m_shaderValueList;

	// screen
	RefPtr<RenderTarget>	m_renderTargets[MaxMultiRenderTargets];
	RefPtr<DepthBuffer>		m_depthBuffer;
	Rect					m_scissorRect;
};

class BatchStateBlock
{
public:
	DrawElementBatch	state;
	Matrix				transfrom;
};

class DrawElementList
{
public:
	typedef size_t	CommandHandle;

	DrawElementList();

	detail::DrawElement* GetCommand(CommandHandle handle);

	void ClearCommands();

	template<typename T, typename... TArgs>
	T* AddCommand(const DrawElementBatch& state, detail::IRendererPloxy* renderer, TArgs... args)
	{
		CommandHandle handle = AllocCommand(sizeof(T));
		T* t = new (GetCommand(handle))T(args...);
		m_commandList.Add(handle);
		PostAddCommandInternal(state, renderer, t);
		return t;
	}

private:
	CommandHandle AllocCommand(size_t byteCount);
	void PostAddCommandInternal(const DrawElementBatch& state, detail::IRendererPloxy* renderer, DrawElement* element);

	List<size_t>					m_commandList;
	ByteBuffer						m_commandDataBuffer;
	size_t							m_commandDataBufferUsed;
	List<detail::DrawElementBatch>	m_batchList;
};

} // namespace detail

/**
	@brief	
*/
class DrawList	// TODO: Renderer
	: public Object
{
public:

	//--------------------------------------------------------------------------
	/** @name Render targets */
	/** @{ */

	/** �����_�����O�^�[�Q�b�g��ݒ肵�܂��B*/
	void SetRenderTarget(int index, RenderTarget* texture);

	/** ���ݐݒ肳��Ă��郌���_�����O�^�[�Q�b�g���擾���܂��B*/
	RenderTarget* GetRenderTarget(int index) const;

	/** �[�x�o�b�t�@��ݒ肵�܂��B*/
	void SetDepthBuffer(DepthBuffer* depthBuffer);

	/** ���ݐݒ肳��Ă���[�x�o�b�t�@���擾���܂��B*/
	DepthBuffer* GetDepthBuffer() const;

	/** �V�U�[�̈��ݒ肵�܂��B*/
	void SetViewport(const Rect& rect);

	/** ���ݐݒ肳��Ă���V�U�[�̈���擾���܂��B*/
	const Rect& GetViewport() const;

	/** @} */

	void SetTransform(const Matrix& transform);

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);

	void DrawSprite2D(
		const SizeF& size,
		Texture* texture,
		const RectF& srcRect,
		const Color& color);

LN_INTERNAL_ACCESS:
	DrawList();
	virtual ~DrawList();
	void Initialize(detail::GraphicsManager* manager);

private:
	detail::InternalContext			m_internalContext;
	detail::BatchStateBlock			m_state;
	detail::DrawElementList			m_drawElementList;
};

LN_NAMESPACE_END

