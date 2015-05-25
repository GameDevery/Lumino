
#include "../Internal.h"
#include "ModelBone.h"

namespace Lumino
{
namespace Modeling
{

//=============================================================================
// ModelBone
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelBone::ModelBone()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelBone::~ModelBone()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelBone::Create(ModelBoneCore* core)
{
	m_core = core;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelBone::UpdateTransformHierarchy(const Matrix& parentMatrix)
{
	// m_localTransform �́A�{�[���̃��[�J���p���ŃA�j���[�V�������K�p����Ă���B
	// �K�p����Ă��Ȃ���� Identity�B
	m_combinedMatrix = m_localTransform;

	// �e����̕��s�ړ���
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	// �e�s��ƌ���
	m_combinedMatrix *= parentMatrix;

	// �q�{�[���X�V
	LN_FOREACH(ModelBone* bone, m_children)
	{
		bone->UpdateTransformHierarchy(m_combinedMatrix);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelBone::SetAnimationTargetValue(Animation::ValueType type, const void* value)
{
	LN_VERIFY(type == Animation::ValueType_SQTTransform) { return; }
	m_localTransform = *((SQTTransform*)value);
}

} // namespace Modeling
} // namespace Lumino
