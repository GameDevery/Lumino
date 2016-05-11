﻿
#include "Internal.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btMotionState.h>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Physics/Collider.h>
#include <Lumino/Physics/RigidBody.h>
#include "BulletUtils.h"

LN_NAMESPACE_BEGIN
namespace Physics
{

#if 0
struct	DefaultMotionState : public btMotionState
{
	btTransform m_graphicsWorldTrans;
	btTransform	m_centerOfMassOffset;
	btTransform m_startWorldTrans;
	void*		m_userPointer;

	DefaultMotionState(const btTransform& startTrans = btTransform::getIdentity(),const btTransform& centerOfMassOffset = btTransform::getIdentity())
		: m_graphicsWorldTrans(startTrans),
		m_centerOfMassOffset(centerOfMassOffset),
		m_startWorldTrans(startTrans),
		m_userPointer(0)

	{
		
	}

	///synchronizes world transform from user to physics
	virtual void	getWorldTransform(btTransform& centerOfMassWorldTrans ) const 
	{
			centerOfMassWorldTrans = 	m_centerOfMassOffset.inverse() * m_graphicsWorldTrans ;

			//BulletUtil::dumpBtTransform(centerOfMassWorldTrans);
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void	setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		//BulletUtil::dumpBtTransform(centerOfMassWorldTrans);

		/*printf("setWorldTransform() %f %f %f\n", 
			centerOfMassWorldTrans.getOrigin().x(), 
			centerOfMassWorldTrans.getOrigin().y(), 
			centerOfMassWorldTrans.getOrigin().z());
			m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset ;*/
			//if (LMath::isNaN( centerOfMassWorldTrans.getOrigin().x() ))
			//{
			//	int a = 0;
			//}

			/* ここで設定されるものと m_btRigidBody->getWorldTransform() で取得したものは、たとえシミュレーション直後であっても微妙に違う。
			const btMatrix3x3& r = m_graphicsWorldTrans.getBasis();
		const btVector3&   p = m_graphicsWorldTrans.getOrigin();
		LMatrix m(
			r[0].x(), r[1].x(), r[2].x(), 0.0f,
			r[0].y(), r[1].y(), r[2].y(), 0.0f,
			r[0].z(), r[1].z(), r[2].z(), 0.0f,
			p.x(),   p.y(),    p.z(),    1.0f );

		printf("setWorldTransform()\n");
		m.cdump();
		*/
	}
};



struct RigidBody::KinematicMotionState
: public btMotionState
{
RigidBody* mBody;
btTransform m_graphicsWorldTrans;

KinematicMotionState( const btTransform& startTrans )
	: mBody					( NULL )
	, m_graphicsWorldTrans	( startTrans )
{
}

virtual void getWorldTransform( btTransform& centerOfMassWorldTrans ) const
{
	/* stepSimulation() から呼ばれる。非同期にする場合は注意 */
	//printf("getWorldTransform()\n");
	/*
	btTransform		bttrBoneTransform;

    LMatrix gl_mat = mFrame->getWorldMatrix();
        
	bttrBoneTransform.setFromOpenGLMatrix( (float *)(&gl_mat) );

	centerOfMassWorldTrans = bttrBoneTransform * m_BoneOffset;
	*/
	centerOfMassWorldTrans = m_graphicsWorldTrans;
}

virtual void setWorldTransform( const btTransform& centerOfMassWorldTrans )
{
	//printf("setWorldTransform()\n");
	m_graphicsWorldTrans = centerOfMassWorldTrans;
}

void setMatrix( const LMatrix& matrix )
{
	m_graphicsWorldTrans.setFromOpenGLMatrix( (btScalar*)&matrix );
}
};
#endif


//==============================================================================
// RigidBody
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(RigidBody, BodyBase);

//------------------------------------------------------------------------------
RigidBody* RigidBody::Create(Collider* collider)
{
	ConfigData data;
	RefPtr<RigidBody> obj(LN_NEW RigidBody(), false);
	obj->Initialize(GetPhysicsManager(nullptr), collider, data);
	obj.SafeAddRef();
	return obj;
}

//------------------------------------------------------------------------------
RigidBody::RigidBody()
	: BodyBase()
	, m_btRigidBody(nullptr)
	, m_collider(nullptr)
	, m_group(0xffff)
	, m_groupMask(0xffff)
	, m_worldTransform()
	, m_rigidBodyConstraintFlags(RigidBodyConstraintFlags::None)
	, m_modifiedFlags(Modified_None)
{
}

//------------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	if (m_btRigidBody != NULL)
	{
		// 剛体を World から取り除く
		// (Manager でやろうとすると、MultiThreadingRefObjectList からの削除をコールバックしたりしないとダメだったり、同じ処理を何箇所かに書かなければならないので面倒)
		m_manager->GetBtWorld()->removeCollisionObject(m_btRigidBody);

		btMotionState* state = m_btRigidBody->getMotionState();
		LN_SAFE_DELETE(state);

		LN_SAFE_RELEASE(m_collider);
		//btCollisionShape* shape = m_btRigidBody->getCollisionShape();
		//LN_SAFE_DELETE(shape);

		LN_SAFE_DELETE(m_btRigidBody);
	}
}

