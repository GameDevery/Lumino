
#pragma once

#include "Common.h"
#include "IDebugRenderer.h"

namespace Lumino
{
namespace Physics
{


/// PhysicsManager �̎����N���X
class PhysicsManager
	: public RefObject
{
public:
	PhysicsManager(SimulationType type);
	virtual ~PhysicsManager();

public:

	/// �񓯊��V�~�����[�V�����̎��s��Ԃ̎擾
	ASyncSimulationState GetASyncSimulationState() const { return m_asyncSimulationState; }

	/// StepSimulation() �̑O�����B���C���X���b�h�ŌĂяo�����ƁB�����X�V�X���b�h�����s���ł���Αҋ@����܂ő҂B
	void SyncBeforeStepSimulation();

	/// 
	void StepSimulation(float elapsedTime);

	void DrawDebugShapes(IDebugRenderer* renderer);

public:	// internal

	/// ���C���X���b�h�A�܂��͕������Z�X���b�h����Ă΂��B
	void StepSimulationInternal();

	btDiscreteDynamicsWorld* GetBtWorld() { return m_btWorld; }
	void AddRigidBody(RigidBody* body);
	void AddJoint(Joint* joint);


private:

	// ���ꂼ�ꃏ�[���h�ւ̒ǉ����\�b�h���Ⴄ�̂ŕ����Ă���
	//typedef MultiThreadingRefObjectList<CollisionBody*>	CollisionBodyList;
	typedef MultiThreadingRefObjectList<RigidBody*>		RigidBodyList;		// �\�t�g�{�f�B�Ƃ̓��[���h�ւ̒ǉ����@���Ⴄ�̂ŕ����Ă���
	typedef MultiThreadingRefObjectList<Joint*>			JointList;

	SimulationType	m_simulationType;

	btDefaultCollisionConfiguration*		m_btCollisionConfig;
	btCollisionDispatcher*					m_btCollisionDispatcher;
	btDbvtBroadphase*						m_btBroadphase;
	//btAxisSweep3*							m_btBroadphase;
	btSequentialImpulseConstraintSolver*	m_btSolver;
	btDiscreteDynamicsWorld*				m_btWorld;
	//btSoftRigidDynamicsWorld*				m_btWorld;
	btSoftBodyWorldInfo*					m_softBodyWorldInfo;

	//btSoftRigidDynamicsWorld*			            m_btWorld;
	//btSoftBodyWorldInfo							m_btSoftBodyWorldInfo;
	//btSoftBodyRigidBodyCollisionConfiguration*	m_btCollisionConfiguration;

	//RigidBodyArray			mRigidBodyArrayList;
	//CollisionBodyArray		mCollisionBodyArray;
	//JointArray				mJointArray;

#ifdef LN_USE_PARALLEL
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
#endif

	ASyncSimulationState		m_asyncSimulationState;
	//CollisionBodyList			m_collisionBodyList;
	RigidBodyList				m_rigidBodyList;
	JointList					m_jointList;

	float						m_elapsedTime;

	class DebugDrawer;
	DebugDrawer*	m_debugDrawer;
};

} // namespace Physics
} // namespace Lumino
