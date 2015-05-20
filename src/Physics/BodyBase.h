
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Physics
{

/// �ڐG�_���
struct ContactInfo
{
	Vector3		Normal;			///< �ڐG�_�@��
	Vector3		Position;		///< �ڐG�_�ʒu
	BodyBase*	TargetBody;		///< �ڐG�{�f�B
};

/// �e�� Body �̃x�[�X�N���X
class BodyBase
    : public RefObject
{
public:
	typedef ArrayList<ContactInfo>	ContactInfoList;

public:
    BodyBase();
    virtual ~BodyBase();

public:

	/// ������
	void Create(PhysicsManager* manager, btCollisionObject* obj);

	/// ���
	virtual BodyType GetBodyType() const = 0;

	/// �ڐG�_���̃��X�g
	const ContactInfoList& GetContactInfoList() const { return m_contactList; }

	/// ���[�U�[�f�[�^�̐ݒ�
	void SetUserData(void* data) { m_userData = data; }

	/// ���[�U�[�f�[�^�̎擾
	void* GetUserData() const { return m_userData; }

public:	// internal
	///// �V�~�����[�V�����X�V���O�̏���
	//virtual void preUpdate() {}

	///// �V�~�����[�V�����X�V����̏���
	//virtual void postUpdate() {}

	/// �f�o�b�O�p�̌`��`��
	virtual void DrawDebugShape(IDebugRenderer* renderer) {}

	void ClearContactList() { m_contactList.Clear(); }

	void AddContact(const ContactInfo& contact) { m_contactList.Add(contact); }

protected:
	PhysicsManager*		m_manager;
	void*				m_userData;
	ContactInfoList		m_contactList;
};

} // namespace Physics
} // namespace Lumino
