/*
	[2015/5/13] Bullet �� btCollisionObject::get/setWorldTransform �� btMotionState::get/setWorldTransform �ɂ���

		btCollisionObject �� m_worldTransform �́A�����p���܂��� setCenterOfMassTransform() �� set ����邾���B
		Bullet �̉��Z���ʂ��i�[�������̂ł͂Ȃ��B
		����ɁA�����p���� btMotionState �̃|�C���^���w�肳��Ă���� btMotionState �̎p�����g����B

		�Ƃ肠�����ȒP�ɂ܂Ƃ߂�ƁAbtMotionState �̂ق����̂��B



	[2015/5/1]
		
		Bullet ���g�ɂ��}���`�X���b�h�@�\�� GPGPU �@�\������B
		http://nullorempry.jimdo.com/2012/03/10/bullet-physics%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7/
		�����A2.72 �ł͎�������ĂȂ������B���� MMD �ɋ߂Â���Ȃ玩�O�ŉ������K�v�����邩������Ȃ��B

		�Ebullet �̃}���`�X���b�h�@�\�ɗ���
		�E�V���O���X���b�h�p�̕������Z���A���O�Ń}���`�X���b�h�ŕ��
		


		MMD ���f���ɂ��āE�E�E

			�@���̂��{�[���ʒu�Ɉړ�
			�A�������Z
			�B�{�[�������̈ʒu�Ɉړ�
			�C�`��

			�`��𐳂����s�����߂ɂ́A�������Z�ƕ`�悪1�t���[�����Œ���Ɏ��s����Ȃ���΂Ȃ�Ȃ��B
			����ɂ��Ă��܂���1�t���[���O�̕������Z���Ń{�[���X�V���s�����ƂɂȂ�̂ŁA���f���������ړ������Ƃ��ɐL�тĂ��܂��B
			LightNote �ł͂�ނȂ��A�������Z��Scene&GUI �̍X�V�����ɂ��āA�`��Ƃ͒���ɂ��Ă����B

			�ǂ����Ă��������Z����񉻂���Ȃ�A�B �ɂĕ������Z���������Ă��Ȃ���΁A
			�e�{�[���̑O��t���[������̃I�t�Z�b�g���l�����Ď����ňړ�����K�v������B
			�� �O��t���[���Ƃ������A�Ō�̇@����B�@�ł��A�������Ă��Ȃ���΍��̈ʒu���ړ����Ȃ��悤�ɂ���K�v������B



		Model �N���X�̃C���^�[�t�F�C�X�́H

			Model::GetBones();
			Model::GetMaterials();

			MotionController::AdvanceTime();

			UserUpdate();

			Model::UpdateBonesBeforePhysics();

			PhysicsManager::StepSimulation();

			Model::UpdateBonesAfterPhysics();

			Model::GetSubsetCount();
			Model::DrawSubset();


		�܂��͓������[�h�I�����[�ł����Ǝv���B�O�Ɠ����B
		bullet �̃}���`�X���b�h�@�\�ŉ��P�ł���Ȃ炻��ł������B
		�܂��ꔭ����Ă݂ăx���`�}�[�N���āA����ŉ��P�̗]�n������Ό�������A�ŁB

		���Ȃ݂Ƀ��C�u�������Ń}���`�X���b�h�Ή�����ƂȂ�����A
		�v���p�e�B�͂��ׂĕێ����[�h�BStepSimu ���O�ŃR�~�b�g&bullet�I�u�W�F�N�g�쐬�Ƃ����Ђ悤�B
		�\�t�g�{�f�B�̓m�[�h�P�ʂł��ێ�����K�v������B


		MMD ���f���̃}���`�X���b�h�����X�V�ɂ��āE�E�E
			
			�v������ StepSim ���������Ă��Ȃ���ΐe�{�[���̎p�����p������A�����̊K�w�X�V���s���B
			�|�C���g�́A�ǂ�����Ċ������Ă���̂��`�F�b�N���邩�B

			�E�P���ɁA�{�[���X�V���ɕ����X���b�h���ҋ@�������ׁA�ҋ@���̎������X�V����
				��	NG�BAPI ���V���O���X���b�h���[�h�Ɠ����ɂ������ꍇ�A�{�[���X�V�^�C�~���O�� StepSim �����N�G�X�g��������ł���B
				StepSim �����l�Ɂu�ҋ@���ł���Ύ��s�v�̏������s�����ŁA���̒���ɂ���{�[���X�V�͍��m���ŏ������s���Ȃ��قȂ�B

			�E�v������ StepSim ���������Ă��邩���ׂ� (�{�[���X�V���A��ɗv�����ꂽ StepSim �͓����Ă��邩������Ȃ�)
				�� �ł����Ƃ��Ă��A�{�[���X�V�Ɏg���鍄�̎p���͂P�t���[���O�̂��̂ł���B

*/

