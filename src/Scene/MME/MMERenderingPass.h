
#pragma once

#include "../RenderingPass.h"

namespace Lumino
{
namespace Scene
{

/// MMDPass ���L�[�Ƃ��ăV�F�[�_�e�N�j�b�N���������� RenderingPass
class MMERenderingPass
	: public RenderingPass
{
public:
	MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, MMEShader* ownerShader = NULL);
	virtual ~MMERenderingPass();

private:
	virtual void RenderNode(RenderingParams& params, SceneNode* node);

private:
	/// ���̃p�X�̃f�t�H���g&�D��V�F�[�_�ƁA�w�肵���m�[�h�̃V�F�[�_����`��Ɏg�p����e�N�j�b�N��I������B
	void SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams);

private:
	MMDPass		m_mmdPass;
	MMEShader*	m_ownerShader;			///< ���̃p�X (OFFSCREENRENDERTARGET) �̐������ƂȂ����V�F�[�_
};

} // namespace Scene
} // namespace Lumino