//------------------------------------------------------------------------------
void RigidBody::Initialize(PhysicsManager* manager, Collider* collider, const ConfigData& configData)
{
	LN_REFOBJ_SET(m_collider, collider);

	// 各初期プロパティ
	float num = configData.Mass * configData.Scale;
	float friction;
	float hitFraction;
	float linearDamping;
	float angularDamping;
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (configData.KinematicObject)
	{
		num = 0.0f;
		friction = configData.Friction;
		hitFraction = configData.Restitution;
		linearDamping = configData.LinearDamping;
		angularDamping = configData.AngularDamping;
	}
	else
	{
		collider->GetBtCollisionShape()->calculateLocalInertia(num, localInertia);
		friction = configData.Friction;
		hitFraction = configData.Restitution;
		linearDamping = configData.LinearDamping;
		angularDamping = configData.AngularDamping;
	}

	// 初期姿勢と MotionState
	btTransform initialTransform;
	if (configData.InitialTransform != NULL)
	{
		initialTransform.setFromOpenGLMatrix((const btScalar*)configData.InitialTransform);
		initialTransform.getOrigin().setX(initialTransform.getOrigin().x() * configData.Scale);
		initialTransform.getOrigin().setY(initialTransform.getOrigin().y() * configData.Scale);
		initialTransform.getOrigin().setZ(initialTransform.getOrigin().z() * configData.Scale);
	}
	else {
		initialTransform.setIdentity();
	}
	btMotionState* motionState;
	if (configData.KinematicObject)
	{
		motionState = new btDefaultMotionState(initialTransform/*initialTransformMatrix * Matrix.Translation(frame.Bone.Position * scale)*/);
	}
	else
	{
		//motionState = new DefaultMotionState(Matrix.Invert(rigid.BoneLocalPosition) * Matrix.Translation(frame.Bone.Position) * frame.CombinedMatrix);
		motionState = new btDefaultMotionState(initialTransform/*initialTransformMatrix * Matrix.Translation(frame.Bone.Position * scale)*/);
	}

	// RigidBody 作成
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(num, motionState, collider->GetBtCollisionShape(), localInertia);
	bodyInfo.m_linearDamping = configData.LinearDamping;	// 移動減
	bodyInfo.m_angularDamping = configData.AngularDamping;	// 回転減
	bodyInfo.m_restitution = configData.Restitution;	    // 反発力
	bodyInfo.m_friction = configData.Friction;				// 摩擦力
	bodyInfo.m_additionalDamping = configData.AdditionalDamping;
	m_btRigidBody = new btRigidBody(bodyInfo);

	if (configData.KinematicObject)
	{
		// CF_KINEMATIC_OBJECT と DISABLE_DEACTIVATION はセット。決まり事。
		// http://bulletjpn.web.fc2.com/07_RigidBodyDynamics.html
		m_btRigidBody->setCollisionFlags( /*m_btRigidBody->getCollisionFlags() | */btCollisionObject::CF_KINEMATIC_OBJECT);
		m_btRigidBody->setActivationState( /*m_btRigidBody->getActivationState() | */DISABLE_DEACTIVATION);
	}
	else
	{
		m_btRigidBody->setActivationState( /*m_btRigidBody->getActivationState() | */DISABLE_DEACTIVATION);
	}
	m_btRigidBody->setSleepingThresholds(0.0f, 0.0f);

	m_group = configData.Group;
	m_groupMask = configData.GroupMask;
	m_modifiedFlags = Modified_Activate;

	BodyBase::Create(manager, m_btRigidBody);
	m_manager->AddRigidBody(this);


	m_mass = configData.Mass;
}
//------------------------------------------------------------------------------
void RigidBody::SetPosition(const Vector3& position)
{
	m_worldTransform.m[3][0] = position.x;
	m_worldTransform.m[3][1] = position.y;
	m_worldTransform.m[3][2] = position.z;
	m_modifiedFlags |= Modified_WorldTransform;	// 姿勢を更新した
	m_modifiedFlags |= Modified_Activate;		// Activate要求

	//btVector3 pos = BulletUtil::LNVector3ToBtVector3( position );

	//if ( m_btRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT )
	//{
	//	btTransform Transform = m_btRigidBody->getWorldTransform();
	//	Transform.setOrigin( pos );
	//	m_btRigidBody->setWorldTransform( Transform );
	//}
	//else
	//{
	//	m_btRigidBody->getWorldTransform().setOrigin( pos );
	//}
	//m_btRigidBody->activate( true );

	//m_btRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
}
void RigidBody::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}


