﻿
#pragma once
#include "Common.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class GraphicsManager;

namespace detail
{
class RenderTargetTextureCache;

LN_ENUM_FLAGS(ContextStateFlags)
{
	None = 0x00,
	CommonState = 0x01,
	ShaderPass = 0x02,		// これ以外のステートに比べて割と頻繁に変更されるので個別に分ける
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

	// TODO: あとで IContex は GraphicsContext と統合する。なのでこれらはこのままここに置いておく
	Matrix					worldTransform;
	Matrix					viewTransform;
	Matrix					projectionTransform;


	ContextStateFlags		modifiedFlags = ContextStateFlags::None;

	ContextState();
	~ContextState();


	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	void SetFillBrush(Brush* brush);
	Brush* GetFillBrush() const { return m_fillBrush; }

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
	Brush*		m_fillBrush;
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

} // namespace detail


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

