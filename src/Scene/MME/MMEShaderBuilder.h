
#pragma once

#include "MMETypes.h"

namespace Lumino
{
namespace Scene
{
class MMEShader;
class MMEShaderErrorInfo;

/// MMEShaderBuilder
///		���̃N���X�� MMEShader �̐ÓI�ȃf�[�^�̍\�z���s���B
///		�E�ϐ��̗v���f�[�^�A�����̌���
///		�E�e�N�j�b�N�̉��
///		�E�e�N�j�b�N�A�p�X�̃X�N���v�g�̃R�}���h��
///		�e�N�X�`�������\�[�X�̊m�ۂ͂��̃N���X�ł͍s��Ȃ��B
///		���Ƀ����_�����O�^�[�Q�b�g�̓T�C�Y��{���w��ō쐬���Ă���ꍇ�̓o�b�N�o�b�t�@�T�C�Y���ς������Ċm�ۂ���ׂ��Ȃ̂ŁA
///		���s���Ɏ��Ԃ� (�p�ɂɂł͂Ȃ���) �ς�邱�Ƃ����蓾��B�������������\�[�X�́u���I�v�ƈʒu�t���Ă���B
///		�܂��A���^�f�[�^�̉�͏����ƃ��\�[�X�m�ۂ̏��������킹��Ƒ����ȃR�[�h�ʂɂȂ�̂ŁA�S���𕪂��邱�ƂŊȗ�����}���Ă���B
class MMEShaderBuilder
{
public:
	static MMEShader* Create(SceneGraphManager* manager, Graphics::Shader* coreShader, MMEShaderErrorInfo* errorInfo);

private:
	MMEShaderBuilder(SceneGraphManager* manager, Graphics::Shader* coreShader, MMEShaderErrorInfo* errorInfo);
	~MMEShaderBuilder();

	void Build();

	///	�V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
	///		@param[out] script_output		: STANDARDSGLOBAL �� ScriptOutput �̒l���i�[����ϐ��̃A�h���X (��� "color")
	///		@param[out] script_class		: �G�t�F�N�g�t�@�C���̎g�p�ړI (�f�t�H���g�� "object")
	///		@param[out] script_order		: �G�t�F�N�g�t�@�C���̎��s�^�C�~���O (�f�t�H���g�� "standard")            
	///		@param[out] is_controllobject	: �L���� CONTROLOBJECT �Z�}���e�B�N�X�̏ꍇ�Atrue ���i�[�����
	///		script_output_�Ascript_class_�Ascript_order_ �� STANDARDSGLOBAL ��
	///		����ꍇ�̂ݐݒ肳���B���̊֐��̌Ăяo������ sv_->Request == MME_VARREQ_STANDARDSGLOBAL ��
	///		�`�F�b�N��A�ϐ��̒��g���m�F���邱�ƁB
	static void CheckVariableRequest(
		Graphics::ShaderVariable* var,    // TODO: const��������
		MMEShaderVariable* sv,
		MMEScriptOutput* script_output,
		MMEScriptClass* script_class,
		MMEScriptOrder* script_order,
		bool* is_controllobject);

	/// �Z�}���e�B�N�X���ɑΉ����� MME�Z�}���e�B�N�X ���擾����
	static MMESemantic GetMMESemanticBySemanticName(const String& name);

	/// �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Light" �̏ꍇ�� fales�A����ȊO("Camera") �� true ��Ԃ�
	///		TODO: �r���[�ƃv���W�F�N�V�����s��̎��_�����߂邽�߂Ɏg���B�K���ǂ��炩�K�v�Ȃ̂ŁA�ȗ�����Ă������͌x������ׂ������H
	static bool CheckAnnotationCameraOrLight(Graphics::ShaderVariable* var);

	/// �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Geometry" �̏ꍇ�� 1�A"Light" �̏ꍇ�� 2�A����ȊO�� 0 ��Ԃ�
	static int CheckAnnotationGeometryOrLight(Graphics::ShaderVariable* var);

	/// ���O���w�肵�ăA�m�e�[�V��������������B������Ȃ���� NULL ��Ԃ��B (�啶���������̋�ʖ����BIShaderVariable ����͑召��������ʂ���ł������Ȃ��̂ŗp��)
	static Graphics::ShaderVariable* GetAnnotationByName(Graphics::ShaderVariable* var, const TCHAR* name);

	/// �v�����ڂ���A�v�Z����K�v�̂���s��}�X�N�̃r�b�g����쐬���ĕԂ�
	static uint32_t CheckMatrixMask(MMEVariableRequest req);

private:

	MMEShader*				m_mmeShader;
	MMEShaderErrorInfo*		m_errorInfo;

};

} // namespace Scene
} // namespace Lumino