#if 0
//------------------------------------------------------------------------------
void RigidBody::setRotation( const LQuaternion& rotation )
{
	btQuaternion q = BulletUtil::LNQuaternionToBtQuaternion( rotation );
	m_btRigidBody->getWorldTransform().setRotation( q );

		
	//m_btRigidBody->getWorldTransform().setBasis().
	//btTransform transform = m_btRigidBody->getCenterOfMassTransform().setRotation( q );
	//transform.setRotation( q );
	//m_btRigidBody

	m_btRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
}

//------------------------------------------------------------------------------
void RigidBody::setAngle( const LVector3& euler )
{
	btMatrix3x3 m;
	//m.setEulerZYX( euler.x, euler.y, euler.z );
	m.setEulerYPR( euler.y, euler.x, euler.z );
#if 1
	m_btRigidBody->getWorldTransform().setBasis(m);
#else
		

	btTransform transform = m_btRigidBody->getCenterOfMassTransform();
	transform.setBasis( m );

	m_btRigidBody->setCenterOfMassTransform( transform );
	//m_btRigidBody->getWorldTransform().setBasis( m );
#endif
}

//------------------------------------------------------------------------------
void RigidBody::setDamping( float linDamping, float angDamping )
{
	m_btRigidBody->setDamping( linDamping, angDamping );
}

//------------------------------------------------------------------------------
void RigidBody::setRestitution( float value ) 
{ 
	return m_btRigidBody->setRestitution( value ); 
}

//------------------------------------------------------------------------------
float RigidBody::getRestitution() const 
{ 
	return m_btRigidBody->getRestitution(); 
}

//------------------------------------------------------------------------------
void RigidBody::setFriction( float value ) 
{ 
	m_btRigidBody->setFriction( value ); 
}

//------------------------------------------------------------------------------
float RigidBody::getFriction() const 
{ 
	return m_btRigidBody->getFriction(); 
}

//------------------------------------------------------------------------------
const LMatrix& RigidBody::getWorldMatrix() const
{
	return mWorldMatrix;
}

