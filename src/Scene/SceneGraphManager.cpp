/*
	[2015/5/11]	�V�F�[�_����̃p�t�H�[�}���X���P��
		�E�V�F�[�_�ϐ��� Scene �P�ʃf�[�^�ANode �P�ʃf�[�^�̂悤�ɃO���[�v������
			���͑S�Ă̕ϐ���1�̔z��Ŏ����Ă���A�T�u�Z�b�g�P�ʂ̕`�悵�����Ƃ��� Scene �P�ʂ̕ϐ��� for �Ō��ɍs���Ă���B

		�ECamera �P�ʃf�[�^�X�V�őS�ẴV�F�[�_���X�V���Ă��邪�A�{���ɕK�v�Ȃ̂����ɂ���B
			3D �V�[���̕`��Ȃ̂ɁA2D �̕����X�V����͖̂��ʁB�t���R��B����ł� DrawingLayer �̐��������ʂȍX�V������B
			��q�́u���r�W�����v�Ƒg�ݍ��킹�āANode �P�ʃf�[�^�X�V�Ɠ����^�C�~���O�ōŐV�ɍX�V�ł���Ɩ��ʂ������Ȃ�B

		�u���r�W�����v�c
			�f�[�^���X�V���ꂽ�Ƃ��A�t���[���X�V�񐔂Ɗ֘A����I�u�W�F�N�g(�Ⴆ�΁���Camera�|�C���^) ���L�[�Ƃ��āA
			���̃f�[�^���}�[�N����B���ɍX�V�������Ă΂ꂽ�Ƃ��A���̃L�[����v���Ă���΍X�V���Ȃ��B
			�x���]���̍l���ɋ߂������B
			�E�{���ɕK�v�ɂȂ����Ƃ��ɍX�V����B
			�E�X�V����f�[�^�̌��������ꍇ�͍X�V���Ȃ��B


	[2015/5/11]
		�c���[�\���̎Q�Ɗ֌W�ɂ��āc

		�e���q�̏ꍇ	������ɂ���
			�E�q�̓��[�U�[�� Release ���Ă����������Ȃ��BRemoveChild() �𖾎��I�ɌĂяo���K�v������B
			�E�e�� Release �����炷��������B�q�̓��[�U�[�� Release ����܂ŏ����Ȃ��B
			�� �q�͏�ɐe�� NULL �ɂȂ邱�Ƃ��l������K�v������B�e�́A������Ƃ��Ɏq.Parent �� NULL ���i�[����B

		�e���q�̏ꍇ
			�E�e�͑S�Ă̎q�� Release ���Ȃ��Ə����Ȃ��B
			�� �G�f�B�^�A�v���ŃV�[���O���t��ҏW����Ƃ��A�m�[�h�̍폜�����̂������ʓ|�ɂȂ�B
			�q�����m�[�h���폜����Ƃ��͑S�Ă̎q�𑖍����� Release ���Ȃ���΂Ȃ�Ȃ��B

	[2015/5/10] MMM �G�t�F�N�g�ׂ̍����d�l����
		�E�����̃e�N�j�b�N���p�X�̏����Ƀ}�b�`���Ă��A�g�p�����̂̓t�@�C����Ő�ɒ�`����Ă���e�N�j�b�N1�̂݁B
		�ESubset �A�m�e�[�V�������w�肳��Ă���ꍇ�ł����̂Ƃ���B
		  �T�u�Z�b�g�͈͂��d�Ȃ�2�̃e�N�j�b�N�������Ă��A�g�p�����̂͐�ɒ�`����Ă�����́B


	[2015/5/7]
		�EMME �V�F�[�_�ŁACONTROLOBJECT �� name �A�m�e�[�V������ "(OffscreenOwner)" �ł���V�F�[�_�͋��L�ł��Ȃ��B

		�E�f�t�H���g���C�g�� ViewProjMatrix ���́A�m�[�h�ɉe������ [0] �̃��C�g���g�p����B
		  �Ȃ����Ƃ����ƁA���������f�t�H���g���C�g�͕`���Ԃ��Ƃ�1���݂���ׂ��ŁA�V�[���S�̂�
		  �O���[�o���v�f�ł͂Ȃ����߁B

	[2015/5/6]
		�ȑO�� SceneNode ����������A�f�t�H���g�Œǉ��������W��Ԃ��w�肵�ăf�t�H���g�V�F�[�_�����蓖�ĂĂ����B
		����͂���̓i�V�BSetShader() ���Ȃ����� SceneNode �� Shader �������Ƃ͂Ȃ��B
		�`�掞�� RenderPass �Ƀf�t�H���g�̃V�F�[�_���������A������g�p����B


	�`��̗���
		
		�E�g�b�v���x���BViewPane::Render() ����т����ΑS�ẴV�[�����`�悳���C���[�W�B(�A�^�b�`����Ă���J�����ɂ�邪)
			ViewPane::Render()
				for Layer::PreRender()
				for Layer::Render()
				for Layer::PostRender()

			DrawingLayer::Render()
				SceneGraph::Render(camera, rootNode)

			SceneGraph::Render()
				������J�����O
				���C�g�̃\�[�g (�e�m�[�h�ɋ߂�4������U��)
				for RenderPass::Render(rootNode)

			RenderPass::Render()
				�m�[�h���ċA�I�ɕ`��


		�ERenderPass
			�O�܂ł� ContextDrawParam �� OffscreenScene �ɑ�����́B
			OffscreenScene �����łȂ��A�x���V�F�[�f�B���O�Ȃ񂩂� 1���̊G��`���̂�
			�m�[�h�c���[��������`�悷��̂����ʁB
			���̎��A1��ڂ͖@����`�悷��A2��ڂ͐[�x��`�悷��A3��ڂ̓X�y�L�����A4��ڂ̓e�N�X�`���c ���A
			�`�悲�ƂɎg�p����V�F�[�_���قȂ�B




		�EViewPane ���Ƃɍs������
			�EZ�\�[�g
			�E������J�����O
*/

