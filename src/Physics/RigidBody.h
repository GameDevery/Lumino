
#pragma once

#include "Common.h"
#include "BodyBase.h"

namespace Lumino
{
namespace Physics
{

struct RigidBodyInitArgs
{
    btCollisionShape*   Shape;      ///< (BodyBase  �폜���� delete �����)
    btRigidBody*        RigidBody;  ///< (RigidBody �폜���� delete �����	PMD �����C��������폜�\��)
	//uint16_t               Group;
	//uint16_t               Mask;
	//float				Mass;
	//bool				AdditionalDamping;
	//bool				KinematicObject;	///< Kinematic�I�u�W�F�N�g�Ƃ��� (MotionState �� getWorldTransform() ���Ă΂��悤�ɂȂ�)

	RigidBodyInitArgs()
	{
		//AdditionalDamping = false;
		RigidBody = NULL;
		//KinematicObject = false;
	}
};

/// ���̂̃N���X
class RigidBody
    : public BodyBase
{
public:

	/// ������ԃf�[�^ (MMD �����ɂ��킹�ėp�ӂ��Ă���B���ۂɎg���Ƃ��̓v���p�e�B�I�� Get/Set �ŕҏW���A�x���� bt �I�u�W�F�N�g�����̂��X�}�[�g����)
	struct ConfigData
	{
		float			Mass;				///< ����
		uint16_t		Group;				///< �Փ˃O���[�v
		uint16_t		GroupMask;			///< ��Փ˃O���[�v
	    float			Restitution;	    ///< ������ (HitFraction)
	    float			Friction;		    ///< ���C��
		float			LinearDamping;	    ///< �ړ���
	    float			AngularDamping;		///< ��]��
		const Matrix*	InitialTransform;	///< �����p�� (NULL �� Identity)
		bool			AdditionalDamping;	///< �����̗L��
		bool			KinematicObject;	///< Kinematic�I�u�W�F�N�g�Ƃ��� (���ʂ� 0.0 �Ƃ��Ĉ����AMotionState �� getWorldTransform() ���Ă΂��悤�ɂȂ�)

		float			Scale;				///< (Mass �ɏ�Z����X�P�[���l)

		ConfigData()
		{
			Group = 0xffff;
			GroupMask = 0xffff;
			Mass = 0.0;
			Restitution = 0.0;
			Friction = 0.5;
			LinearDamping = 0.0;
			AngularDamping = 0.0;
			InitialTransform = NULL;
			AdditionalDamping = false;
			KinematicObject = false;
			Scale = 1.0f;
		}
	};

public:
    RigidBody();
    virtual ~RigidBody();

public:

	/// ������ (���̂��󂯎���ă��[���h�ɒǉ�����) (���sPMD�p��public�B��� protected �ɂ���)
	///		shape		: (BodyBase  �폜���� delete �����)
	void Create(PhysicsManager* manager, btCollisionShape* shape, const ConfigData& configData);

#if 0
	/// �ʒu�̐ݒ�
	void setPosition( const Vector3& position );

	/// ��]�̐ݒ�
	void setRotation( const Quaternion& rotation );

	/// ��]�p�x�̐ݒ�
	void setAngle( const Vector3& euler );

	/// �ړ��E��]�����l�̐ݒ�
    void setDamping( float linDamping, float angDamping );

	/// ���˗��̐ݒ�
    void setRestitution( float value_ );

	/// ���˗��̎擾
    float getRestitution() const;

	/// ���C�W���̐ݒ�
    void setFriction( float value_ );

	/// ���C�W���̎擾
    float getFriction() const;

	/// ���[���h�ϊ��s��̎擾
	const Matrix& getWorldMatrix() const;

	/// (�L�l�}�e�B�b�N�ȍ��̗p setWorldMatrix())
	void setKinematicAlignmentMatrix( const Matrix& matrix );
#endif
	/// ���̂� sleep ��Ԃ��������� (���J����K�v�͖��������H)
	void Activate();

	/// ���[���h�ϊ��s��̐ݒ�
	void SetWorldTransform(const Matrix& matrix);

	/// ���[���h�ϊ��s��̎擾
	const Matrix& GetWorldTransform() const;

	void ClearForces();

	/// �w��̎p���������I�ɐݒ肷�� (���x�� 0 �Ƀ��Z�b�g�����)
	//void SetWorldTransformForced(const Matrix& matrix);
	//void moveToForced(const Matrix& matrix);

	/// �������Z�̑Ώۂł��邩 (false �̏ꍇ�A�Փ˔���̂ݑΏ�)
	bool IsContactResponse() const { return true; }

public:	// internal
	virtual BodyType GetBodyType() const { return BodyType_RigidBody; }
	btRigidBody* GetBtRigidBody() { return m_btRigidBody; }
	uint16_t GetGroup() const { return m_group; }
	uint16_t GetGroupMask() const { return m_groupMask; }

	/// �V�~�����[�V�������O�X�V���� (���C���X���b�h����Ă΂��)
	void SyncBeforeStepSimulation();

	/// �V�~�����[�V��������X�V���� (���C���܂��͕����X�V�X���b�h����Ă΂��)
	void SyncAfterStepSimulation();

protected:

	enum ModifiedFlags
	{
		Modified_None = 0x0000,
		Modified_Activate = 0x0001,
		Modified_WorldTransform = 0x0002,
		Modified_ClearForces = 0x0003,
	};

	struct KinematicMotionState;

	btRigidBody*			m_btRigidBody;
	uint16_t				m_group;
	uint16_t				m_groupMask;
	Matrix					m_worldTransform;			///< (postUpdate() �Őݒ肳���)
	uint32_t				m_modifiedFlags;
};

/// ���ʂ̃N���X
class Plane
	: public RigidBody
{
public:
	Plane(PhysicsManager* manager, uint16_t group = 0xffff, uint16_t groupMask = 0xffff);
	virtual ~Plane();
};

/// 
class Box
	: public RigidBody
{
public:
	Box(PhysicsManager* manager, const Vector3& size, float mass, uint16_t group = 0xffff, uint16_t groupMask = 0xffff);
	Box(PhysicsManager* manager, const Vector3& size, const ConfigData& configData);
	virtual ~Box();
};

/// 
class Capsule
	: public RigidBody
{
public:
	Capsule(PhysicsManager* manager, float radius, float length, float mass, uint16_t group = 0xffff, uint16_t groupMask = 0xffff);
	Capsule(PhysicsManager* manager, float radius, float length, const ConfigData& configData);
	virtual ~Capsule();
};

/// 
class Sphere
	: public RigidBody
{
public:
	Sphere(PhysicsManager* manager, float radius, float mass, uint16_t group = 0xffff, uint16_t groupMask = 0xffff);
	Sphere(PhysicsManager* manager, float radius, const ConfigData& configData);
	virtual ~Sphere();
};

} // namespace Physics
} // namespace Lumino
