
#pragma once
#include "../../src/Animation/AnimationState.h"		//  TODO
#include "../../src/Animation/Animator.h"		//  TODO
#include "../Mesh.h"

LN_NAMESPACE_BEGIN
class Animator;
class PmxSkinnedMeshResource;	// TODO: ���ۉ�������
class PmxBoneResource;			// TODO: ���ۉ�������
class PmxIKResource;
class SkinnedMeshModel;
class SkinnedMeshBone;
using SkinnedMeshModelPtr = RefPtr<SkinnedMeshModel>;
using SkinnedMeshBonePtr = RefPtr<SkinnedMeshBone>;

class RigidBody;	// TODO: MMD �ł̂ݕK�v
class DofSpringJoint;		// TODO: MMD �ł̂ݕK�v
namespace detail { class PhysicsWorld; }	// TODO: MMD �ł̂ݕK�v
namespace detail { class MmdSkinnedMeshRigidBody; }
namespace detail { class MmdSkinnedMeshJoint; }
class PmxRigidBodyResource;
class PmxJointResource;


/**
	@brief
*/
class SkinnedMeshModel
	: public Object
	, public detail::IAnimationTargetElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	// TODO: Unity �ł� Mesh ����͐؂藣���ꂽ�Ɨ������R���|�[�l���g�ł���B���������ق����������ȁH
	Animator* GetAnimator() const { return m_animator; }

protected:
	// IAnimationTargetElement interface
	virtual int GetAnimationTargetAttributeCount() const override;
	virtual detail::IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) override;

LN_INTERNAL_ACCESS:
	SkinnedMeshModel();
	virtual ~SkinnedMeshModel();
	void Initialize(GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh);

	void PreUpdate();

	void PostUpdate();



	// �{�[���s����A���[�g�{�[������K�w�I�ɍX�V����
	// (�A�j���[�V�����K�p��ɌĂяo��)
	void UpdateBoneTransformHierarchy();

	// �X�L�j���O�Ɏg�p����ŏI�{�[���s��̍쐬
	void UpdateSkinningMatrices();

	// �X�L�j���O�s��z��̎擾 (�v�f���͗v�f���̓{�[�����B��������̂܂܃X�L�j���O�e�N�X�`���ɏ������߂�)
	//Matrix* GetSkinningMatrices() { return m_skinningMatrices; }

	// �X�L�j���O�s��z����������񂾃e�N�X�`���̎擾
	Texture* GetSkinningMatricesTexture() { return m_skinningMatricesTexture; }

	// �T�u�Z�b�g���̎擾
	//int GetSubsetCount() const;

	// �}�e���A���擾
	//const Material& GetMaterial(int subsetIndex) const;

	// �T�u�Z�b�g�`��
	//void DrawSubset(int subsetIndex);

private:
	void UpdateIK();
	void UpdateBestow();


LN_INTERNAL_ACCESS:	// TODO:
	RefPtr<PmxSkinnedMeshResource>	m_meshResource;
	RefPtr<MaterialList>			m_materials;
	Array<SkinnedMeshBonePtr>		m_allBoneList;				// �S�{�[�����X�g
	Array<SkinnedMeshBone*>			m_rootBoneList;				// ���[�g�{�[�����X�g (�e�������Ȃ��{�[�����X�g)
	Array<Matrix>					m_skinningMatrices;			// �X�L�j���O�Ɏg�p����ŏI�{�[���s�� (�v�f���̓{�[����)
	Array<Quaternion>				m_skinningLocalQuaternions;
	RefPtr<Texture2D>				m_skinningMatricesTexture;	// Texture fetch �ɂ�� GPU �X�L�j���O�p�̃e�N�X�`��
	RefPtr<Texture2D>				m_skinningLocalQuaternionsTexture;	// Texture fetch �ɂ�� GPU �X�L�j���O�p�̃e�N�X�`��
	RefPtr<Animator>				m_animator;
	Array<SkinnedMeshBone*>			m_ikBoneList;

	// TODO: ����͕������Z�@�\�����T�u�N���X��������ق��������C������
	RefPtr<detail::PhysicsWorld>	m_physicsWorld;
	Array<RefPtr<detail::MmdSkinnedMeshRigidBody>>	m_rigidBodyList;
	Array<RefPtr<detail::MmdSkinnedMeshJoint>>		m_jointList;
};


/**
	@brief
*/
class SkinnedMeshBone
	: public Object
	, public detail::IAnimationTargetAttribute
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	SkinnedMeshBone* GetParent() const { return m_parent; }

LN_INTERNAL_ACCESS:
	SkinnedMeshBone();
	virtual ~SkinnedMeshBone();
	void Initialize(PmxBoneResource* boneResource);
	void PostInitialize(SkinnedMeshModel* owner, int depth);

	// PmxBoneResource �̎擾
	PmxBoneResource* GetCore() const;

	// �q�{�[���̒ǉ�
	void AddChildBone(SkinnedMeshBone* bone);

	// �{�[���s����K�w�I�ɍX�V����
	void UpdateGlobalTransform(bool hierarchical);

	//  �����ςݍs�� (���f�����̃O���[�o���s��) �̎擾
	const Matrix& GetCombinedMatrix() const { return m_combinedMatrix; }

	// ���[�J���s��������l�ɖ߂�
	void ResetLocalTransform() { m_localTransform = SQTTransform::Identity; }

	SQTTransform* GetLocalTransformPtr() { return &m_localTransform; }

protected:
	// IAnimationTargetAttribute interface
	virtual const String& GetAnimationTargetName() const override;
	virtual void SetAnimationTargetValue(ValueType type, const void* value) override;

LN_INTERNAL_ACCESS:	// TODO
	RefPtr<PmxBoneResource>	m_core;				// ���L�f�[�^�N���X
	SkinnedMeshBone*		m_parent;
	Array<SkinnedMeshBone*>	m_children;			// �q�{�[�����X�g
	SQTTransform			m_localTransform;	// ���[�V�������������ނ̂͂���
	Matrix					m_combinedMatrix;	// �����ςݍs�� ()
	int						m_depth;			// 0 ����
	PmxIKResource*			m_ikInfo;

	friend class SkinnedMeshModel;
};

namespace detail
{

class MmdSkinnedMeshRigidBody
	: public RefObject
{
LN_INTERNAL_ACCESS:
	MmdSkinnedMeshRigidBody();
	virtual ~MmdSkinnedMeshRigidBody();
	void Initialize(SkinnedMeshModel* ownerModel, PmxRigidBodyResource* rigidBodyResource, float scale);

	RigidBody* GetRigidBody() const;
	void UpdateBeforePhysics();
	void UpdateAfterPhysics();

private:
	PmxRigidBodyResource*	m_resource;
	SkinnedMeshBone*		m_bone;
	RefPtr<RigidBody>		m_rigidBody;
	Matrix					m_boneLocalPosition;
	Matrix					m_boneOffset;
	Matrix					m_offsetBodyToBone;
};

class MmdSkinnedMeshJoint
	: public RefObject
{
LN_INTERNAL_ACCESS:
	MmdSkinnedMeshJoint();
	virtual ~MmdSkinnedMeshJoint();
	void Initialize(SkinnedMeshModel* ownerModel, PmxJointResource* jointResource);

private:
	RefPtr<DofSpringJoint>	m_joint;
};

}

LN_NAMESPACE_END
