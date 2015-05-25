
#include "../Internal.h"
#include <Lumino/Graphics/Renderer.h>
#include "ModelManager.h"
#include "ModelBone.h"
#include "Model.h"

//Lumino::Graphics::Device::IVertexBuffer* g_ttttt = NULL;
namespace Lumino
{
namespace Modeling
{

//=============================================================================
// Model
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Model::Model()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Model::~Model()
{
	LN_SAFE_DELETE_ARRAY(m_skinningMatrices);

	LN_FOREACH(ModelBone* obj, m_boneList) {
		obj->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::Create(ModelManager* manager, const PathName& filePath)
{
	m_manager = manager;
	m_modelCore.Attach(manager->CreateModelCore(filePath));

	//---------------------------------------------------------
	// Bone �̃C���X�^���X��
	int boneCount = m_modelCore->Bones.GetCount();
	m_boneList.Resize(boneCount);
	// �܂��� Create ����B
	for (int i = 0; i < boneCount; i++)
	{
		m_boneList[i] = LN_NEW ModelBone();
		m_boneList[i]->Create(m_modelCore->Bones[i]);
	}
	// ���Ɏq�Ɛe���q����
	for (int i = 0; i < boneCount; i++)
	{
		int parentIndex = m_modelCore->Bones[i]->ParentBoneIndex;
		if (0 <= parentIndex && parentIndex < boneCount) {
			m_boneList[parentIndex]->AddChildBone(m_boneList[i]);
		}
		else {
			m_rootBoneList.Add(m_boneList[i]);	// �e�����Ȃ��B���[�g�{�[���Ƃ��Ċo���Ă���
		}
	}
	m_skinningMatrices = LN_NEW Matrix[boneCount];
	m_skinningMatricesTexture.Attach(Graphics::Texture::Create(Size(4, boneCount), Graphics::TextureFormat_R32G32B32A32_Float, 1, m_manager->GetGraphicsManager()));

	// �A�j���[�V�����Ǘ�
	m_animator.Attach(LN_NEW Animation::Animator());
	m_animator->Create(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::UpdateBoneTransformHierarchy()
{
	LN_FOREACH(ModelBone* bone, m_rootBoneList)
	{
		bone->UpdateTransformHierarchy(Matrix::Identity);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::UpdateSkinningMatrices()
{
	// �X�L�j���O�s��̍쐬
	for (int i = 0; i < m_boneList.GetCount(); i++)
	{
		/*
		�����p���́A�X�L�j���O���Ȃ��Ă������p���B
		�܂�A���_�X�L�j���O�ŏ����p���ɂ������Ƃ��� Matrix::Identity �̃{�[���s���n���B

		�{�[���͍ŏ�����I�t�Z�b�g�������Ă邯�ǁA
		������X�L�j���O�ɓK�p����Ǝp��������Ă��܂��B
		���̂��߁A�����I�t�Z�b�g��ł������������K�v�B���ꂪ GetInitialTranstormInv()�B

		ID3DXSkinInfo::GetBoneOffsetMatrix() ��
		�擾�ł���s�� (SkinnedMesh�T���v���� D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) ��
		����ɂ�������̂��ۂ��B
		�T���v���ł��`��̒��O�ɑΏۃ{�[���s��ɂ��̍s�����Z���Ă���B
		*/
		m_skinningMatrices[i] = m_boneList[i]->GetCore()->GetInitialTranstormInv();
		m_skinningMatrices[i] *= m_boneList[i]->GetCombinedMatrix();
	}

	// �X�L�j���O�e�N�X�`���X�V
	if (!m_skinningMatricesTexture.IsNull())
	{
		m_skinningMatricesTexture->SetSubData(Point(0, 0), m_skinningMatrices);
	}


	// �S�Ẵ��[�J���g�����X�t�H�[�������Z�b�g����
	//		���Z�b�g���Ă����Ȃ��ƁAIK�Ŗ�肪�o��B
	//		(IK�͂��̎��_��LocalTransform�ɑ΂��ď������s�����߁A��]�p�x���ǂ�ǂ񑝂����肷��)
	//		�Ȃ��A��A�̍X�V�̍Ō�ōs���Ă���̂́A�A�j���[�V��������̍X�V��
	//		�O���ōs���Ă��邽�߁B
	// TODO: �ł���Έ�A�̏����̒��ŕK���ʂ�Ƃ���Ɉړ�������
	LN_FOREACH(ModelBone* obj, m_boneList) {
		obj->ResetLocalTransform();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Model::GetSubsetCount() const
{
	return m_modelCore->Material.Attributes.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Graphics::Material& Model::GetMaterial(int subsetIndex) const
{
	return m_modelCore->Material.Materials[subsetIndex];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::DrawSubset(int subsetIndex)
{
	//g_ttttt = m_modelCore->VertexBuffer->m_deviceObj;

	Graphics::Renderer* r = m_manager->GetGraphicsManager()->GetRenderer();
	r->SetVertexBuffer(m_modelCore->VertexBuffer);
	r->SetIndexBuffer(m_modelCore->IndexBuffer);
	r->DrawPrimitiveIndexed(
		Graphics::PrimitiveType_TriangleList,
		m_modelCore->Material.Attributes[subsetIndex].StartIndex,
		m_modelCore->Material.Attributes[subsetIndex].PrimitiveNum);
}

} // namespace Modeling
} // namespace Lumino
