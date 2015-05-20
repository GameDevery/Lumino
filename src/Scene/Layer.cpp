
#pragma once

#include "../Internal.h"
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Layer.h"

namespace Lumino
{
namespace Scene
{ 

//=============================================================================
// DrawingLayer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingLayer::DrawingLayer(SceneGraphManager* manager)
	: m_manager(manager)
	, m_camera(NULL)
	, m_renderingRootNode(NULL)
	, m_renderingPassList(LN_NEW RenderingPassRefList())
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingLayer::~DrawingLayer()
{
	LN_SAFE_RELEASE(m_camera);
	LN_SAFE_RELEASE(m_renderingRootNode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingLayer::PreRender(const SizeF& viewSize)
{
	LN_THROW(m_camera != NULL, InvalidOperationException);
	LN_THROW(m_renderingRootNode != NULL, InvalidOperationException);
	
	// �J�����s��̍X�V (TODO: �����ōX�V����ƁA�������C���[�������J�������Q�Ƃ��Ă��鎞�ɖ��ʂȌv�Z�ɂȂ邩��)
	m_camera->UpdateMatrices(viewSize);

	// �V�F�[�_�̃J�����P�ʃf�[�^�̍X�V
	//		TODO: �Ƃ肠�����S�V�F�[�_�X�V���Ă���B����Ȃɂ�������̃V�F�[�_�͎g��Ȃ����낤�Ƃ����z��B
	//		������񐔂������Ă���΂��̕ӂ��p�t�H�[�}���X�I�ɃN���e�B�J���ɂȂ�B
	//		���P�Ă͂��邪�A�Ƃ肠�����B(SceneGraphManaer.cpp �Q��)
	LN_FOREACH(MMEShader* shader, *m_manager->GetShaderList()) {
		shader->UpdateCameraParams(m_camera, viewSize);
	}

	// ���̃��C���[�̃J�����Ɉ˂�f�[�^���X�V���� (������J�����O��J��������̋����̍X�V)
	m_renderingRootNode->UpdateViewFlustumHierarchy(m_camera, &m_renderingNodeList, &m_renderingLightList);

	// ���C�g�s��̍X�V
	LN_FOREACH(Light* light, m_renderingLightList) {
		light->UpdateMatrices(viewSize);
	}

	// Z �\�[�g�E�D��x�\�[�g
	std::stable_sort(m_renderingNodeList.begin(), m_renderingNodeList.end(), SceneNode::CmpZAndPrioritySort);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingLayer::Render()
{
	RenderingParams params;
	params.Renderer = m_manager->GetGraphicsManager()->GetRenderer();
	params.GeometryRenderer = m_manager->GetGeometryRenderer();
	params.CurrentCamera = m_camera;
	LN_FOREACH(RenderingPass* pass, *m_renderingPassList)
	{
		params.Pass = pass;	// TODO: ����Ȃ�����
		LN_FOREACH(SceneNode* node, m_renderingNodeList)
		{
			pass->RenderNode(params, node);
		}
		pass->PostRender(params);
	}
}

} // namespace Scene
} // namespace Lumino
