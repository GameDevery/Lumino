
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
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(sharingMesh != nullptr);

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
// �p���X�V�@ (���[�U�[�X�V�̑O�Ɋm�肷��K�v���������)
//		�E�{�[���̃O���[�o���s��X�V
//		�E���̍X�V (�t���[���ʒu�����̈ʒu��)
void SkinnedMeshModel::PreUpdate()
{
	UpdateBoneTransformHierarchy();
}

//------------------------------------------------------------------------------
// �p���X�V�A (���[�U�[�X�V�̌�A�`��̑O�ɍs���K�v���������)
//		�E���[�t�X�V
//		�EIK�X�V
//		�E���̍X�V (���̈ʒu���t���[���ʒu��)
//		�E�X�L�j���O�s��̍쐬
void SkinnedMeshModel::PostUpdate()
{
	// IK �X�V
	for (PmxIKResource* ik : m_meshResource->iks)
	{
		SolveIK(ik);
	}

	// �X�L�j���O�s��̍쐬
	UpdateSkinningMatrices();
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::UpdateBoneTransformHierarchy()
{
	for (SkinnedMeshBone* bone : m_rootBoneList)
	{
		bone->UpdateGlobalTransform(true);
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
static void NormalizeEular(Vector3* eulers)
{
	float& x = eulers->x;
	float& y = eulers->y;
	float& z = eulers->z;
	if (x < -Math::PI || Math::PI < x)
	{
		if (x > 0)
		{
			x -= Math::PI * 2;
		}
		else
		{
			x += Math::PI * 2;
		}
	}
	if (y < -Math::PI*0.5f || Math::PI*0.5f < y)
	{
		if (y > 0)
		{
			y -= Math::PI * 2;
		}
		else
		{
			y += Math::PI * 2;
		}
	}
	if (z < -Math::PI || Math::PI < z)
	{
		if (z > 0)
		{
			z -= Math::PI * 2;
		}
		else
		{
			z += Math::PI * 2;
		}
	}
}

//------------------------------------------------------------------------------
static void LimitIKRotation(const PmxIKResource::IKLink& ikLink, SkinnedMeshBone* ikLinkBone)
{
	if (!ikLink.IsRotateLimit) return;

	Quaternion& localRot = ikLinkBone->GetLocalTransformPtr()->rotation;

	RotationOrder type;
	bool locked;
	Vector3 euler;

	type = RotationOrder::XYZ;
	euler = localRot.ToEulerAngles(type, &locked);
	if (locked)
	{
		type = RotationOrder::YZX;
		euler = localRot.ToEulerAngles(type, &locked);
		if (locked)
		{
			type = RotationOrder::ZXY;
			euler = localRot.ToEulerAngles(type, &locked);
			if (locked)
			{
				LN_ASSERT(0);	// ���蓾�Ȃ��͂������c
			}
		}
	}

	// �p�x�C��
	NormalizeEular(&euler);
	euler.Clamp(ikLink.MinLimit, ikLink.MaxLimit);

	// �߂�
	Matrix rotMat = Matrix::MakeRotationEulerAngles(euler, type);
	localRot = Quaternion::MakeFromRotationMatrix(rotMat);
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::SolveIK(PmxIKResource* ik)
{
	/*
		[����] MMD��IK�ɂ́AIK�̌v�Z���ʂ�IK�n��̎q�{�[���ɂ͓K�p����Ȃ��Ƃ�������������܂��B
		http://ch.nicovideo.jp/penguin/blomaga/ar70894
	*/

	//ModelFrame2* effector = mOwnerModel->getFrame( ikTargetBone->getFrameCore()->IKTargetBoneIndex );

	// IK�\���{�[���̃O���[�o���s����čX�V����
	//for ( short jk = ik->IKLinks.size() - 1 ; jk >= 0 ; --jk)
	//{
	//	mOwnerModel->getFrame(ik->IKLinks[jk].LinkBoneIndex)->updateGlobalMatrix(false);
	//	//frames[mIKCore->IKBoneIndexArray[j]].updateGlobalMatrix( false );
	//}
	//effector->updateGlobalMatrix( true );


	//ModelFrameCore* frameCore = ikTargetBone->getFrameCore();

	SkinnedMeshBone* ikBone = m_allBoneList[ik->IKBoneIndex];			// IK �{�[�� (IK �������{�[���B�ڕW�n�_)
	SkinnedMeshBone* effector = m_allBoneList[ik->IKTargetBoneIndex];	// IK �^�[�Q�b�g�{�[�� (�G�t�F�N�^�BIK�Ɍ����ׂ��{�[�������̒��̐擪�{�[��)

	for (int i = 0; i < ik->LoopCount; ++i)
	{
		// IK�{�[���̃O���[�o���ʒu
		const Vector3& targetPos = ikBone->GetCombinedMatrix().GetPosition();

		for (int iLink = 0; iLink < ik->IKLinks.GetCount(); ++iLink)
		{
			// �������{�[��
			PmxIKResource::IKLink& ikLink = ik->IKLinks[iLink];
			SkinnedMeshBone* ikLinkBone = m_allBoneList[ikLink.LinkBoneIndex];

			// �G�t�F�N�^�̈ʒu
			const Vector3& effPos = effector->GetCombinedMatrix().GetPosition();

			// ���[���h���W�n���璍�ڃm�[�h�̋Ǐ����W�n�ւ̕ϊ�
			// (IK�����N��̃��[�J�����W�n�֕ϊ�����s��)
			Matrix invCoord = Matrix::MakeInverse(ikLinkBone->GetCombinedMatrix());
			
			// �e�x�N�g���̍��W�ϊ����s���A�������̃{�[��i��̍��W�n�ɂ���
			// (1) ���ڃm�[�h���G�t�F�N�^�ʒu�ւ̃x�N�g��(a)(���ڃm�[�h)
			Vector3 localEffPos = Vector3::TransformCoord(effPos, invCoord);

			// (2) ��֐�i���ڕW�ʒu�ւ̃x�N�g��(b)(�{�[��i����W�n)
			Vector3 localTargetPos = Vector3::TransformCoord(targetPos, invCoord);

			// (1) ��֐߁��G�t�F�N�^�ʒu�ւ̕����x�N�g��
			localEffPos.Normalize();
			// (2) ��֐߁��ڕW�ʒu�ւ̕����x�N�g��
			localTargetPos.Normalize();

			// ��]�p
			float rotationDotProduct = Vector3::Dot(localEffPos, localTargetPos);
			if (rotationDotProduct > 1.f) rotationDotProduct = 1.f;
			float rotationAngle = acosf(rotationDotProduct);

#if 0
			//��]�ʐ�����������
			if (rotationAngle > Math::PI * ik->IKRotateLimit * (iLink + 1))
				rotationAngle = Math::PI * ik->IKRotateLimit * (iLink + 1);
			if (rotationAngle < -Math::PI * ik->IKRotateLimit * (iLink + 1))
				rotationAngle = -Math::PI * ik->IKRotateLimit * (iLink + 1);

#else
			//��]�ʐ�����������
			if (rotationAngle > ik->IKRotateLimit)
				rotationAngle = ik->IKRotateLimit;
			if (rotationAngle < -ik->IKRotateLimit)
				rotationAngle = -ik->IKRotateLimit;
#endif

			// ��]��
			Vector3 rotationAxis = Vector3::Cross(localEffPos, localTargetPos);
			
			//if (frame->getFrameCore()->IKLimitter)
			//	frame->getFrameCore()->IKLimitter->adjustAxisLimits( &rotationAxis );
			rotationAxis.Normalize();


			if (!Math::IsNaN(rotationAngle) && rotationAngle > 1.0e-3f && !rotationAxis.IsNaNOrInf())
			{
				// �֐߉�]�ʂ̕␳
				Quaternion rotQuat(rotationAxis, rotationAngle);
				ikLinkBone->GetLocalTransformPtr()->rotation = Quaternion::Multiply(ikLinkBone->GetLocalTransformPtr()->rotation, rotQuat);

				// ��]����
				LimitIKRotation(ikLink, ikLinkBone);

				// ���[�̂ق�����AIK�\���{�[���̃O���[�o���s����čX�V����
				// TODO: IKLinks �̏��Ԃ͕t���������[�ւ�O��ɂ��Ă���B�O�̂��ߎ��O�`�F�b�N�����ق�����������
				for (short jk = ik->IKLinks.GetCount() - 1; jk >= 0; --jk)
				{
					m_allBoneList[ik->IKLinks[jk].LinkBoneIndex]->UpdateGlobalTransform(false);
				}
				effector->UpdateGlobalTransform(true);
			}
		}
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
	, m_parent(nullptr)
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
void SkinnedMeshBone::AddChildBone(SkinnedMeshBone* bone)
{
	LN_CHECK_ARG(bone != nullptr);
	LN_CHECK_ARG(bone->m_parent == nullptr);
	m_children.Add(bone);
	bone->m_parent = this;
}

//------------------------------------------------------------------------------
void SkinnedMeshBone::UpdateGlobalTransform(bool hierarchical)
{
	// m_localTransform �́A�{�[���̃��[�J���p���ŃA�j���[�V�������K�p����Ă���B
	// �K�p����Ă��Ȃ���� Identity�B
	m_combinedMatrix = m_localTransform;

	// �e����̕��s�ړ���
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	// �e�s��ƌ���
	if (m_parent != nullptr)
	{
		m_combinedMatrix *= m_parent->GetCombinedMatrix();
	}

	// �q�{�[���X�V
	if (hierarchical)
	{
		for (SkinnedMeshBone* bone : m_children)
		{
			bone->UpdateGlobalTransform(hierarchical);
		}
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
