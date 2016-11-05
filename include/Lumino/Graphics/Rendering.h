
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Shader.h"
#include "Texture.h"

LN_NAMESPACE_BEGIN
class Material;
class StaticMeshModel;
class DrawList;

namespace detail {
class IRendererPloxy;
class PrimitiveRenderer;
class BlitRenderer;
class MeshRendererProxy;
class SpriteRenderer;
class TextRenderer;
class DrawElementBatch;
class RenderingPass2;

enum class DrawingSectionId
{
	None,
	Lines,
};

class CommandDataCache
{
public:
	using DataHandle = size_t;

	CommandDataCache();
	~CommandDataCache();

	void Reserve(size_t dataCount, size_t byteCount);
	void Clear();
	DataHandle AllocData(size_t byteCount);
	byte_t* GetData(DataHandle handle); 

	int GetDataCount() const { return m_dataList.GetCount(); }
	byte_t* GetDataByIndex(int index) { return GetData(m_dataList[index]); }

private:
	List<DataHandle>		m_dataList;
	ByteBuffer				m_dataBuffer;
	size_t					m_dataBufferUsed;
};

class InternalContext
	: public RefObject
{
public:
	InternalContext();
	void Initialize(detail::GraphicsManager* manager);
	Details::Renderer* GetRenderStateManager();
	Details::Renderer* BeginBaseRenderer();
	PrimitiveRenderer* BeginPrimitiveRenderer();
	BlitRenderer* BeginBlitRenderer();
	MeshRendererProxy* BeginMeshRenderer();
	SpriteRenderer* BeginSpriteRenderer();
	TextRenderer* BeginTextRenderer();

	void SetViewInfo(const SizeF& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix);
	void SetCurrentStatePtr(const DrawElementBatch* state);
	detail::SpriteRenderer* GetSpriteRenderer();

	void Flush();

private:
	void SwitchActiveRenderer(detail::IRendererPloxy* renderer);


	IRendererPloxy*				m_current;
	Details::Renderer*			m_baseRenderer;
	RefPtr<PrimitiveRenderer>	m_primitiveRenderer;
	RefPtr<BlitRenderer>		m_blitRenderer;
	RefPtr<MeshRendererProxy>	m_meshRenderer;
	RefPtr<SpriteRenderer>		m_spriteRenderer;
	RefPtr<TextRenderer>		m_textRenderer;
	const DrawElementBatch*		m_currentStatePtr;

	friend class DrawList;
};

class DrawElement
{
public:
	Matrix				transform;
	int					batchIndex;
	DrawingSectionId	drawingSectionId;
	detail::Sphere		boundingSphere;
	int					subsetIndex;

	DrawElement();
	virtual ~DrawElement();

	virtual void MakeElementInfo(const CameraInfo& cameraInfo, ElementInfo* outInfo);
	virtual void MakeSubsetInfo(Material* material, SubsetInfo* outInfo);

	//void Draw(InternalContext* context, RenderingPass2* pass);
	virtual void DrawSubset(InternalContext* context/*, int subsetIndex*/) = 0;
	const detail::Sphere& GetBoundingSphere() const { return boundingSphere; }

	// (���[�J�����W�n)
	void MakeBoundingSphere(const Vector3& minPos, const Vector3& maxPos);
};

class DrawElementBatch
{
public:
	static const int MaxMultiRenderTargets = 4;

	struct BuiltinMaterialParameter
	{
		//bool		alphaBlendEnabled;
		//BlendMode	blendMode;
		Color		blendColor;
		ToneF		tone;
	};

	//struct ShaderValuePair
	//{
	//	ShaderVariable*		variable;
	//	ShaderValue			value;
	//};

	DrawElementBatch();

	void SetRenderTarget(int index, RenderTarget* renderTarget);
	RenderTarget* GetRenderTarget(int index) const;
	void SetMaterial(Material* value);
	void SetStandaloneShaderRenderer(bool enabled);
	bool IsStandaloneShaderRenderer() const;

