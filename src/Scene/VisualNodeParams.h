
#pragma once

#include <Lumino/Graphics/Material.h>
#include <Lumino/Graphics/RenderState.h>
#include "MME/MMEShader.h"
#include "SceneNode.h"

namespace Lumino
{
namespace Scene
{
/// �T�u�Z�b�g�P�ʂ̕`��p�����[�^
struct VisualNodeSubsetParams
{
    Graphics::Material		Material;       ///< �`��I�u�W�F�N�g�̃}�e���A��
    float					Opacity;        ///< �s�����x (0.0 �` 1.0) (�V�F�[�_�ɏ�Z�F�𑗂�Ƃ��A���̃��l�ɏ�Z�����)
	Graphics::ColorF		ColorScale;     ///< ��Z�F
	Graphics::ColorF		BlendColor;     ///< �u�����h�F
	Graphics::Tone			Tone;           ///< �F��
    Matrix					UVTransform;    ///< �e�N�X�`�����W�ϊ��s��
	RefPtr<MMEShader>		SceneShader;

	/// �����l
	VisualNodeSubsetParams()
        : Opacity           (1.0f)
        , ColorScale        (1.0f, 1.0f, 1.0f, 1.0f) 
        , BlendColor        (0.0f, 0.0f, 0.0f, 0.0f)
        , Tone              (0.0f, 0.0f, 0.0f, 0.0f)
		, UVTransform		(Matrix::Identity)
		, SceneShader		()
    {}

	void Multiply(const VisualNodeSubsetParams& parent)
	{
		Opacity *= parent.Opacity;
		ColorScale.MultiplyClamp(parent.ColorScale);
		BlendColor.AddClamp(parent.BlendColor);
		Tone.AddClamp(parent.Tone);
		if (SceneShader.IsNull()) {
			SceneShader = parent.SceneShader;
		}
	}
};

/// VisualNodeParams
///		TODO: ���̃N���X�͐̂́A�`��X���b�h�� Scene �ŊǗ����Ă�������̖��c�BVisualNode �ɓ������Ă��悢�B
class VisualNodeParams
{
public:
	VisualNodeParams();
	~VisualNodeParams();

public:

	/// �쐬
	void Create(int subsetCount);

	/// �T�u�Z�b�g�P�ʂ̕`��p�����[�^�̎擾 (-1 ���w�肷��ƃO���[�o���p�����[�^���擾����)
	VisualNodeSubsetParams& GetSubsetParams(int index);
	const VisualNodeSubsetParams& GetSubsetParams(int index) const;

	/// ���ۂɕ`�掞�Ɏg�����̍X�V
	///		parentParams �� NULL �̏ꍇ�͐e����󂯌p���Ȃ��B
	///		�܂��AparentParams �� UpdateSubsetRenderParam() �ς݂ł��邱�ƁB
	void UpdateSubsetRenderParam(const VisualNodeParams* parentParams);

	const VisualNodeSubsetParams& GetCombinedSubsetParams(int index) const { return m_combinedSubsetParams[index]; }

private:
	VisualNodeSubsetParams				m_globalParams;			///< �S�ẴT�u�Z�b�g�ɉe������p�����[�^
	ArrayList<VisualNodeSubsetParams>	m_subsetParams;			///< �e�T�u�Z�b�g�̃p�����[�^
	VisualNodeSubsetParams				m_combinedGlobalParams;	///< �����ς݂́A�S�ẴT�u�Z�b�g�ɉe������p�����[�^
	ArrayList<VisualNodeSubsetParams>	m_combinedSubsetParams;	///< �����ς݂́A�e�T�u�Z�b�g�̃p�����[�^ (UpdateSubsetRenderParam() �Ōv�Z�����)


};

} // namespace Scene
} // namespace Lumino
