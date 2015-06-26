
#pragma once

#include <Lumino/Base/String.h>
#include <Lumino/BindingSupport.h>
#include "Common.h"
#include "RenderingPass.h"
#include "MME/MMETypes.h"

namespace Lumino
{
namespace Scene
{

class SceneNodeRefList
	: public RefObjectListBase<SceneNode>
{
public:
	SceneNodeRefList() {}
	virtual ~SceneNodeRefList() {}
};



/// SceneNode
class SceneNode
	: public RefObject
	, public IMMESceneObject
{
public:
	SceneNode();
	virtual ~SceneNode();

public:

	/// �m�[�h���̐ݒ�
	void SetName(const String& name);

	/// �m�[�h���̎擾
	const String& GetName() const { return m_name; }

	/// ���[���h�ϊ��s��̐ݒ�
	void SetTransform(const Matrix& matrix) { m_localMatrix = matrix; m_transformModified = false; }

	/// ���[���h�ϊ��s��̎擾
	const Matrix& GgetMatrix() const { return m_localMatrix; }

	/// �ʒu�̐ݒ�
	void SetPosition(const Vector3& pos) { m_transform.Translation = pos; m_transformModified = true; }

	/// �ʒu�̎擾
	const Vector3& GetPosition() const { return m_transform.Translation; }

	/// ��]�̐ݒ�
	void SetRotation(const Quaternion& rot) { m_transform.Rotation = rot; m_transformModified = true; }

	/// ��]�̎擾
	const Quaternion& GetRotation() const { return m_transform.Rotation; }

	/// �g�嗦�̐ݒ�
	void SetScale(const Vector3& scale) { m_transform.Scale = scale; m_transformModified = true; }

	/// �g�嗦�̎擾
	const Vector3& GetScale() const { return m_transform.Scale; }

	/// ���_�̐ݒ�
	void SsetCenter(const Vector3& center) { m_transformCenter = center; m_transformModified = true; }

	/// ���_�̎擾
	const Vector3& GetCenter() const { return m_transformCenter; }

	///// ����Ԃ̐ݒ�
	//void SetVisible(bool enabled) { m_isVisible = enabled; }

	///// ����Ԃ̔���
	//bool IsVisible() const { return m_isVisible; }

	/// ��]�����̐ݒ�
	void SetRotateOrder(RotationOrder order) { m_rotOrder = order; }

	/// ��]�����̎擾
	RotationOrder GetRotateOrder() const { return m_rotOrder; }

	/// �D��x�̐ݒ� (�����������ɕ`�悳���)
	void SetPriority(int priority) { m_priority = priority; }

	/// �D��x�̎擾
	int GetPriority() const { return m_priority; }

	/// �r���{�[�h��Ԃ̐ݒ�
	void SetBillboardType(BillboardType type) { m_billboardType = type; }

	/// �r���{�[�h��Ԃ̎擾
	BillboardType GetBillboardType() const { return m_billboardType; }

	/// �����X�V�̗L���ݒ�
	void SetEnableAutoUpdate(bool flag) { m_isAutoUpdate = flag; }

	/// �����X�V�̗L������
	bool IsEnableAutoUpdate() const { return m_isAutoUpdate; }

	/// �q�m�[�h�̒ǉ� (WPF �� StakPanel.Children �݂����� Collection ��Ԃ��̂��ǂ����ǁAAddChild() �͂悭�g���̂Ń��[�e�B���e�B�Ƃ��Ē�`s��u���̂��ǂ�����)
	void AddChild(SceneNode* node);

public:

	void Create(SceneGraphManager* manager);

	/// �m�[�h��ʂ̎擾
	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_BaseNode; }

	/// 1�t���[�����̍X�V����
	virtual void UpdateFrame(float elapsedTime) {}

	/// �O���[�o�����W�ϊ��s��Ɗe�v���p�e�B���K�w�I�ɍX�V����
	///		���̏�����1�t���[�����ŊJ�n����1�x�����Ă΂��B
	///		���W�ϊ��s����X�V���A�`�悷��ׂ��m�[�h�ł��邩���t�B���^�����O����B
	virtual void UpdateFrameHierarchy(SceneNode* parent, SceneNodeList* renderingNodeList);

	/// ���_�Ɉ˂�f�[�^���K�w�I�ɍX�V���� (���鎋�_����`�悳���Ƃ��A�ŏ��ɌĂяo�����)
	///		renderingNodeList	: ���̎��_�̒��Ŏ��ۂɕ`�悷����̂͂��̃��X�g�ɒǉ����� (������J�����O�Ȃ�)
	///		renderingLightList	: this �ȉ��� Light �͂��̃��X�g�ɒǉ�����
	///		���̏�����1�t���[�����ŁA���C���[�P�ʂ̕`��J�n���ɌĂ΂��B
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList);

	/// ���̃m�[�h�̋߂����C�g��I������
	///		UpdateViewFlustumHierarchy() �ŁA����̕`��ŕK�v�ȃ��C�g�����܂�B
	///		���̃��X�g���g���āA���̃m�[�h�̕`��ɉe�����郉�C�g�����X�g�A�b�v����B
	///		maxCount �̓V�F�[�_���K�v�Ƃ��Ă��郉�C�g�̍ő吔�BrenderingLightList �̒����� maxCount ��肾���Ηǂ��B
	virtual void UpdateAffectLights(LightNodeList* renderingLightList, int maxCount) {}

	/// ���̃m�[�h��`�悷��
	//virtual void Render(RenderingParams& params) {}

	virtual void DrawSubsetInternal(RenderingParams& params, int subsetIndex, MMEShader* shader, Graphics::ShaderPass* pass) {};

	/// Z �\�[�g�p�̔�r�֐� (�����ƗD��x�Ń\�[�g)
	static bool CmpZAndPrioritySort(const SceneNode* left, const SceneNode* right);


	// MME �� CONTROLOBJECT �A�m�e�[�V�����֌W
	virtual bool IsVisible() const { return false; }
	virtual const Matrix& GetMatrix() const { return m_combinedGlobalMatrix; }
	virtual float GetOpacity() const { return 0.0f; }
	virtual const Matrix& FindFrameMarix(const String& name) const  { return Matrix::Identity; }	// TODO
	virtual float FindMorphBlend(const String& name) const { return 0.0f; }	// TODO
	virtual LightNodeList* GetAffectLightList() { return NULL; }
	

protected:
	SceneGraphManager*	m_manager;
	String				m_name;
	Matrix				m_localMatrix;
	SQTTransform		m_transform;
	Vector3				m_transformCenter;
	RotationOrder		m_rotOrder;
	int					m_priority;
	BillboardType		m_billboardType;
	//bool				m_isVisible;
	bool				m_transformModified;	///< ���W�ϊ��s��̍Čv�Z���K�v��
	bool				m_isAutoUpdate;

	RefPtr<SceneNodeRefList>	m_children;
	SceneNode*			m_parentNode;

	Matrix				m_combinedGlobalMatrix;

	friend class DrawingLayer;
	float				m_zDistance;			///< Z �\�[�g�p�̋����l (�\�[�g���̂����x�I�ɃN���e�B�J���ɂȂ肻���Ȃ̂ŁAgetter �����z�֐��ɂ͂������Ȃ�)

	friend class MMERenderingPass;	// TODO: VisualNode �Ɏ����Ă��ׂ������B���ł� friend �����Ƃ�������
	RenderingPassClientData	m_renderingPassClientDataList[RenderingPass::MaxRenderingPass];
};

} // namespace Scene
} // namespace Lumino
