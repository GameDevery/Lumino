
#pragma once
#include "Common.h"
#include "../Graphics/Painter.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class SceneGraphRenderingContext
	: public RenderingContext
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
