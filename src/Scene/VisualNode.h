
#pragma once

#include <Lumino/Graphics/Texture.h>
#include "MME/MMEShaderTechnique.h"
#include "SceneNode.h"
#include "VisualNodeParams.h"

namespace Lumino
{
namespace Scene
{

/// VisualNode
class VisualNode
	: public SceneNode
{
public:
	VisualNode();
	virtual ~VisualNode();

public:

	/// ����Ԃ̐ݒ�
	void SetVisible(bool visible) { m_isVisible = visible; }

	/// ����Ԃ̔���
	bool IsVisible() const { return m_isVisible; }

	/// �s�����x�̐ݒ� (0.0 �` 1.0)
	void SetOpacity(float opacity, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).Opacity = opacity; }

	/// �s�����x�̎擾
	float GetOpacity(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).Opacity; }

	/// ��Z�F�̐ݒ�
	void SetColorScale(const Graphics::ColorF& color, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale = color; }

	/// ��Z�F�̎擾
	const Graphics::ColorF& GetColorScale(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale; }

	/// �u�����h�J���[�̐ݒ�
	void SetBlendColor(const Graphics::ColorF& color, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).BlendColor = color; }

	/// �u�����h�J���[�̎擾
	const Graphics::ColorF& GetBlendColor(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).BlendColor; }

	/// �F���̐ݒ�
	void SetTone(const Graphics::Tone& tone, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).Tone = tone; }

	/// �F���̎擾
	const Graphics::Tone& GetTone(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).Tone; }

	/// UV �ϊ��s��̐ݒ�
	void SetUVTransform(const Matrix& matrix, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform = matrix; }

	/// UV �ϊ��s��̐ݒ�
	const Matrix& GetUVTransform(int subsetIndex = -1)  const { return m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform; }

	/// �V�F�[�_�̐ݒ�
	void SetShader(MMEShader* shader, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader = shader; }

	/// �V�F�[�_�̎擾
	MMEShader* GetShader(int subsetIndex = -1) { return m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader; }

	/// �������@�̐ݒ�
	void SetBlendMode(Graphics::BlendMode mode) { m_renderState.Blend = mode; }

	/// �������@�̎擾
	Graphics::BlendMode GetBlendMode() const { return m_renderState.Blend; }

	/// �[�x�e�X�g�̗L���ݒ�
	void SetEnableDepthTest(bool flag) { m_renderState.DepthTest = flag; }

	/// �[�x�e�X�g�̗L������
	bool IsEnableDepthTest() const { return m_renderState.DepthTest; }

	/// �[�xWrite�̗L���ݒ�
	void SetEnableDepthWrite(bool flag) { m_renderState.DepthWrite = flag; }

	/// �[�x�������݂̗L������
	bool IsEnableDepthWritet() const { return m_renderState.DepthWrite; }

	/// �J�����O���@�̐ݒ�
	void SetCullingMode(Graphics::CullingMode mode) { m_renderState.Culling = mode; }

	/// �J�����O���@�̎擾
	Graphics::CullingMode GetCullingMode() const { return m_renderState.Culling; }

public:

	/// �쐬
	void Create(SceneGraphManager* manager, int subsetCount);

	const Graphics::RenderState& GetRenderState() const { return m_renderState; }
	int GetSubsetCount() const { return m_subsetCount; }
	const VisualNodeParams& GetVisualNodeParams() const { return m_visualNodeParams; }

	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_VisualNode; }
	virtual void UpdateFrameHierarchy(SceneNode* parent, SceneNodeList* renderingNodeList);
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList);
	virtual void UpdateAffectLights(LightNodeList* renderingLightList, int maxCount);

	/// �m�[�h�P�ʂ̕`����̍X�V�B���̌シ����A�̃T�u�Z�b�g�`�悪�n�܂�
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) {}


	// IMMESubsetRenerer
//	virtual void OnDrawSubset(int subsetIndex, void* userData, MMEShader* shader, pass) { DrawSubset(*((RenderingParams*)userData), subsetIndex); }

	void DrawSubsetInternal(RenderingParams& params, int subsetIndex, MMEShader* shader, Graphics::ShaderPass* pass);

	virtual void DrawSubset(RenderingParams& params, int subsetIndex) = 0;

	virtual LightNodeList* GetAffectLightList() { return &m_affectLightList; }

	/// ���C�g�\�[�g�p�̔�r�֐� (�����ƗD��x�Ń\�[�g)
	static bool CmpLightSort(const Light* left, const Light* right);

protected:
	int						m_subsetCount;

	friend class RenderingPass;
	VisualNodeParams		m_visualNodeParams;

	// �ȉ��̃v���p�e�B�̓T�u�Z�b�g�P�ʂŊǗ����Ȃ��B
	// ��������A������ݒ�̌p�����l������K�v��������́B
	// �����_�����O�X�e�[�g�Ȃ񂩂̓T�u�Z�b�g�P�ʂŐݒ�ł���悤�ɂ��邱�Ƃ��\�����ǁA
	// ���ۂɃT�u�Z�b�g�P�ʂŐݒ肵�������Ƃ��Ă���́H���čl����ƃm�[�h�P�ʂł����Ǝv���B
	Graphics::RenderState	m_renderState;	///< �����_�����O�X�e�[�g
	bool					m_isVisible;

	LightNodeList			m_affectLightList;
};

} // namespace Scene
} // namespace Lumino
