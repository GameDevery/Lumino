
#pragma once

//#include "../../Graphics/Interface.h"
//#include "../../Graphics/DeviceObjects.h"
//#include "../DrawCommand.h"
//#include "MMETypes.h"
//#include "SceneShader.h"
#include <Lumino/Base/Nullable.h>
#include "MMETypes.h"
#include "ShaderScriptCommandList.h"

namespace Lumino
{
namespace Scene
{
class MMEShader;
class MMEShaderPass;
class MMEShaderErrorInfo;

/**
	@note	
		�A�m�e�[�V����
			string Subset		e.g) "0-4,6,8"
			string MMDPass		"object" or "object_ss" or "shadow" or "edge"
			bool UseTexture
			bool UseSphereMap
			bool UseToon
			bool UseSelfShadow	[MMM]
			string Script

			MMDPass �� "object_ss" �� MMM �ł͎g�p�񐄏��B
*/
class MMEShaderTechnique
	: public RefObject
{
public:
	MMEShaderTechnique();
	virtual ~MMEShaderTechnique();

public:

	/// ������
	void Initialize(MMEShader* shader, Graphics::ShaderTechnique* tech, MMEShaderErrorInfo* errorInfo);

	/// ���̃e�N�j�b�N�����V�F�[�_���擾����
	MMEShader* GetOwnerShader() const { return m_ownerShader; }

	/// �`��p�X���擾����
	MMDPass GetMMDPass() const { return m_mmdPass; }

	/// �e�N�X�`�����g�p���Ă���ގ��݂̂�ΏۂƂ��邩�ǂ���
	const Nullable<bool>& IsUseTexture() const { return m_isUseTexture; }

	/// �X�t�B�A�}�b�v���g�p���Ă���ގ��݂̂�ΏۂƂ��邩�ǂ���
	const Nullable<bool>& IsUseSphereMap() const { return m_isUseSphereMap; }

	/// ���f���݂̂�ΏۂƂ��邩�ǂ���
	const Nullable<bool>& IsUseToon() const { return m_isUseToon; }

	/// [MMM] �Z���t�V���h�E���g�p���Ă���ގ��݂̂�ΏۂƂ��邩�ǂ���
	const Nullable<bool>& IsUseSelfShadow() const { return m_isUseSelfShadow; }

	/// ���̃e�N�j�b�N�͎w�肵���ԍ��̃T�u�Z�b�g��`��ł��邩
	bool ContainsSubsetIndex(int subsetIndex) const;
	
	/// �`��R�}���h���X�g�̎擾
	ShaderScriptCommandList& GetDrawingCommandList() { return m_scriptCommandList; }

private:

	/// MMEShaderPass ����������
	MMEShaderPass* FindMMEShaderPass(const TCHAR* passName);

	/// �X�N���v�g������̕ϐ����󂯎���� commandList �ɃR�}���h��ǉ����Ă���
	//      thisPass : �p�X���̃X�N���v�g����͂���ꍇ�A���̃p�X��n�� (Draw="Geometry" �̎��s�ɓn��pass)
	void ParseScriptCommandString(Graphics::ShaderVariable* scriptAnno, Graphics::ShaderPass* thisPass, ShaderScriptCommandList* commandList);


	static void ParseSubsetIndexArray(const String& subsetText, ArrayList<bool>* subsetIndexArray, bool* endToRange);

	/// �`��T�u�Z�b�g�����������񂩂�A�T�u�Z�b�g�ԍ��̔z����쐬����
	///		'-' �ɂ��͈͎w��Łux�Ԉȍ~���ׂāv��`�悷��ꍇ�A�z��̏I�[�ɂ� -1 ���i�[�����B
	static void ParseSubsetIndexArray(const TCHAR* subsetText, ArrayList<int>* subsetIndexArray);

private:

	typedef ArrayList< std::pair<String, ShaderScriptCommandList> >	PassScriptCommandList;

	MMEShaderErrorInfo*			m_errorInfo;
	MMEShader*					m_ownerShader;
	Graphics::Shader*			m_coreShader;
	Graphics::ShaderTechnique*	m_technique;

	MMDPass						m_mmdPass;
	Nullable<bool>				m_isUseTexture;
	Nullable<bool>				m_isUseSphereMap;
	Nullable<bool>				m_isUseToon;
	Nullable<bool>				m_isUseSelfShadow;
	ArrayList<bool>				m_subsetIndexArray;
	bool						m_subsetIndexEndToRange;

	ShaderScriptCommandList		m_scriptCommandList;	///< ���̃e�N�j�b�N�� Script �y�юq�p�X�� Script
	ArrayList<MMEShaderPass*>	m_mmeShaderPasses;
};


class MMEShaderPass
	: public RefObject
{
public:
	Graphics::ShaderPass*	m_pass;
	ShaderScriptCommandList	m_scriptCommandList;
};

} // namespace Scene
} // namespace Lumino
