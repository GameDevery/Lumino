
#pragma once
#include "Common.h"
#include "../Graphics/DrawingContext.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class SceneGraphRenderingContext
	: public DrawingContext
{
LN_INTERNAL_ACCESS:
	RenderingPass*				Pass;
	Details::Renderer*			Renderer;	// TODO �����N���X
	GeometryRenderer*			GeometryRenderer;
	Camera*						CurrentCamera;
	MMEShader*					Shader;				// �{���ɕK�v�ȃV�F�[�_ (VisualNode::Render() �ȉ��Ŏg�p�\)

protected:
	friend class SceneGraphManager;
	SceneGraphRenderingContext(GraphicsManager* manager);
	virtual ~SceneGraphRenderingContext();
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