#include "../Internal.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

// Win32
//#include <BulletMultiThreaded/Win32ThreadSupport.h>
//#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>

//#include <BulletMultiThreaded/btParallelConstraintSolver.h>
//#include <BulletMultiThreaded/SequentialThreadSupport.h>

#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/Joint.h>
#include <Lumino/Physics/PhysicsManager.h>

namespace Lumino
{
namespace Physics
{

//=============================================================================
// DebugDrawer
//=============================================================================
class PhysicsManager::DebugDrawer
	: public btIDebugDraw
{
public:

	DebugDrawer()
		: mIDebugRenderer(NULL)
		, mDebugMode(0)
	{}
	virtual ~DebugDrawer() {}

public:

	void setDebugRenderer(IDebugRenderer* r) { mIDebugRenderer = r; }

public:

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
	{
		mIDebugRenderer->DrawLine(
			Vector3(from.getX(), from.getY(), from.getZ()),
			Vector3(to.getX(), to.getY(), to.getZ()),
			Vector3(fromColor.getX(), fromColor.getY(), fromColor.getZ()),
			Vector3(toColor.getX(), toColor.getY(), toColor.getZ()));
	}

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color, color);
	}

	virtual void	drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
	{
	}

	virtual void	drawBox(const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
	{
	}

	virtual void	drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
	{
	}

	virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
	}

	virtual void	reportErrorWarning(const char* warningString)
	{
		printf("Physics: %s\n", warningString);
	}

	virtual void	draw3dText(const btVector3& location, const char* textString)
	{
	}

	virtual void	setDebugMode(int debugMode) { mDebugMode = debugMode; }

	virtual int		getDebugMode() const { return mDebugMode; }

protected:

	IDebugRenderer* mIDebugRenderer;
	int mDebugMode;
};

//=============================================================================
// PhysicsManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

PhysicsManager::PhysicsManager(SimulationType type)
	: m_simulationType(type)
	, m_asyncSimulationState(ASyncSimulationState_Idling)
	, m_elapsedTime(0.0f)
	, m_debugDrawer(NULL)
{
	// ���ȉ��ŏo�Ă���P��Ƃ�
	//		http://nullorempry.jimdo.com/2012/03/10/bullet-physics%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7/

	int maxNumOutstandingTasks = 4;

#ifdef LN_USE_PARALLEL
	m_threadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
		"collision",
		processCollisionTask,
		createCollisionLocalStoreMemory,
		maxNumOutstandingTasks));
#endif

	// �J�X�^���R���W�����R�[���o�b�N
	//gContactAddedCallback = CustomMaterialCombinerCallback;

	// �R���W�����R���t�B�O
	btDefaultCollisionConstructionInfo defaultCollisionConstructionInfo;
	//defaultCollisionConstructionInfo.m_defaultMaxPersistentManifoldPoolSize = 32768;
	m_btCollisionConfig = new btDefaultCollisionConfiguration(defaultCollisionConstructionInfo);
	// �\�t�g�{�f�B�g���Ƃ�
	//m_btCollisionConfig = LN_NEW btSoftBodyRigidBodyCollisionConfiguration();// 

	// �R���W�����f�B�X�p�b�`��
#ifdef LN_USE_PARALLEL
	m_btCollisionDispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision, maxNumOutstandingTasks, m_btCollisionConfig);
#else
	m_btCollisionDispatcher = LN_NEW btCollisionDispatcher(m_btCollisionConfig);

