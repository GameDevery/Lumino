
#pragma once

#include "../Internal.h"
#include "MME/MMEShaderTechnique.h"
#include "Light.h"
#include "VisualNode.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// VisualNode
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNode::VisualNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNode::~VisualNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::Create(SceneGraphManager* manager, int subsetCount)
{
	SceneNode::Create(manager);
	m_visualNodeParams.Create(subsetCount);
	m_subsetCount = subsetCount;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::UpdateFrameHierarchy(SceneNode* parent, SceneNodeList* renderingNodeList)
{
	// �e�� VisualNode �ł���� Visual �֌W�̃v���p�e�B���p�����čX�V����
	if (parent != NULL && parent->GetSceneNodeType() == SceneNodeType_VisualNode) {
		m_visualNodeParams.UpdateSubsetRenderParam(&static_cast<VisualNode*>(parent)->m_visualNodeParams);
	}
	else {
		m_visualNodeParams.UpdateSubsetRenderParam(NULL);
	}

	// ���̃m�[�h�͕`��ł���
	renderingNodeList->Add(this);

	SceneNode::UpdateFrameHierarchy(parent, renderingNodeList);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList)
{
	// TODO: ������J�����O��

	// TOOD: Z�\�[�g�p�̋������v�Z


	// ���̃m�[�h�͕`��ł���
	renderingNodeList->Add(this);

	SceneNode::UpdateViewFlustumHierarchy(camera, renderingNodeList, renderingLightList);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::UpdateAffectLights(LightNodeList* renderingLightList, int maxCount)
{
	/*
		�܂��S�Ẵ��C�g�ɁA���̃m�[�h�Ƃ̋������Z�b�g����B
		���̌�߂����Ƀ\�[�g���āA�\�[�g���ʂ̐擪����K�v�Ȑ�������肾���Ă���B
		���C�g�̐������Ȃ���΂ǂ�ȃA���S���Y���ɂ��Ă��卷�͂Ȃ��Ǝv�����A
		�m�[�h�P�ʂŃ\�[�g�����s�����̂ő��x�I�ɏ����S�z�B
		�擪�����m�肵���Ƃ��Ƀ\�[�g���I�����铙�A�œK���̗]�n�͂��邪�c�B
	*/

	m_affectLightList.Resize(maxCount);

	// �\�[�g��l�̌v�Z
	LN_FOREACH(Light* light, *renderingLightList) {
		light->m_tmpDistance = (light->m_combinedGlobalMatrix.GetPosition() - m_combinedGlobalMatrix.GetPosition()).GetLengthSquared();
	}

	// �\�[�g
	std::stable_sort(renderingLightList->begin(), renderingLightList->end(), CmpLightSort);

	// �o�� (����Ȃ����� NULL �Ŗ��߂�)
	int mn = renderingLightList->GetCount();
	int n = maxCount;
	int i = 0;
	for (; i < mn; ++i) {
		m_affectLightList[i] = renderingLightList->GetAt(i);
	}
	for (; i < n; ++i) {
		m_affectLightList[i] = NULL;
	}
}
bool VisualNode::CmpLightSort(const Light* left, const Light* right)
{
	if (left->m_priority == right->m_priority)
	{
		// �����͏����B�߂��ق����ɕ`�悷��B
		return left->m_zDistance < right->m_zDistance;
	}
	// �D��x�͍~���B�����ق����ɕ`�悷��B
	return left->m_priority < right->m_priority;
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::Render(RenderingParams& params)
{
	// �����_�����O�X�e�[�g�̐ݒ�
	params.Renderer->SetRenderState(m_renderState);

	for (int iSubset = 0; iSubset < m_subsetCount; iSubset++)
	{
		// ����̃p�X�Ŗ{���ɕK�v�ȏ�� (�g�p����V�F�[�_��) ���擾����
		RenderingPriorityParams priorityParams;
		params.Pass->SelectPriorityParams(this, iSubset, &priorityParams);
		if (priorityParams.Hide) {	// ���̃p�X�ł͕`�悵�Ȃ�
			return;
		}
		params.Shader = priorityParams.Shader;

		// �V�F�[�_�̃m�[�h�P�ʃf�[�^���X�V����
		priorityParams.Shader->UpdateNodeParams(this, params.CurrentCamera, m_affectLightList);

		// �e�N�j�b�N�̌���
		//m_visualNodeParams.GetCombinedSubsetParams
		MMEShaderTechnique* tech = priorityParams.Shader->FindTechnique(
			(InternalRenderingPass)params.Pass->GetPassID(),
			!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.Texture.IsNull(),
			!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.SphereTexture.IsNull(),
			!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.ToonTexture.IsNull(),
			false,	// TODO
			iSubset);

		// �e�N�j�b�N��������Ȃ������B���̏����ɓ��Ă͂܂�̂́A�e�N�j�b�N�̃^�[�Q�b�g�T�u�Z�b�g�͈͂��w�肳��Ă��āA
		// iSubset ��������ɂ��}�b�`���Ȃ������ꍇ�B���̏ꍇ�̓f�t�H���g�̃V�F�[�_��T���B
		if (tech == NULL)
		{
			if (params.Pass->GetDefaultShader() != NULL)
			{
				tech = params.Pass->GetDefaultShader()->FindTechnique(
					(InternalRenderingPass)params.Pass->GetPassID(),
					!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.Texture.IsNull(),
					!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.SphereTexture.IsNull(),
					!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.ToonTexture.IsNull(),
					false,	// TODO
					iSubset);
				if (tech == NULL) {
					// �f�t�H���g�̃V�F�[�_�ɂ���v����e�N�j�b�N��������Ȃ������B
					// ���� iSubset �͕`�悵�Ȃ��B�Ƃ������ł��Ȃ��B
					continue;
				}
			}
		}

		// �R�}���h�o�R�ŕ`����s
		ShaderScriptCommandList::DrawParams dp;
		dp.Params = &params;
		dp.RenderingNode = this;
		dp.SubsetIndex = iSubset;
		tech->GetDrawingCommandList().Execute(dp);
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::DrawSubsetInternal(RenderingParams& params, int subsetIndex, MMEShader* shader, Graphics::ShaderPass* pass)
{
	// �V�F�[�_�̃T�u�Z�b�g�P�ʂ̃f�[�^���X�V����
	if (shader != NULL) {
		shader->UpdateSubsetParams(m_visualNodeParams.GetCombinedSubsetParams(subsetIndex));
	}

	// �p�X�J�n
	if (pass != NULL) {
		pass->Apply();
	}

	// �T�u�Z�b�g�`��̖{��
	DrawSubset(params, subsetIndex);
}


} // namespace Scene
} // namespace Lumino