	void SetBrush(Brush* brush);
	Brush* GetBrush() const;
	void SetFont(RawFont* font);
	RawFont* GetFont() const;

	void SetBaseAlphaBlendEnabled(bool enabled);
	void SetBaseBlendMode(BlendMode mode);


	bool Equal(const DrawElementBatch& obj) const;
	void Reset();
	Shader* ApplyStatus(InternalContext* context, RenderTarget* defaultRenderTarget, DepthBuffer* defaultDepthBuffer, Shader* defaultShader);
	size_t GetHashCode() const;

	intptr_t				m_rendererId;

	// render state		TODO: �}�e���A���ɑ�����X�e�[�g�͕K�v�Ȃ�
	bool					m_baseAlphaBlendEnabled;
	BlendMode				m_baseBlendMode;
	CullingMode				m_cullingMode;
	bool					m_alphaTestEnabled;

	BuiltinMaterialParameter	m_builtinMaterialParameter;

	// depth stencil
	bool					m_depthTestEnabled;
	bool					m_depthWriteEnabled;

	// TODO: �K�p����ƂȂ����}�e���A���́A�`��I���܂� Freeze ����B�p�����[�^��ύX���Ă͂Ȃ�Ȃ��B
	// (�܂邱�҂���� Freeze �̕K�v�Ȃ����ǁA���ە`��ƃ}�e���A���ύX�̃^�C�~���O�͕����邾�낤)
	RefPtr<Material>		m_material;
	// shader	TODO: �T�u�Z�b�g�P�ʂŃX�e�[�g�ς�����悤�ɂ��������Ƃ����邯�ǁA����ϐ��l�����̂͂�����Ɩ��ʂȋC������
	//RefPtr<ShaderPass>		m_shaderPass;
	//List<ShaderValuePair>	m_shaderValueList;
	bool					m_standaloneShaderRenderer;

	// screen
	RefPtr<RenderTarget>	m_renderTargets[MaxMultiRenderTargets];
	RefPtr<DepthBuffer>		m_depthBuffer;
	Rect					m_scissorRect;

	// painting
	RefPtr<Brush>			m_brush;
	RefPtr<RawFont>			m_font;


private:


	mutable size_t			m_hashCode;
	mutable bool			m_hashDirty;
};

class BatchStateBlock
{
public:
	DrawElementBatch	state;
	Matrix				transfrom;

	void Reset()
	{
		state.Reset();
		transfrom = Matrix::Identity;
	}
};

class DrawElementList
{
public:
	typedef size_t	CommandHandle;

	DrawElementList();

	int GetElementCount() { return m_commandDataCache.GetDataCount(); }
	DrawElement* GetElement(int index) { return reinterpret_cast<DrawElement*>(m_commandDataCache.GetDataByIndex(index)); }
	DrawElementBatch* GetBatch(int index) { return &m_batchList[index]; }

	void ClearCommands();

	template<typename T, typename... TArgs>
	T* AddCommand(const DrawElementBatch& state, detail::IRendererPloxy* renderer, TArgs... args)
	{
		auto handle = m_commandDataCache.AllocData(sizeof(T));
		T* t = new (m_commandDataCache.GetData(handle))T(args...);
		PostAddCommandInternal(state, renderer, t);
		return t;
	}

	byte_t* AllocExtData(size_t size) { return m_extDataCache.GetData(m_extDataCache.AllocData(size)); }

private:
	void PostAddCommandInternal(const DrawElementBatch& state, detail::IRendererPloxy* renderer, DrawElement* element);

