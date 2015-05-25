
#include "../../Internal.h"
#include "../VisualNode.h"
#include "MMERenderingPass.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// RenderingPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMERenderingPass::MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, MMEShader* ownerShader)
	: RenderingPass(manager)
	, m_mmdPass(mmdPass)
	, m_ownerShader(ownerShader)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMERenderingPass::~MMERenderingPass()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMERenderingPass::RenderNode(RenderingParams& params, SceneNode* node)
{
	if (node->GetSceneNodeType() != SceneNodeType_VisualNode) {
		return;
	}

	VisualNode* visualNode = static_cast<VisualNode*>(node);

	// �����_�����O�X�e�[�g�̐ݒ�
	params.Renderer->SetRenderState(visualNode->GetRenderState());

	int subsetCount = visualNode->GetSubsetCount();
	for (int iSubset = 0; iSubset < subsetCount; iSubset++)
	{
		// ����̃p�X�Ŗ{���ɕK�v�ȏ�� (�g�p����V�F�[�_��) ���擾����
		RenderingPriorityParams priorityParams;
		SelectPriorityParams(visualNode, iSubset, &priorityParams);
		if (priorityParams.Hide) {	// ���̃p�X�ł͕`�悵�Ȃ�
			return;
		}
		params.Shader = priorityParams.Shader;

		// �m�[�h�P�ʃf�[�^���X�V����
		priorityParams.Shader->UpdateNodeParams(visualNode, params.CurrentCamera, *visualNode->GetAffectLightList());
		visualNode->UpdateNodeRenderingParams(priorityParams.Shader);

		const Graphics::Material& material = visualNode->GetVisualNodeParams().GetCombinedSubsetParams(iSubset).Material;

		// �e�N�j�b�N�̌���
		MMEShaderTechnique* tech = priorityParams.Shader->FindTechnique(
			m_mmdPass,
			!material.Texture.IsNull(),
			!material.SphereTexture.IsNull(),
			!material.ToonTexture.IsNull(),
			false,	// TODO
			iSubset);

		// �e�N�j�b�N��������Ȃ������B���̏����ɓ��Ă͂܂�̂́A�e�N�j�b�N�̃^�[�Q�b�g�T�u�Z�b�g�͈͂��w�肳��Ă��āA
		// iSubset ��������ɂ��}�b�`���Ȃ������ꍇ�B���̏ꍇ�̓f�t�H���g�̃V�F�[�_��T���B
		if (tech == NULL)
		{
			if (params.Pass->GetDefaultShader() != NULL)
			{
				tech = params.Pass->GetDefaultShader()->FindTechnique(
					m_mmdPass,
					material.Texture.IsNull(),
					material.SphereTexture.IsNull(),
					material.ToonTexture.IsNull(),
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
		dp.RenderingNode = visualNode;
		dp.SubsetIndex = iSubset;
		tech->GetDrawingCommandList().Execute(dp);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMERenderingPass::SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams)
{
	RenderingPassClientData* data = &node->m_renderingPassClientDataList[m_internalEntryID];

	// node �� m_internalID �Ԗڂ̃t�B���^����ݒ肵�����̂� this �ł͂Ȃ��B
	// (node ���V�����쐬���ꂽ�I�u�W�F�N�g�ł��邩�A�O�� RenderingPass ��������� ID ���ԋp���ꂽ��A�V���ɍ쐬���ꂽ RenderingPass ������ ID ���擾�����ꍇ)
	// �m�[�h�����`�F�b�N���Ċ��蓖�Ă�V�F�[�_�̃C���f�b�N�X�� node �Ɏ������Ă����B
	if (data->OwnerPass != this)
	{
		data->OwnerPass = this;
		data->PriorityShaderIndex = -1;	// ��v�Ȃ����l�����āA�����l�͔�\���O���[�v

		int count = m_priorityEntryList.GetCount();
		for (int i = 0; i < count; ++i)
		{
			PriorityParamsEntry& e = m_priorityEntryList[i];

			// "*" �͑S�Ẵm�[�h��ΏۂƂ���L�[�B���̂܂ܒǉ��B
			if (e.MatchingNameKey == _T("*"))
			{
				data->PriorityShaderIndex = i;
				break;
			}
			// ���̃I�t�X�N���[��RT�����V�F�[�_���ݒ肳��Ă���m�[�h���g
			else if (e.MatchingNameKey == _T("self") && node->GetSceneNodeType() == SceneNodeType_VisualNode)
			{
				if (static_cast<VisualNode*>(node)->GetShader(-1) == m_ownerShader)
				{
					data->PriorityShaderIndex = i;
					break;
				}
			}
			// ���C���h�J�[�h�t���̔�r
			else if (StringUtils::Match(e.MatchingNameKey.GetCStr(), node->GetName().GetCStr()))
			{
				data->PriorityShaderIndex = i;
				break;
			}
		}
	}

	// ���� OffscreenScene �𐶐������V�F�[�_�Ɠ���̃V�F�[�_�������Ă�����̂�K���Ƀ`���C�X����
	// (OffscreenScene ���܂ރV�F�[�_�͊�{�I�ɋ��L���T�|�[�g���Ȃ��B���̂��߁A��ӂɂȂ�͂�)
	//if (obj->getShader() && obj->getShader() == mSceneShader) {
	//	mOffscreenOwner = obj;
	//}

	// �O���[�v��������Ȃ������m�[�h�̓f�t�H���g�l�ŕԂ�
	if (data->PriorityShaderIndex = -1)
	{
		if (m_priorityEntryList.IsEmpty() && node->GetSceneNodeType() == SceneNodeType_VisualNode)
		{
			// �D��p�����[�^���ݒ�B (OFFSCREENRENDERTARGET �ł͂Ȃ�)
			// �m�[�h�̎����Ă���V�F�[�_��Ԃ��B
			outParams->Shader = static_cast<VisualNode*>(node)->GetVisualNodeParams().GetCombinedSubsetParams(subsetIndex).SceneShader;//m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader;
		}

		outParams->Hide = false;
	}
	// �o�^����Ă���O���[�v
	else
	{
		// TODO: subsetIndex ���l�������V�F�[�_�̊��蓖��

		outParams->Shader = m_priorityEntryList[data->PriorityShaderIndex].Params.Shader;
		outParams->Hide = m_priorityEntryList[data->PriorityShaderIndex].Params.Hide;
	}

	// �����܂łŉ����I������Ă��Ȃ���΃f�t�H���g�̃V�F�[�_���g�p����
	if (outParams->Shader == NULL) {
		outParams->Shader = m_defaultShader;
	}

	// �����܂łŉ����I������Ă��Ȃ��Ƃ������Ƃ́A�f�t�H���g�̃V�F�[�_����Ȃ������Ƃ������ƁB
	// �`����s��Ȃ��_�~�[�p�X�ƍl���邱�Ƃ��ł��邪�A����Ȃ̉��Ɏg���̂��B
	// �Ƃ肠��������ł͑z��O�ł���B
	LN_THROW(outParams->Shader != NULL, InvalidOperationException);
}

} // namespace Scene
} // namespace Lumino