#include "../Internal.h"
#include <Lumino/Imaging/BitmapPainter.h>
#include <Lumino/Graphics/Color.h>
#include "ViewPane.h"
#include "SceneNode.h"
#include "SceneGraphManager.h"

// TODO: �ړ�
#include "Camera.h"
#include "Layer.h"
#include "MME/MMERenderingPass.h"
#include "InfomationRenderingPass.h"

namespace Lumino
{
namespace Scene
{
	
//=============================================================================
// SceneGraphManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphManager::SceneGraphManager(const ConfigData& configData)
	: m_fileManager(configData.FileManager, true)
	, m_physicsManager(configData.PhysicsManager, true)
	, m_graphicsManager(configData.GraphicsManager, true)
	//, m_rootNode(NULL)
	, m_time(0.0)
	, m_defaultRoot(NULL)
	, m_default3DRoot(NULL)
	, m_default2DRoot(NULL)
	, m_default3DCamera(NULL)
	, m_default2DCamera(NULL)
	, m_mmdViewPane(NULL)
	, m_default3DLayer(NULL)
	, m_default2DLayer(NULL)
{
	memset(m_mmdRenderingPasses, 0, sizeof(m_mmdRenderingPasses));

	m_geometryRenderer.Attach(Graphics::GeometryRenderer::Create(m_graphicsManager), false);

	// �_�~�[�e�N�X�`��
	m_dummyTexture.Attach(Graphics::Texture::Create(Size(32, 32)));
	Imaging::BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Graphics::Color::White);
	m_dummyTexture->Unlock();

