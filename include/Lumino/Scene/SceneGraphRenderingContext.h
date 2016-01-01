
#pragma once
#include "Common.h"
#include "../Graphics/DrawingContext.h"

LN_NAMESPACE_BEGIN
class SpriteRenderer;
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class SceneGraphRenderingContext
	//: public GraphicsContext
{
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	//Details::Renderer*			Renderer;	// TODO �����N���X
	//GeometryRenderer*			GeometryRenderer;
	Camera*						CurrentCamera;
	MMEShader*					Shader;				// �{���ɕK�v�ȃV�F�[�_ (VisualNode::Render() �ȉ��Ŏg�p�\)

public:
	DrawingContext* BeginDrawingContext();
	SpriteRenderer* BeginSpriteRendering();
	void Flush();

	Details::Renderer* GetRenderer() { return m_internalContext->Renderer; }
	GeometryRenderer* GetGeometryRenderer() { return m_internalContext->GeometryRenderer; }	// TODO: ���Ƃŏ����Ǝv��

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(GraphicsContext* internalContext);
	virtual ~SceneGraphRenderingContext();

private:
	GraphicsContext*	m_internalContext;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
