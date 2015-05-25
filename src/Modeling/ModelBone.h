
#pragma once
#include "../Animation/AnimationState.h"
#include "ModelCore.h"

namespace Lumino
{
namespace Modeling
{

/// �{�[���̃C���X�^���X�N���X
class ModelBone
	: public RefObject
	, public Animation::IAnimationTargetAttribute
{
public:
	ModelBone();
	virtual ~ModelBone();

public:

	/// �쐬
	void Create(ModelBoneCore* core);

	/// ModelBoneCore �̎擾
	ModelBoneCore* GetCore() { return m_core; }

	/// �q�{�[���̒ǉ�
	void AddChildBone(ModelBone* bone) { m_children.Add(bone); }

	/// �{�[���s����K�w�I�ɍX�V����
	void UpdateTransformHierarchy(const Matrix& parentMatrix);

	///  �����ςݍs�� (���f�����̃O���[�o���s��) �̎擾
	const Matrix& GetCombinedMatrix() const { return m_combinedMatrix; }

	/// ���[�J���s��������l�ɖ߂�
	void ResetLocalTransform() { m_localTransform = SQTTransform::Identity; }

protected:
	// override Animation::IAnimationTargetAttribute
	virtual const String& GetAnimationTargetName() const { return m_core->Name; }
	virtual void SetAnimationTargetValue(Animation::ValueType type, const void* value);

private:
	RefPtr<ModelBoneCore>	m_core;				///< ���L�f�[�^�N���X
	ModelBoneList			m_children;			///< �q�{�[�����X�g
	SQTTransform			m_localTransform;	///< ���[�V�������������ނ̂͂���
	Matrix					m_combinedMatrix;	///< �����ςݍs�� (���f�����̃O���[�o���s��)
};

} // namespace Modeling
} // namespace Lumino