#endif

	// �u���[�h�t�F�[�Y�A���S���Y�� (�Փˌ��o�̂��߂̃O���[�v�����A���S���Y��)
	/*
		btDbvtBroadphase
			AABB�؂ɂ�鍂���œ��I�ȊK�w��Ԃ��g���܂��B
		btAxisSweep3 bt32BitAxisSweep3
			����3D�X�C�[�v&�v���[��(incremental 3D sweep and prune)���������Ă��܂��B
		btCudaBroadphase
			GPU���g�p���������Ȉ�l�O���b�h���������Ă��܂��B
			http://bulletjpn.web.fc2.com/05_BulletCollisionDetection.html
	*/
	//   btVector3	btv3WorldAabbMin( -3000.0f, -3000.0f, -3000.0f );
	//btVector3	btv3WorldAabbMax(  3000.0f,  3000.0f,  3000.0f );
	//int			iMaxProxies = 32766;//5 * 5 * 5 + 1024;//
	//m_btBroadphase = new btAxisSweep3( btv3WorldAabbMin, btv3WorldAabbMax, iMaxProxies );
	m_btBroadphase = LN_NEW btDbvtBroadphase();

	// �\���o
#ifdef LN_USE_PARALLEL
	m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
	m_btSolver = new btParallelConstraintSolver(m_threadSupportSolver);
	//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
	m_btCollisionDispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
#else
	m_btSolver = new btSequentialImpulseConstraintSolver();
#endif

	// ���[���h�̍쐬
	//m_btWorld = LN_NEW btSoftRigidDynamicsWorld( m_btCollisionDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfig, NULL );
	m_btWorld = new btDiscreteDynamicsWorld(m_btCollisionDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfig);

	m_debugDrawer = LN_NEW DebugDrawer();
	m_btWorld->setDebugDrawer(m_debugDrawer);
	//m_btWorld->setInternalTickCallback(pickingPreTickCallback,this,true);
	//m_btWorld->getDispatchInfo().m_enableSPU = true;

	//btOverlapFilterCallback * filterCallback = new FilterCallback();
	//m_btWorld->getPairCache()->setOverlapFilterCallback( &gFilterCallback );

#if 0	// MMM setting (�f�t�H���g�� 10)
	m_btWorld->getSolverInfo().m_numIterations = 4;
#endif
#ifdef LN_USE_PARALLEL
	m_btWorld->getSimulationIslandManager()->setSplitIslands(false);
	m_btWorld->getSolverInfo().m_numIterations = 4;
	m_btWorld->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

	m_btWorld->getDispatchInfo().m_enableSPU = true;
#endif
	//m_btWorld->setDebugDrawer(&gDebugDrawer);

	// �d��		TODO: �����l
	m_btWorld->setGravity(btVector3(0.0f, -9.8f * 2.5f, 0.0f));
	//SetGravity(Vector3(0.0f, -9.8f * 2.5f, 0.0f));
	//SetGravity( LVector3( 0.0f, /*-9.8f*/-9.81f * 10.0f/* * 2.0f*/, 0.0f ) );

	m_softBodyWorldInfo = new btSoftBodyWorldInfo();
	m_softBodyWorldInfo->air_density = 1.2f;
	m_softBodyWorldInfo->water_density = 0;
	m_softBodyWorldInfo->water_offset = 0;
	m_softBodyWorldInfo->water_normal = btVector3(0.0f, 0.0f, 0.0f);
	m_softBodyWorldInfo->m_gravity = m_btWorld->getGravity();
	m_softBodyWorldInfo->m_broadphase = m_btBroadphase;
	m_softBodyWorldInfo->m_dispatcher = m_btCollisionDispatcher;
	m_softBodyWorldInfo->m_sparsesdf.Initialize();
	m_softBodyWorldInfo->m_sparsesdf.Reset();

	/*
	const btScalar	s=4;
	const btScalar	h=6;
	const int		r=20;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0,h-2,0));



	int fixed=0;//4+8;
	btSoftBody*		psb=btSoftBodyHelpers::CreatePatch(*m_softBodyWorldInfo,btVector3(-s,h,-s),
	btVector3(+s,h,-s),
	btVector3(-s,h,+s),
	btVector3(+s,h,+s),r,r,fixed,true);
	m_btWorld->addSoftBody(psb);
	psb->setTotalMass(0.1);

	psb->m_cfg.piterations = 10;
	psb->m_cfg.citerations = 10;
	psb->m_cfg.diterations = 10;
	//	psb->m_cfg.viterations = 10;

	*/
}

