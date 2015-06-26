
#pragma once

#include <Lumino/Base/Nullable.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "Common.h"
#include "MME/MMEShader.h"

namespace Lumino
{
namespace Scene
{

class RenderingParams
{
public:
	RenderingPass*				Pass;
	Graphics::Renderer*			Renderer;
	Graphics::GeometryRenderer*	GeometryRenderer;
	Camera*						CurrentCamera;
	MMEShader*					Shader;				///< �{���ɕK�v�ȃV�F�[�_ (VisualNode::Render() �ȉ��Ŏg�p�\)
};

struct RenderingPassClientData
{
	RenderingPass*	OwnerPass;				///< 
	int				PriorityShaderIndex;	///< RenderingPass::m_priorityShaderList �̃C���f�b�N�X (-1 �͈�v�Ȃ�)
};

struct RenderingPriorityParams
{
	MMEShader*		Shader;
	bool			Hide;
};

/// MMD �ł����Ƃ���� MMDPass ("object" ��) �ƁA�I�t�X�N���[��RT�ɑ�������B
/// RenderingPass �͕����̃��C���[�ŋ��L�ł���B�@�\��ǉ�����Ƃ��A
/// �����o�Ɉꎞ�f�[�^���K�v�ɂȂ�ۂ�1��̃����_�����O�Ŋ�������悤�ɒ��ӂ��邱�ƁB
class RenderingPass
	: public RefObject
{
public:
	static const int MaxRenderingPass = 32;		///< 1 �̃V�[���O���t���ō쐬�ł��� RenderingPass �̍ő吔

public:
	struct NodeData
	{
		RenderingPass*	OwnerPass;
		int				PriorityShaderIndex;
	};

public:
	/// passID �́A���̃p�X����̓I�ɉ���`�悷��̂����w�肷��B
	/// 65536 �ȍ~�͗\��ς݂ŁAInternalRenderingPass �񋓌^�ŕ\�����B
	/// �������[�U�[���J�X�^���̃p�X��p�ӂ������ꍇ�͂��� ID ��Ǝ���`���ASceneNode::Render() ����
	/// �J�X�^���̏������������邱�Ƃ�z�肵�Ă���B
	/// ownerShader �� OFFSCREENRENDERTARGET �Ƃ��Ă��̃p�X�����Ƃ��A�������̃V�F�[�_���w�肷��
	RenderingPass(SceneGraphManager* manager/*, int passID, MMEShader* ownerShader = NULL*/);
	virtual ~RenderingPass();

public:

	//int GetPassID() const { return m_passID; }

	MMEShader* GetDefaultShader() { return m_defaultShader; }

	/// ���̃p�X�ŗD��I�Ɏg�p�����V�F�[�_��ǉ�����
	void AddPriorityShader(const String& matchingNameKey, MMEShader* shader, bool isHide);

public:

	/// �w�肵���m�[�h�����̃p�X��ŕ`�悷��
	///		�ȑO�� VisualNode �� Render() ���������Ă������A���̏�����������Ɏ����Ă����B
	///		�ړI�̓m�[�h�P�ʂ̃����_�����O�̗�����p�X���ŃJ�X�^�}�C�Y�ł���悤�ɂ��邱�ƁB
	///		�Ⴆ�΁A���̏����͔ėp�I MME �p�̃p�X�ƂȂ��Ă���A�Q�[�������ɂ͂��I�[�o�[�X�y�b�N�C���B
	///		�����ƃQ�[�������ɃX�}�[�g�������J�X�^���p�X����������Ƃ��A�I�[�o�[���C�h����B
	virtual void RenderNode(RenderingParams& params, SceneNode* node);

	virtual void PostRender(RenderingParams& params) {}


protected:

	struct PriorityParamsEntry
	{
		String					MatchingNameKey;		///< ���̃V�F�[�_�����蓖�Ă�m�[�h�� (���C�h�J�[�h�L��)
		RenderingPriorityParams	Params;
		//RefPtr<MMEShader>	Shader;					///< ���蓖�Ă�V�F�[�_
	};

	typedef Array<PriorityParamsEntry>	PriorityParamsEntryList;

	friend class SceneGraphManager;
	SceneGraphManager*			m_manager;
	int							m_internalEntryID;		///< ���̃p�X�Ɋ��蓖�Ă���ID (0 �` MaxRenderingPass-1)
	RefPtr<Graphics::Texture>	m_renderTarget;			///< ���̃p�X�̕`���ƂȂ郌���_�����O�^�[�Q�b�g (NULL�BMMDPass �� "zplot" �ł���� Z�o�b�t�@���^�[�Q�b�g�ɂȂ�)
	RefPtr<Graphics::Texture>	m_depthBuffer;			///< ���̃p�X�̕`���ƂȂ�[�x�o�b�t�@ (NULL��)
	RefPtr<MMEShader>			m_defaultShader;		///< ���̃p�X�̕`��Ńf�t�H���g�Ƃ��Ďg�p�����V�F�[�_ (�V�F�[�_�������Ă��Ȃ� VisualNode �ɑ΂��Ďg����)
	PriorityParamsEntryList		m_priorityEntryList;	///< ���̃p�X�ŗD��I�Ɏg�p�����V�F�[�_ (�V�F�[�_�������Ă��� VisualNode �ɑ΂��Ă������I�ɂ�������g��)
	Nullable<Graphics::ColorF>	m_clearColor;			///< �p�X�J�n���Ƀ����_�����O�^�[�Q�b�g���N���A����F
	Nullable<float>				m_clearDepth;			///< �p�X�J�n���ɐ[�x�o�b�t�@���N���A����Z�l
};

} // namespace Scene
} // namespace Lumino
