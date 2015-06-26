
#pragma once

#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include <Lumino/Graphics/Shader.h>
#include "MMETypes.h"

namespace Lumino
{
namespace Scene
{
class MMEShader;

//class IMMESubsetRenerer
//{
//public:
//	virtual void OnDrawSubset(int subsetIndex, void* userData) = 0;
//};

/**
	@note	Script �������I�Ɏw�肳��Ă��Ȃ��ꍇ�́A
			�ETechnique	�̏ꍇ �c �S�Ă̎q Pass �̃R�}���h���X�g�����s����
			�EPass �̏ꍇ �c "Draw=Geometry" �����s����

			Technique �� Pass ���́A�q�R�}���h���X�g�����݂��邩�Ŕ��ʂ����B
*/
class ShaderScriptCommandList
	: public RefObject
{
public:

	struct DrawParams
	{
		//Graphics::Renderer*			Renderer;
		//Graphics::GeometryRenderer*	GeometryRenderer;
		RenderingParams*			Params;
		//int							SubsetCount;
		SceneNode*					RenderingNode;
		int							SubsetIndex;
		//IMMESubsetRenerer*			SubsetRenderer;
		//void*						UserData;
		//const Array<int>*			SubsetNumbers;	///< �e����n���ꂽ�`�悷��ׂ��T�u�Z�b�g�ԍ����X�g (�����p�B�O������� NULL ��n������)
	};

	enum ValidationError
	{
		ValidationError_Success = 0,
		ValidationError_InvalidRenderTargetIndex,			///< �����_�����O�^�[�Q�b�g�̃C���f�b�N�X���s��
		ValidationError_ScriptExternal_Color_InvalidCall,	///< "ScriptExternal=Color" �� postscript �ł͂Ȃ��V�F�[�_����Ăяo���ꂽ
		ValidationError_ScriptExternal_Color_MultipleCall,	///< "ScriptExternal=Color" �� 2��ȏ�Ăяo���ꂽ�B
		ValidationError_ScriptExternal_Color_CallInLoop,	///< "ScriptExternal=Color" �� Loop ���ŌĂяo���ꂽ�B
		ValidationError_Pass_CallByPass,					///< "Pass" ���p�X��ŌĂяo���ꂽ�B
		ValidationError_Loop_CallByPass,					///< "LoopByCount" "LoopEnd" "LoopGetIndex" �� �p�X��ŌĂяo���ꂽ�B
		ValidationError_LoopBlockUnMath,					///< "LoopByCount" �` "LoopEnd" �̃y�A������Ȃ�
		ValidationError_DrawGeometry_CallByTechnique,		///< "DrawGeometry" �� �e�N�j�b�N��ŌĂяo���ꂽ�B
		ValidationError_DrawGeometry_InvalidCall,			///< "DrawGeometry" �� "scene" �V�F�[�_����Ăяo���ꂽ�B 
		ValidationError_DrawBuffer_CallByTechnique,			///< "DrawBuffer" �� �e�N�j�b�N��ŌĂяo���ꂽ�B
		ValidationError_DrawBuffer_InvalidCall,				///< "DrawBuffer" �� "object" �V�F�[�_����Ăяo���ꂽ�B 
	};

public:
	ShaderScriptCommandList();
	~ShaderScriptCommandList();

public:

	/// ������
	void Initialize(MMEShader* ownerShader);

	/// �e�N�j�b�N�̎q�p�X�̃R�}���h�X�g��ǉ�����
	void AddChildPassCommandList(ShaderScriptCommandList* commandList);

	/// �`�悷��T�u�Z�b�g�ԍ��̔z���ݒ肷�� (�e�N�j�b�N�p)
	//void SetDrawingSubsetNumbers(const Array<int>& numbers);

	/// ���̃R�}���h���X�g�����s����
	void Execute(DrawParams& params);

	/// ScriptExternal=Color �܂Ŏ��s����
	void ExecutePreScriptExternal_Color(DrawParams& params);

	/// ScriptExternal=Color ������s����
	void ExecutePostScriptExternal_Color(DrawParams& params);

	/// �X�N���v�g�������������m�F����
	ValidationError CheckValid(MMEScriptClass scriptClass, MMEScriptOrder scriptOrder);


public:

	/// �����_�����O�^�[�Q�b�g��ݒ肷��
	/// (textureVariable �� NULL �ɂ���ƁA�f�t�H���g(�R�}���h�J�n��)�Ƀ��Z�b�g)
	void Add_RenderColorTarget(int index, Graphics::ShaderVariable* textureVariable);

	/// �����_�����O�^�[�Q�b�g��ݒ肷��
	/// (textureVariable �� NULL �ɂ���ƁA�f�t�H���g(�R�}���h�J�n��)�Ƀ��Z�b�g)
	void Add_RenderDepthStencilTarget(Graphics::ShaderVariable* textureVariable);

	/// �����_�����O�^�[�Q�b�g���N���A����F��ݒ肷��
	void Add_ClearSetColor(const Vector4& color);		// TODO: ���̂�����AGraphics::IShaderVariable ���� getVector ����悤�ɕύX

	/// �[�x�o�b�t�@���N���A����Z�l��ݒ肷��
	void Add_ClearSetDepth(float depth);

	/// �����_�����O�^�[�Q�b�g���N���A����
	void Add_ClearColor();

	/// �[�x�X�e���V���T�[�t�F�C�X���N���A����
	void Add_ClearDepth();

	/// ScriptExternal=Color
	void Add_ScriptExternal_Color();

	/// �w�肵���p�X�����s���� 
	/// (���̕`��R�}���h�����s����BcommandList �� NULL �ɂ���ƁADraw=Geometry �݂̂����s����)
	void Add_Pass(Graphics::ShaderPass* pass, ShaderScriptCommandList* commandList);

	/// �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v����
	void Add_LoopByCount(int count);

	/// ���[�v�I�[
	void Add_LoopEnd();

	/// ���[�v���̃��[�v�J�E���^�̒l���A�w�肵���p�����[�^�ɐݒ肷��
	void Add_LoopGetIndex(Graphics::ShaderVariable* variable);

	/// �I�u�W�F�N�g��`�悷��
	void Add_DrawGeometry(Graphics::ShaderPass* pass);

	/// �����_�����O�^�[�Q�b�g�̃X�N���[���ƈ�v����T�C�Y�́A�����`�̃|���S����`�悷��
	void Add_DrawBuffer(Graphics::ShaderPass* pass);

private:

	/// ���̃R�}���h���X�g�����s���� (�߂�l�͍Ō�Ɏ��s�����R�}���h���w���v���O�����J�E���^)
	int InternalExecute(DrawParams& param, int programCounter, int currentLoopCount);

	/// Draw=Geometry
	void DrawGeometry(DrawParams& param, Graphics::ShaderPass* pass);

	/// ���݂̃����_�����O�^�[�Q�b�g���̏�Ԃ��L��
	void PushCurrentState(DrawParams& param);

	/// �L��������Ԃ𕜋A
	void PopCurrentState(DrawParams& param);

private:

	/// �`��R�}���h�̎��
	enum CommandType
	{
		COMMAND_Unknown = 0,
		COMMAND_RenderColorTarget,
		COMMAND_RenderDepthStencilTarget,
		COMMAND_ClearSetColor,
		COMMAND_ClearSetDepth,
		COMMAND_ClearColor,
		COMMAND_ClearDepth,
		COMMAND_ScriptExternal_Color,
		COMMAND_Pass,
		COMMAND_LoopByCount,
		COMMAND_LoopEnd,
		COMMAND_LoopGetIndex,
		COMMAND_DrawGeometry,
		COMMAND_DrawBuffer,
	};

	/// �`��R�}���h
	struct Command
	{
		CommandType		Type;
		union
		{
			struct
			{
				int							Index;
				Graphics::ShaderVariable*	TextureVariable;	///< �����_�����O�^�[�Q�b�g��ێ����Ă���V�F�[�_�ϐ�
			} RenderColorTarget;

			struct
			{
				Graphics::ShaderVariable*	TextureVariable;	///< �[�x�o�b�t�@��ێ����Ă���V�F�[�_�ϐ�
			} RenderDepthStencilTarget;

			struct
			{
				float						Color[4];
			} ClearSetColor;

			struct
			{
				float						Depth;
			} ClearSetDepth;

			struct
			{
				Graphics::ShaderPass*		Pass;
				ShaderScriptCommandList*		CommandList;
			} Pass;

			struct
			{
				int							Count;
			} LoopByCount;

			struct
			{
				Graphics::ShaderVariable*	Variable;			///< ���[�v�񐔂��i�[����V�F�[�_�ϐ�
			} LoopGetIndex;

			struct
			{
				Graphics::ShaderPass*		Pass;
			} DrawGeometry;

			struct
			{
				Graphics::ShaderPass*		Pass;
			} DrawBuffer;
		};
	};

	typedef Array<Command>					CommandArray;
	typedef Array<ShaderScriptCommandList*>	CommandListArray;

private:
	MMEShader*					m_ownerShader;
	CommandArray				m_commandArray;
	//Array<int>				m_drawingSubsetNumbers;
	CommandListArray			m_childCommandListArray;
	ShaderScriptCommandList*		m_parentList;					///< Pass �̏ꍇ�A�e�e�N�j�b�N�̃R�}���h���X�g���w��
	int							m_scriptExternalColorPosition;	///< ScriptExternal=Color �̂���v�f�ԍ�
	Graphics::ColorF			m_clearColor;					///< "ClearSetColor=" �Őݒ肳�ꂽ�l
	float						m_clearDepth;					///< "ClearSetDepth=" �Őݒ肳�ꂽ�l

	Graphics::Texture*			m_oldRenderTarget[Graphics::Renderer::MaxMultiRenderTargets];
	Graphics::Texture*			m_oldDepthBuffer;
	Graphics::ColorF			m_oldClearColor;
	float						m_oldClearDepth;

};

} // namespace Scene
} // namespace Lumino
