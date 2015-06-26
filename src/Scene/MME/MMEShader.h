
#pragma once

#include "../Common.h"
#include "MMETypes.h"

namespace Lumino
{
namespace Scene
{
class MMEShaderErrorInfo;
class MMEShaderVariable;
class MMEShaderTechnique;
	
/// MMEShader
class MMEShader
	: public RefObject
{
public:
	static const int MaxAffectLights = 4;	///< �e�����󂯂郉�C�g�̍ő吔 (MMM �� 3)

	/// �쐬
	static MMEShader* Create(const char* code, int codeLength, MMEShaderErrorInfo* errorInfo, SceneGraphManager* manager);

public:

	/// Graphics::Shader �̎擾
	Graphics::Shader* GetCoreShader() const { return m_coreShader; }

	/// �ݒ肷��K�v�����郉�C�g�̐��̎擾
	int GetRequiredLightCount() const { return m_requiredLightCount; }

	/// Scene �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	void UpdateSceneParams(const MMESceneParams& params, SceneGraphManager* scene);

	/// Camera �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	void UpdateCameraParams(Camera* camera, const SizeF& viewPixelSize);

	/// Node �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	void UpdateNodeParams(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList);

	/// Subset �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	void UpdateSubsetParams(const VisualNodeSubsetParams& params);

	/// CONTROLLOBJECT �ݒ� (obj �� NULL �\�B���݂��Ȃ���� bool �^�ϐ��� false ��ݒ肷�邽��)
	static void SetControllObjectParam(MMEShaderVariable* sv, const IMMESceneObject* obj);

	/// WORLD�AVIEW�APROJECTION�AWORLDVIEW�AVIEWPROJECTION�AWORLDVIEWPROJECTION
	// �I�u�W�F�N�g�̃��[���h�s��ƁA�J�����܂��̓��C�g�̃r���[�E�v���W�F�N�V�����s������߂�B
	// outMatrix �ɒl���i�[�����ꍇ�� true ��Ԃ��B
	// node �̌����ς݃O���[�o���s����쐬������ŌĂяo�����ƁB
	static bool GetGeometryTransform(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList, MMEVariableRequest req, int lightIndex, Matrix* outMatrix);

	/// �w�肵���p�X�Ɉ�v����e�N�j�b�N����������
	MMEShaderTechnique* FindTechnique(MMDPass mmdPass, bool UseTexture, bool UseSphereMap, bool UseToon, bool UseSelfShadow, int subsetIndex);


private:
	MMEShader(SceneGraphManager* manager);
	virtual ~MMEShader();

	typedef Array<MMEShaderVariable*>	MMEShaderVariableList;
	typedef Array<MMEShaderTechnique*>	MMEShaderTechniqueList;

	SceneGraphManager*		m_manager;

	// �ȉ��AMMEShaderBuilder�@���璼�ڃZ�b�g�����f�[�^
	friend class MMEShaderBuilder;
	RefPtr<Graphics::Shader>	m_coreShader;
	MMEScriptOutput				m_mmeScriptOutput;
	MMEScriptClass				m_mmeScriptClass;
	MMEScriptOrder				m_mmeScriptOrder;
	MMEShaderVariableList		m_mmeShaderVariableList;
	MMEShaderVariableList		m_controlObjectVariables;	///< "CONTROLOBJECT" ��v�����Ă���ϐ����X�g
	uint32_t					m_worldMatrixCalcMask;		///< MMEWorldMatrixCalcFlags �̑g�ݍ��킹 (������������g��Ȃ�����)
	int							m_requiredLightCount;		///< ���̃V�F�[�_���K�v�Ƃ��Ă��郉�C�g�̐� (���C�g��v������ϐ��̂����A��ԑ����v�f��)
	MMEShaderTechniqueList		m_mmeShaderTechniqueList;
};

} // namespace Scene
} // namespace Lumino