	CommandDataCache		m_commandDataCache;
	CommandDataCache		m_extDataCache;
	List<DrawElementBatch>	m_batchList;
};


class InternalRenderer
	: public RefObject
{
public:
	InternalRenderer();
	virtual ~InternalRenderer();
	void Initialize(GraphicsManager* manager);

	void Render(
		DrawElementList* elementList,
		const detail::CameraInfo& cameraInfo,
		RenderTarget* defaultRenderTarget,
		DepthBuffer* defaultDepthBuffer,
		RenderingPass2* pass);

private:
	GraphicsManager*	m_manager;
	List<DrawElement*>	m_renderingElementList;
};


class ScopedStateBlock2
{
public:
	ScopedStateBlock2(DrawList* renderer);
	~ScopedStateBlock2();
	void Apply();

private:
	DrawList*		m_renderer;
	BatchStateBlock	m_state;
};

//class DrawingSection
//{
//public:
//	virtual ~DrawingSection() = default;
//
//	DrawElementBatch*	stateInSection;
//};
//
//class DrawingSection_Line
//{
//public:
//	virtual ~DrawingSection_Line() = default;
//
//
//};


class RenderingPass2
	: public Object
{
public:
	RenderingPass2();
	virtual ~RenderingPass2();
	void Initialize(GraphicsManager* manager);

	Shader* GetDefaultShader() const;

	//virtual void RenderElement(DrawList* renderer, DrawElement* element);
	//virtual void RenderElementSubset(DrawList* renderer, DrawElement* element, int subsetIndex);

private:
	RefPtr<Shader>	m_defaultShader;
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
	void SetRenderTarget(int index, RenderTarget* renderTarget);

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


	//--------------------------------------------------------------------------
	/** @name Render status */
	/** @{ */

	void SetAlphaBlendEnabled(bool enabled);
	void SetBlendMode(BlendMode mode);

	/** @} */

	void SetTransform(const Matrix& transform);


	void SetBrush(Brush* brush);
	void SetFont(RawFont* font);

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);
	

	/**
		@brief		������`�悵�܂��B
		@param[in]	position1	: �n�_�̈ʒu
		@param[in]	color1		: �n�_�̐F
		@param[in]	position2	: �I�_�̈ʒu
		@param[in]	color2		: �I�_�̐F
	*/
	void DrawLinePrimitive(
		const Vector3& position1, const Color& color1,
		const Vector3& position2, const Color& color2);

	/**
		@brief
		@details	�f�t�H���g�ł͔����v��肪�\�ʂƂȂ�܂��B
	*/
	void DrawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,	// TODO: ����
		const Vector3& position4, const Vector2& uv4, const Color& color4/*,
		ShaderPass* shaderPass*/);

	void DrawMesh(StaticMeshModel* mesh, int subsetIndex, Material* material);

	void Blit(Texture* source);
	void Blit(Texture* source, const Matrix& transform);
	void Blit(Texture* source, RenderTarget* dest, const Matrix& transform);
	void Blit(Texture* source, RenderTarget* dest, Material* material);


	void DrawText_(const StringRef& text, const RectF& rect, StringFormatFlags flags);

	void DrawSprite(
		const Vector3& position,
		const SizeF& size,
		Texture* texture,
		const RectF& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection);

LN_INTERNAL_ACCESS:
	DrawList();
	virtual ~DrawList();
	void Initialize(detail::GraphicsManager* manager);
	detail::DrawElementList* GetDrawElementList() { return &m_drawElementList; }
	void BeginMakeElements();
	//void BeginFrame(RenderTarget* defaultRenderTarget, DepthBuffer* defaultDepthBuffer);
	void EndFrame();
	const detail::BatchStateBlock& GetState() const { return m_state; }
	void SetState(const detail::BatchStateBlock& state) { m_state = state; }
	template<typename TElement> TElement* ResolveDrawElement(detail::DrawingSectionId sectionId, detail::IRendererPloxy* renderer);
	void DrawMeshSubsetInternal(StaticMeshModel* mesh, int subsetIndex, Material* material);
	void BlitInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Material* material);

private:
	detail::GraphicsManager*		m_manager;
	detail::BatchStateBlock			m_state;
	detail::DrawElementList			m_drawElementList;

	detail::DrawElement*			m_currentSectionTopElement;
	//detail::DrawElementBatch		m_stateInSection;
};

LN_NAMESPACE_END

