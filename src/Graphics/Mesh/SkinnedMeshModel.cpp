
#include "../Internal.h"
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Graphics/Mesh/SkinnedMeshModel.h>
#include "../GraphicsManager.h"
#include "../../Modeling/PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SkinnedMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SkinnedMeshModel, Object);

//------------------------------------------------------------------------------
SkinnedMeshModel::SkinnedMeshModel()
	: m_meshResource(nullptr)
	, m_materials(nullptr)
	, m_allBoneList()
	, m_rootBoneList()
	, m_skinningMatrices()
	, m_skinningMatricesTexture(nullptr)
	, m_animator(nullptr)
{
}

//------------------------------------------------------------------------------
SkinnedMeshModel::~SkinnedMeshModel()
{
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::Initialize(GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh)
{
	if (LN_CHECKEQ_ARG(manager == nullptr)) return;
	if (LN_CHECKEQ_ARG(sharingMesh == nullptr)) return;

	// ���b�V��(�o�b�t�@��)�͋��L����
	m_meshResource = sharingMesh;

	// �}�e���A���̓R�s�[����
	// TODO: �R�s�[�L���̃t���O���������ق������������H
	int count = m_meshResource->materials.GetCount();
	m_materials = RefPtr<MaterialList>::MakeRef();
	m_materials->Resize(count);
	for (int i = 0; i < count; ++i)
	{
		m_materials->SetAt(i, m_meshResource->materials.GetAt(i)->MakeCommonMaterial());
	}

	//---------------------------------------------------------
	// Bone �̃C���X�^���X��
	int boneCount = m_meshResource->bones.GetCount();
	if (boneCount > 0)
	{
		m_allBoneList.Resize(boneCount);
		// �܂��� Bone �����
		for (int i = 0; i < boneCount; i++)
		{
			m_allBoneList[i] = SkinnedMeshBonePtr::MakeRef();
			m_allBoneList[i]->Initialize(m_meshResource->bones[i]);
		}
		// ���Ɏq�Ɛe���q����
		for (int i = 0; i < boneCount; i++)
		{
			int parentIndex = m_meshResource->bones[i]->ParentBoneIndex;
			if (0 <= parentIndex && parentIndex < boneCount)
			{
				m_allBoneList[parentIndex]->AddChildBone(m_allBoneList[i]);
			}
			else
			{
				m_rootBoneList.Add(m_allBoneList[i]);	// �e�����Ȃ��B���[�g�{�[���Ƃ��Ċo���Ă���
			}
		}

		// �{�[���s����������ނƂ�������
		m_skinningMatrices.Resize(boneCount);
		m_skinningMatricesTexture = RefPtr<Texture2D>::MakeRef();
		m_skinningMatricesTexture->Initialize(manager, SizeI(4, boneCount), TextureFormat::R32G32B32A32_Float, false);	// TODO: Dynamic�ANoManaged

		// �A�j���[�V�����Ǘ�
		m_animator = RefPtr<Animator>::MakeRef();
		m_animator->Create(this);
	}
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateBoneTransformHierarchy()
{
	for (SkinnedMeshBone* bone : m_rootBoneList)
	{
		bone->UpdateTransformHierarchy(Matrix::Identity);
	}
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateSkinningMatrices()
{
	// �X�L�j���O�s��̍쐬
	for (int i = 0; i < m_allBoneList.GetCount(); i++)
	{
		/*
			�����p���́A�X�L�j���O���Ȃ��Ă������p���B
			�܂�A���_�X�L�j���O�ŏ����p���ɂ������Ƃ��� Matrix::Identity �̃{�[���s���n���B

			�{�[���͍ŏ�����I�t�Z�b�g�������Ă邯�ǁA
			������X�L�j���O�ɓK�p����Ǝp��������Ă��܂��B
			���̂��߁A�����I�t�Z�b�g��ł������������K�v�B���ꂪ GetInitialTranstormInv()�B

			ID3DXSkinInfo::GetBoneOffsetMatrix() ��
			�擾�ł���s�� (SkinnedMesh�T���v���� D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) ������ɂ�������̂��ۂ��B
			�T���v���ł��`��̒��O�ɑΏۃ{�[���s��ɂ��̍s�����Z���Ă���B
		*/
		m_skinningMatrices[i] = m_allBoneList[i]->GetCore()->GetInitialTranstormInv();
		m_skinningMatrices[i] *= m_allBoneList[i]->GetCombinedMatrix();
	}

	// �X�L�j���O�e�N�X�`���X�V
	if (!m_skinningMatricesTexture.IsNull())
	{
		m_skinningMatricesTexture->SetSubData(Point(0, 0), &m_skinningMatrices[0]);
	}

	// �S�Ẵ��[�J���g�����X�t�H�[�������Z�b�g����
	//		���Z�b�g���Ă����Ȃ��ƁAIK�Ŗ�肪�o��B
	//		(IK�͂��̎��_��LocalTransform�ɑ΂��ď������s�����߁A��]�p�x���ǂ�ǂ񑝂����肷��)
	//		�Ȃ��A��A�̍X�V�̍Ō�ōs���Ă���̂́A�A�j���[�V��������̍X�V���O���ōs���Ă��邽�߁B
	// TODO: �ł���Έ�A�̏����̒��ŕK���ʂ�Ƃ���Ɉړ�������
	for (SkinnedMeshBone* bone : m_allBoneList)
	{
		bone->ResetLocalTransform();
	}
}

//------------------------------------------------------------------------------
int SkinnedMeshModel::GetAnimationTargetAttributeCount() const
{
	return m_allBoneList.GetCount();
}

//------------------------------------------------------------------------------
IAnimationTargetAttribute* SkinnedMeshModel::GetAnimationTargetAttribute(int index)
{
	return m_allBoneList[index].Get();
}

//==============================================================================
// SkinnedMeshBone
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SkinnedMeshBone, Object);

//------------------------------------------------------------------------------
SkinnedMeshBone::SkinnedMeshBone()
	: m_core(nullptr)
	, m_children()
	, m_localTransform()
	, m_combinedMatrix()
{
}

//------------------------------------------------------------------------------
SkinnedMeshBone::~SkinnedMeshBone()
{
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::Initialize(PmxBoneResource* boneResource)
{
	m_core = boneResource;
}

//------------------------------------------------------------------------------
PmxBoneResource* SkinnedMeshBone::GetCore() const
{
	return m_core;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::UpdateTransformHierarchy(const Matrix& parentMatrix)
{
	// m_localTransform �́A�{�[���̃��[�J���p���ŃA�j���[�V�������K�p����Ă���B
	// �K�p����Ă��Ȃ���� Identity�B
	m_combinedMatrix = m_localTransform;

	// �e����̕��s�ړ���
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	// �e�s��ƌ���
	m_combinedMatrix *= parentMatrix;

	// �q�{�[���X�V
	for (SkinnedMeshBone* bone : m_children)
	{
		bone->UpdateTransformHierarchy(m_combinedMatrix);
	}
}

//------------------------------------------------------------------------------
const String& SkinnedMeshBone::GetAnimationTargetName() const
{
	return m_core->Name;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::SetAnimationTargetValue(ValueType type, const void* value)
{
	LN_CHECK_ARG(type == ValueType_SQTTransform);
	m_localTransform = *((SQTTransform*)value);
}

LN_NAMESPACE_END
