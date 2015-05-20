
#pragma once

#include "../Internal.h"
#include "VisualNodeParams.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// VisualNodeParams
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNodeParams::VisualNodeParams()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNodeParams::~VisualNodeParams()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNodeParams::Create(int subsetCount)
{
	LN_THROW(subsetCount >= 1, ArgumentException);

	m_combinedSubsetParams.Resize(subsetCount);

	// ���� 1 ��葽����΃T�u�Z�b�g�P�ʂ̃p�����[�^���e�쐬����B
	// (1�̏ꍇ�̓������g�p�ʂ�}���邽�߁Am_globalParams �������g��)
	if (subsetCount > 1) {
		m_subsetParams.Resize(subsetCount);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNodeSubsetParams& VisualNodeParams::GetSubsetParams(int index)
{
	// -1 �̎��� m_globalParams ��Ԃ��B
	// �܂��A�T�u�Z�b�g���� 1 �̎��̓������g�p�ʂ��������邽�� m_subsetParams ���m�ۂ��Ă��Ȃ��Bm_globalParams �����L����B
	if (index == -1 || m_subsetParams.IsEmpty()) {
		return m_globalParams;
	}
	return m_subsetParams[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const VisualNodeSubsetParams& VisualNodeParams::GetSubsetParams(int index) const
{
	// �����Ɠ���
	if (index == -1 || m_subsetParams.IsEmpty()) {
		return m_globalParams;
	}
	return m_subsetParams[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNodeParams::UpdateSubsetRenderParam(const VisualNodeParams* parentParams)
{
	//---------------------------------------------------------
	// �O���[�o���ݒ�̍X�V

	m_combinedGlobalParams = m_globalParams;

	// �e�ݒ肪����ꍇ�͐e�̃O���[�o���ݒ���|�����킹��
	if (parentParams != NULL) {
		m_combinedGlobalParams.Multiply(parentParams->m_combinedGlobalParams);
	}


	//---------------------------------------------------------
	// �e�T�u�Z�b�g�ݒ�̍X�V

	if (m_combinedSubsetParams.GetCount() == 1) {
		m_combinedSubsetParams[0] = m_combinedGlobalParams;	// �T�u�Z�b�g���� 1 �̎��̓������g�p�ʂ��������邽�� m_globalParams �������g�p����B
	}
	else
	{
		for (int i = 0; i < m_combinedSubsetParams.GetCount(); i++) {
			m_combinedSubsetParams[i] = m_subsetParams[i];
			m_combinedSubsetParams[i].Multiply(m_combinedGlobalParams);	// �O���[�o���ݒ���|�����킹��
		}
	}
}

} // namespace Scene
} // namespace Lumino