PhysicsManager::~PhysicsManager()
{
	LN_SAFE_DELETE(m_btCollisionConfig);
	LN_SAFE_DELETE(m_btCollisionDispatcher);
	LN_SAFE_DELETE(m_btBroadphase);
	LN_SAFE_DELETE(m_btSolver);
	LN_SAFE_DELETE(m_btWorld);
	LN_SAFE_DELETE(m_softBodyWorldInfo);

#ifdef LN_USE_PARALLEL
	if (m_threadSupportSolver)
	{
		delete m_threadSupportSolver;
	}
	if (m_threadSupportCollision)
	{
		delete m_threadSupportCollision;
	}
#endif

	LN_SAFE_DELETE(m_debugDrawer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::SyncBeforeStepSimulation()
{
	if (m_simulationType == SimulationType_ASync)
	{
		// �I���܂ő҂�
	}

	//---------------------------------------------------------
	// Add�` �Œǉ����ꂽ�V�K�I�u�W�F�N�g�����[���h�ɓo�^

	//// CollisionBody
	//CollisionBodyArray::ObjectArray& collisionArray = mCollisionBodyArray.getRegistOrderObjectArray();
	//ln_foreach(CollisionBody* b, collisionArray)
	//{
	//	m_btWorld->addCollisionObject(b->getBtCollisionObject(), b->getGroup(), b->getGroupMask());
	//}
	// RigidBody
	LN_FOREACH(RigidBody* obj, m_rigidBodyList.GetRegistOrderObjectArray())
	{
		m_btWorld->addRigidBody(obj->GetBtRigidBody(), obj->GetGroup(), obj->GetGroupMask());
	}
	// Joint
	LN_FOREACH(Joint* obj, m_jointList.GetRegistOrderObjectArray())
	{
		m_btWorld->addConstraint(obj->GetBtConstraint());
	}


	//m_collisionBodyList.Commit();
	m_rigidBodyList.Commit();
	m_jointList.Commit();

	//---------------------------------------------------------
	// ���̂̎p���𓯊�

	LN_FOREACH(RigidBody* b, m_rigidBodyList.GetObjectArray())
	{
		b->SyncBeforeStepSimulation();
	}

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::StepSimulation(float elapsedTime)
{
	SyncBeforeStepSimulation();

	if (m_simulationType == SimulationType_Sync)
	{
		m_elapsedTime = elapsedTime;
		StepSimulationInternal();
	}
	else
	{
		LN_THROW(m_asyncSimulationState == ASyncSimulationState_Idling, InvalidOperationException);	// �ҋ@��ԂɂȂ��Ă��Ȃ���΂Ȃ�Ȃ�

		m_elapsedTime = elapsedTime;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::DrawDebugShapes(IDebugRenderer* renderer)
{
	m_debugDrawer->setDebugRenderer(renderer);
	m_btWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btWorld->debugDrawWorld();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::StepSimulationInternal()
{
	//---------------------------------------------------------
	// StepSimulation

	const float internalUnit = 1.0f / 60.0f;

	// http://d.hatena.ne.jp/ousttrue/20100425/1272165711
	// m_elapsedTime �� 1.0(1�b) ��菬�����Ԃ� 16ms ���ő� 60 ��J��Ԃ��Ēǂ������Ƃ���ݒ�B
	// m_elapsedTime �� 1.0 �𒴂��Ă���ꍇ�͒ǂ������ɁA���̂̈ړ����x���Ȃ�B
	m_btWorld->stepSimulation(m_elapsedTime, 60, internalUnit);

	// m_elapsedTime �� 16ms ���傫���ꍇ�́A1�� 16ms ���̃V�~�����[�V�������\�Ȍ���J��Ԃ��� m_elapsedTime �ɒǂ����Ă����ݒ�B
	// �x���قǌv�Z�񐔂�������̂ŁA�ŏI�I�ɔj�]���邩������Ȃ��B
	//m_btWorld->stepSimulation(m_elapsedTime, 1 + (int)(m_elapsedTime / internalUnit), internalUnit);


	//---------------------------------------------------------
	// ���̂̎p���𓯊� (���Z���� �� ���[�U�[)
	LN_FOREACH(RigidBody* b, m_rigidBodyList.GetObjectArray())
	{
		b->SyncAfterStepSimulation();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::AddRigidBody(RigidBody* body)
{
	m_rigidBodyList.AddObject(body);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::AddJoint(Joint* joint)
{
	m_jointList.AddObject(joint);
}

} // namespace Physics
} // namespace Lumino
