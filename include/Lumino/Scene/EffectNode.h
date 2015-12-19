
#pragma once
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Scene/VisualNode.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail { class EffectManager; }

/**
	@brief
*/
class EffectNode
	: public VisualNode	// TODO: �P�� SceneNode �̂ق��������C�����邪�E�E
{
public:

	

protected:
	EffectNode();
	virtual ~EffectNode();
	void Initialize(SceneGraphManager* manager, detail::EffectManager* effectManager);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
