
#pragma once
#include <Lumino/IO/PathName.h>
#include "../Animation/Animator.h"
#include "ModelCore.h"
#include "ModelBone.h"

namespace Lumino
{
namespace Modeling
{

/// Model
class Model
	: public RefObject
	, public Animation::IAnimationTargetElement
{
public:
	Model();
	virtual ~Model();

public:

	/// �쐬
	void Create(ModelManager* manager, const PathName& filePath);

	/// ���̃��f���ɑ΂���A�j���[�V�������s���N���X�̎擾
	Animation::Animator* GetAnimator() { return m_animator; }

	/// �{�[���s����A���[�g�{�[������K�w�I�ɍX�V����
	/// (�A�j���[�V�����K�p��ɌĂяo��)
	void UpdateBoneTransformHierarchy();

	/// �X�L�j���O�Ɏg�p����ŏI�{�[���s��̍쐬
	void UpdateSkinningMatrices();

	/// �X�L�j���O�s��z��̎擾 (�v�f���͗v�f���̓{�[�����B��������̂܂܃X�L�j���O�e�N�X�`���ɏ������߂�)
	Matrix* GetSkinningMatrices() { return m_skinningMatrices; }

	/// �X�L�j���O�s��z����������񂾃e�N�X�`���̎擾
	Graphics::Texture* GetSkinningMatricesTexture() { return m_skinningMatricesTexture; }

	/// �T�u�Z�b�g���̎擾
	int GetSubsetCount() const;

	/// �}�e���A���擾
	const Graphics::Material& GetMaterial(int subsetIndex) const;

	/// �T�u�Z�b�g�`��
	void DrawSubset(int subsetIndex);

protected:
	virtual int GetAnimationTargetAttributeCount() const { return m_boneList.GetCount(); }
	virtual Animation::IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) { return m_boneList[index]; }

private:
	ModelManager*				m_manager;
	RefPtr<ModelCore>			m_modelCore;
	RefPtr<Animation::Animator>	m_animator;		// TODO: �{�[���A�j���ƕ\��A�j���͕�����ׂ������H
	ModelBoneList				m_boneList;					///< �S�{�[�����X�g
	ModelBoneList				m_rootBoneList;				///< ���[�g�{�[�����X�g (�e�������Ȃ��{�[�����X�g)
	Matrix*						m_skinningMatrices;			///< �L�j���O�Ɏg�p����ŏI�{�[���s�� (�v�f���̓{�[����)
	RefPtr<Graphics::Texture>	m_skinningMatricesTexture;	///< Texture fetch �ɂ�� GPU �X�L�j���O�p�̃e�N�X�`��
	
};

} // namespace Modeling
} // namespace Lumino