	// RenderingPass �Ɋ��蓖�Ă� ID
	for (int i = 0; i < RenderingPass::MaxRenderingPass; ++i) {
		m_renderingPassIDStack.Push(i);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphManager::~SceneGraphManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::CreateMMDSceneGraph()
{
	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Create(this);

	m_default3DRoot = LN_NEW SceneNode();
	m_default3DRoot->Create(this);
	m_defaultRoot->AddChild(m_default3DRoot);

	m_default2DRoot = LN_NEW SceneNode();
	m_default2DRoot->Create(this);
	m_defaultRoot->AddChild(m_default2DRoot);

	m_default3DCamera = LN_NEW Camera(this, CameraProjection_3D);
	m_default3DRoot->AddChild(m_default3DCamera);

	m_default2DCamera = LN_NEW Camera(this, CameraProjection_2D);
	m_default2DRoot->AddChild(m_default2DCamera);
	
	m_default3DLayer = LN_NEW DrawingLayer(this);
	m_default3DLayer->SetCamera(m_default3DCamera);
	m_default3DLayer->SetRenderingRootNode(m_default3DRoot);
	
	m_default2DLayer = LN_NEW DrawingLayer(this);
	m_default2DLayer->SetCamera(m_default2DCamera);
	m_default2DLayer->SetRenderingRootNode(m_default2DRoot);

	m_mmdViewPane = LN_NEW ViewPane(this);
	m_mmdViewPane->GetLayerList()->Add(m_default3DLayer);
	m_mmdViewPane->GetLayerList()->Add(m_default2DLayer);

	//m_mmdRenderingPasses[MMD_PASS_zplot] = LN_NEW MMERenderingPass(this, MMD_PASS_zplot);
	//m_mmdRenderingPasses[MMD_PASS_shadow] = LN_NEW MMERenderingPass(this, MMD_PASS_shadow);
	//m_mmdRenderingPasses[MMD_PASS_edge] = LN_NEW MMERenderingPass(this, MMD_PASS_edge);
	m_mmdRenderingPasses[MMD_PASS_object] = LN_NEW MMERenderingPass(this, MMD_PASS_object);
	//m_mmdRenderingPasses[MMD_PASS_object_ss] = LN_NEW MMERenderingPass(this, MMD_PASS_object_ss);
	m_mmdRenderingPasses[MMD_PASS_Infomation] = LN_NEW InfomationRenderingPass(this);

	m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_object]);
	m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_Infomation]);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::ReleaseMMDSceneGraph()
{
	for (int i = 0; i < MMD_PASS_Max; i++) {
		LN_SAFE_RELEASE(m_mmdRenderingPasses[i]);
	}

	LN_SAFE_RELEASE(m_default3DLayer);
	LN_SAFE_RELEASE(m_default2DLayer);
	LN_SAFE_RELEASE(m_defaultRoot);
	LN_SAFE_RELEASE(m_default3DCamera);
	LN_SAFE_RELEASE(m_default2DCamera);
	LN_SAFE_RELEASE(m_default2DRoot);
	LN_SAFE_RELEASE(m_default3DRoot);
	LN_SAFE_RELEASE(m_mmdViewPane);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneNode* SceneGraphManager::FindNodeFirst(const String& name)
{
	NodeNameMap::iterator itr = m_nodeNameMap.find(name);
	if (itr != m_nodeNameMap.end()) {
		return itr->second;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::UpdateFrame(float elapsedTime)
{
	m_elapsedTime = elapsedTime;
	m_time += elapsedTime;
	m_defaultRoot->UpdateFrameHierarchy(NULL, &m_renderingNodeList);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::OnNodeRename(SceneNode* node, const String& oldName, const String& newName)
{
	// �����Â��O������΃��l�[�����ꂽ�Ƃ������ƁB��x map �����菜��
	if (!oldName.IsEmpty())
	{
		std::pair<NodeNameMap::iterator, NodeNameMap::iterator> range = m_nodeNameMap.equal_range(oldName);
		for (NodeNameMap::iterator itr = range.first; itr != range.second; ++itr) {
			if (itr->second == node) {
				m_nodeNameMap.erase(itr);
			}
		}
	}

	// �V�������O�� map �ɒǉ�
	if (!newName.IsEmpty()) {
		m_nodeNameMap.insert(NodeNamePair(newName, node));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::AddRenderingPass(RenderingPass* pass)
{
	// ID ���蓖��
	pass->m_internalEntryID = m_renderingPassIDStack.GetTop();
	m_renderingPassIDStack.Pop();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::RemoveRenderingPass(RenderingPass* pass)
{
	// ID �ԋp
	m_renderingPassIDStack.Push(pass->m_internalEntryID);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::AddShader(MMEShader* shader)
{
	m_sceneShaderList.Add(shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::RemoveShader(MMEShader* shader)
{
	m_sceneShaderList.Remove(shader);
}


} // namespace Scene
} // namespace Lumino