//------------------------------------------------------------------------------
void RigidBody::setKinematicAlignmentMatrix( const LMatrix& matrix )
{
	mKinematicAlignmentMatrix = matrix;
	if ( mKinematicMotionState )
	{
#if 0
		LMatrix m = mKinematicAlignmentMatrix;
		LMatrix::transformBasis( &m );
		mKinematicMotionState->setMatrix( m );
#else
		mKinematicMotionState->setMatrix( mKinematicAlignmentMatrix );
#endif
	}
}
#endif
	

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::SetMass(float mass)
{
	m_mass = mass;
	btVector3 localInertia(0, 0, 0);
	m_collider->GetBtCollisionShape()->calculateLocalInertia(m_mass, localInertia);
	m_modifiedFlags |= Modified_Mass;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::SetConstraintFlags(RigidBodyConstraintFlags flags)
{
	m_rigidBodyConstraintFlags = flags;
	m_modifiedFlags |= Modified_RigidBodyConstraintFlags;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::ApplyForce(const Vector3& force)
{
	m_appliedForce += force;	// 次のシミュレーションまでの力を総和したい
	m_modifiedFlags |= Modified_ApplyForce;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::Activate()
{
	m_modifiedFlags |= Modified_Activate;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::SetWorldTransform(const Matrix& matrix)
{
	m_worldTransform = matrix;
	m_modifiedFlags |= Modified_WorldTransform;
}

//------------------------------------------------------------------------------
const Matrix& RigidBody::GetWorldTransform() const
{
	return m_worldTransform;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::ClearForces()
{
	m_modifiedFlags |= Modified_ClearForces;
}
	
//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::SyncBeforeStepSimulation()
{
	// RigidBodyConstraintFlags
	if ((m_modifiedFlags & Modified_RigidBodyConstraintFlags) != 0)
	{
		btVector3 linearFactor(1.0f, 1.0f, 1.0f);
		btVector3 angularFactor(1.0f, 1.0f, 1.0f);
		if (m_rigidBodyConstraintFlags.TestFlag(RigidBodyConstraintFlags::FreezePositionX)) linearFactor.setX(0.0f);
		if (m_rigidBodyConstraintFlags.TestFlag(RigidBodyConstraintFlags::FreezePositionY)) linearFactor.setY(0.0f);
		if (m_rigidBodyConstraintFlags.TestFlag(RigidBodyConstraintFlags::FreezePositionZ)) linearFactor.setZ(0.0f);
		if (m_rigidBodyConstraintFlags.TestFlag(RigidBodyConstraintFlags::FreezeRotationX)) angularFactor.setX(0.0f);
		if (m_rigidBodyConstraintFlags.TestFlag(RigidBodyConstraintFlags::FreezeRotationY)) angularFactor.setY(0.0f);
		if (m_rigidBodyConstraintFlags.TestFlag(RigidBodyConstraintFlags::FreezeRotationZ)) angularFactor.setZ(0.0f);
		m_btRigidBody->setLinearFactor(linearFactor);
		m_btRigidBody->setAngularFactor(angularFactor);
	}

	// SetWorldTransform 要求
	if ((m_modifiedFlags & Modified_WorldTransform) != 0)
	{
		/*
			stepSimulation() の中でこれらが関係する処理は以下のとおり。
				Kinematic(質量が0)な物体の姿勢を MotionState から btCollisionObject::m_worldTransform に取得 getWorldTransform()
					btCollisionObject::m_worldTransform は setWorldTransform からも設定できるので、MotionState を使う意味はあまり無い。
					ただ、m_updateRevision が変わるのでどう影響するか見切れないところ。黙って MotionState 経由にしておくのが無難かも。
				↓
				シミュレーション実行
					btCollisionObject::m_worldTransform は更新される。
					btCollisionObject::m_interpolationWorldTransform も更新される。setCenterOfMassTransform() 経由で。
					setCenterOfMassTransform() は stepSimulation() の下で呼ばれている。
				↓
				Kinematicではない物体の姿勢を MotionState に set。
					このとき、何かの時間を使って m_interpolationWorldTransform を補完し、その結果を MotionState に set している。
					なので、btCollisionObject::m_worldTransform とも btCollisionObject::m_interpolationWorldTransform とも違う姿勢が渡されてくることになり、
					これがシミュレーションの最終結果となる。

			なお、ソースを見た限りだと btMotionState::getWorldTransform() は Kinematic なオブジェクトでしか呼ばれない。
			普通のオブジェクトは btCollisionObject::setWorldTransform() にて姿勢を設定する必要がある。

			念のため…ということで、両方の setWorldTransform() を行い、様子を見てみる。
		*/
		btTransform transform;
		transform.setFromOpenGLMatrix((btScalar*)&m_worldTransform);
		m_btRigidBody->getMotionState()->setWorldTransform(transform);
		m_btRigidBody->setWorldTransform(transform);
	}

	// Mass
	if ((m_modifiedFlags & Modified_Mass) != 0)
	{
		/* Bullet は StaticObject はシミュレーションから完全に除外している。
		 * StaticObject でないものは World の m_nonStaticRigidBodies に追加されてシミュレーションの
		 * 対象となるのだが、このリストに追加されるのは addRigidBody() された時のみ。
		 * 後から StaticObject であるフラグが変更されても、再度 addRigidBody() しなければ
		 * シミュレーションの対象とはならない。
		 * ここでは、setMassProps() の前後でフラグ isStaticObject() が変わった場合、World に追加しなおしている。
		 */
		bool isStatic = m_btRigidBody->isStaticObject();
		btVector3 inertia;
		m_btRigidBody->getCollisionShape()->calculateLocalInertia(m_mass, inertia);
		m_btRigidBody->setMassProps(m_mass, inertia);
		if (isStatic != m_btRigidBody->isStaticObject())
		{
			m_manager->GetBtWorld()->removeRigidBody(m_btRigidBody);
			m_manager->GetBtWorld()->addRigidBody(m_btRigidBody);
		}
	}

	// ClearForces 要求
	if ((m_modifiedFlags & Modified_ClearForces) != 0)
	{
		m_btRigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		m_btRigidBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		//m_btRigidBody->setInterpolationLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		//m_btRigidBody->setInterpolationAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		//m_btRigidBody->setInterpolationWorldTransform(m_btRigidBody->getCenterOfMassTransform());
		m_btRigidBody->clearForces();
	}

	// ApplyForce
	if ((m_modifiedFlags & Modified_ApplyForce) != 0)
	{
		m_btRigidBody->applyForce(BulletUtil::LNVector3ToBtVector3(m_appliedForce), btVector3(0,0,0));
		m_appliedForce = Vector3::Zero;
	}
	//m_btRigidBody->applyImpulse

	// Activate 要求
	if ((m_modifiedFlags & Modified_Activate) != 0)
	{
		m_btRigidBody->activate();
	}

	m_modifiedFlags = Modified_None;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void RigidBody::SyncAfterStepSimulation()
{
	if (m_btRigidBody->isKinematicObject())
	{
		// static オブジェクトなので受け取る必要はない
	}
	else
	{
		btTransform transform;
		m_btRigidBody->getMotionState()->getWorldTransform(transform);
		transform.getOpenGLMatrix((btScalar*)&m_worldTransform);
	}
}

#if 0

//==============================================================================
// Plane
//==============================================================================

//------------------------------------------------------------------------------
Plane::Plane(PhysicsManager* manager, uint16_t group, uint16_t groupMask)
{
	btCollisionShape* shape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);

	ConfigData data;
	data.Group = group;
	data.GroupMask = groupMask;
	RigidBody::Create(manager, shape, data);
}

//------------------------------------------------------------------------------
Plane::~Plane()
{
}

//==============================================================================
// Box
//==============================================================================

//------------------------------------------------------------------------------
Box::Box(PhysicsManager* manager, const Vector3& size, float mass, uint16_t group, uint16_t groupMask)
{
	btCollisionShape* shape = new btBoxShape(BulletUtil::LNVector3ToBtVector3(size/* * 0.5f*/));

	ConfigData data;
	data.Mass = mass;
	data.Group = group;
	data.GroupMask = groupMask;
	RigidBody::Create(manager, shape, data);
}

//------------------------------------------------------------------------------
Box::Box(PhysicsManager* manager, const Vector3& size, const ConfigData& configData)
{
	btCollisionShape* shape = new btBoxShape(BulletUtil::LNVector3ToBtVector3(size/* * 0.5f*/));
	RigidBody::Create(manager, shape, configData);
}

//------------------------------------------------------------------------------
Box::~Box()
{
}

//==============================================================================
// Capsule
//==============================================================================

//------------------------------------------------------------------------------
Capsule::Capsule(PhysicsManager* manager, float radius, float length, float mass, uint16_t group, uint16_t groupMask)
{
	btCollisionShape* shape = new btCapsuleShape(radius, length);

	ConfigData data;
	data.Mass = mass;
	data.Group = group;
	data.GroupMask = groupMask;
	RigidBody::Create(manager, shape, data);
}

//------------------------------------------------------------------------------
Capsule::Capsule(PhysicsManager* manager, float radius, float length, const ConfigData& configData)
{
	btCollisionShape* shape = new (radius, length);
	RigidBody::Create(manager, shape, configData);
}

//------------------------------------------------------------------------------
Capsule::~Capsule()
{
}

//==============================================================================
// Capsule
//==============================================================================

//------------------------------------------------------------------------------
Sphere::Sphere(PhysicsManager* manager, float radius, float mass, uint16_t group, uint16_t groupMask)
{
	btCollisionShape* shape = new btSphereShape(radius);

	ConfigData data;
	data.Mass = mass;
	data.Group = group;
	data.GroupMask = groupMask;
	RigidBody::Create(manager, shape, data);
}

//------------------------------------------------------------------------------
Sphere::Sphere(PhysicsManager* manager, float radius, const ConfigData& configData)
{
	btCollisionShape* shape = new btSphereShape(radius);
	RigidBody::Create(manager, shape, configData);
}

//------------------------------------------------------------------------------
Sphere::~Sphere()
{
}

#endif

} // namespace Physics
LN_NAMESPACE_END
