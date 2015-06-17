
#pragma once
#include <map>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "../Modeling/ModelManager.h"
#include "Common.h"

#include "MME/MMETypes.h"	// TODO: ����͕ʂ̏ꏊ�Ɉړ��������E�E�E

namespace Lumino
{
namespace Scene
{

/// SceneGraphManager
class SceneGraphManager
	: public RefObject
{
public:

	/// �������f�[�^
	struct ConfigData
	{
		Lumino::FileManager*		FileManager;
		Physics::PhysicsManager*	PhysicsManager;
		Graphics::GraphicsManager*	GraphicsManager;
		Modeling::ModelManager*		ModelManager;

		ConfigData()
			: FileManager(NULL)
			, PhysicsManager(NULL)
			, GraphicsManager(NULL)
			, ModelManager(NULL)
		{}
	};

public:
	SceneGraphManager(const ConfigData& configData);
	virtual ~SceneGraphManager();

public:

	// TODO: SceneGraphManager �̓z���g�ɒP���ȃV�[���O���t�Ǘ������ɂƂǂ߂Ă����āAMMD �p�̃V�[���\�z�͕ʃN���X�ɂ��Ă���������
	void CreateMMDSceneGraph();
	void ReleaseMMDSceneGraph();
	ViewPane* GetMMDViewPane() { return m_mmdViewPane; }
	SceneNode* GetRootNode() { return m_defaultRoot; }
	SceneNode* Get2DRootNode() { return m_default2DRoot; }
	SceneNode* Get3DRootNode() { return m_default3DRoot; }
	Camera* GetDefault3DCamera() { return m_default3DCamera; }

	


	/// ���O���w�肵�ăm�[�h���������� (�����m�[�h�̏ꍇ�͍ŏ��̃m�[�h)
	SceneNode* FindNodeFirst(const String& name);

	/// �S�Ẵm�[�h���K�w�I�ɍX�V����
	void UpdateFrame(float elapsedTime);

	/// ���݂̎��Ԃ��擾���� (�b)
	double GetTime() const { return m_time; }

	/// �O��t���[������̌o�ߎ��Ԃ��擾���� (�b)
	float GetElapsedTime() const { return m_elapsedTime; }

public:	// internal

	Physics::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	Graphics::GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Modeling::ModelManager* GetModelManager() { return m_modelManager; }
	Graphics::GeometryRenderer* GetGeometryRenderer() { return m_geometryRenderer; }
	Graphics::Texture* GetDummyTexture() { return m_dummyTexture; }

	void OnNodeRename(SceneNode* node, const String& oldName, const String& newName);

	void AddNode(SceneNode* node) { m_allNodes.Add(node); }
	void RemoveNode(SceneNode* node) { m_allNodes.Remove(node); }
	void AddRenderingPass(RenderingPass* pass);
	void RemoveRenderingPass(RenderingPass* pass);
	void AddLight(Light* light);
	void AddShader(MMEShader* shader);
	void RemoveShader(MMEShader* shader);
	SceneShaderList* GetShaderList() { return &m_sceneShaderList; }

	/// �w�肵�����W�ɋ߂����C�g���擾���� (�擾���鐔�� outList �̗v�f���B���炩���� Resize() ���Ă�������)
	void SelectLight(Vector3* pos, LightNodeList* outList);

private:
	typedef std::multimap<String, SceneNode*>	NodeNameMap;
	typedef std::pair<String, SceneNode*>		NodeNamePair;

	RefPtr<FileManager>					m_fileManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	RefPtr<Modeling::ModelManager>		m_modelManager;
	RefPtr<Graphics::GeometryRenderer>	m_geometryRenderer;
	RefPtr<Graphics::Texture>			m_dummyTexture;

	double								m_time;					///< ���ԏ����̊J�n�ʒm����̌o�ߎ��� (�b)
	float								m_elapsedTime;			///< �O��t���[������̌o�ߎ��� (�b)

	SceneNodeList						m_allNodes;

	Stack<int>							m_renderingPassIDStack;	///< (0�`MaxRenderingPass-1)
	NodeNameMap							m_nodeNameMap;			///< �m�[�h�𖼑O�Ō������邽�߂̃}�b�v
	//SceneNode*							m_rootNode;
	LightNodeList						m_lightNodeList;		///< �S�Ă� Light �̃��X�g
	SceneNodeList						m_renderingNodeList;	///< (����Ȃ������H)�Ō�� UpdateFrame() �Œ��o���ꂽ�`��\�ȃm�[�h�̃��X�g
	SceneShaderList						m_sceneShaderList;

	SceneNode*		m_defaultRoot;
	SceneNode*		m_default3DRoot;
	SceneNode*		m_default2DRoot;
	Camera*			m_default3DCamera;
	Camera*			m_default2DCamera;
	ViewPane*		m_mmdViewPane;
	DrawingLayer*	m_default3DLayer;
	DrawingLayer*	m_default2DLayer;
	RenderingPass*	m_mmdRenderingPasses[MMD_PASS_Max];

	
};

} // namespace Scene
} // namespace Lumino
