
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
	Camera*						CurrentCamera;
	MMEShader*					Shader;				// �{���ɕK�v�ȃV�F�[�_ (VisualNode::Render() �ȉ��Ŏg�p�\)

public:
	DrawingContext* BeginDrawingContext();
	SpriteRenderer* BeginSpriteRendering();
	GraphicsContext* BeginGraphicsContext() { return m_internalContext; }
	void Flush();

	Details::Renderer* GetRenderer() { return m_internalContext->Renderer; }

	// view proj �� �p�X�J�n���ɃJ�����̂��̂��ݒ肳��Ă���
	RenderingContext2* GetRenderingContext() { return m_renderingContext; }

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(RenderingContext2* renderingContext, GraphicsContext* internalContext);
	virtual ~SceneGraphRenderingContext();

private:
	RenderingContext2*	m_renderingContext;	// TODO: �����܂����͂��������l�����ق�����������
	GraphicsContext*	m_internalContext;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
