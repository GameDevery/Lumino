
#pragma once

#include "RenderingPass.h"

namespace Lumino
{
namespace Scene
{

/// �������Z�̃f�o�b�O�p�`��Ȃ�
class InfomationRenderingPass
	: public RenderingPass
{
public:
	InfomationRenderingPass(SceneGraphManager* manager);
	virtual ~InfomationRenderingPass();

public:
	virtual void PostRender(RenderingParams& params);
};

} // namespace Scene
} // namespace Lumino
