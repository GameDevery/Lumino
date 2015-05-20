
#pragma once

namespace Lumino
{
namespace Physics
{
class PhysicsManager;
class BodyBase;
class RigidBody;
class CollisionBody;
class Joint;
class IDebugRenderer;

enum SimulationType
{
	SimulationType_Sync = 0,
	SimulationType_ASync,
};

/// Body �̎��
enum BodyType
{
	BodyType_Unknown = 0,
	BodyType_RigidBody,
	BodyType_SoftBody,
	BodyType_CollisionBody,
	BodyType_CharacterObject,
	BodyType_Joint,

	BodyType_Max,
};

/// �񓯊��V�~�����[�V�����̎��s���
enum ASyncSimulationState
{
	ASyncSimulationState_Idling = 0,	///< �ҋ@���
	ASyncSimulationState_Runnning,		///< ���s��
};

} // namespace Physics
} // namespace Lumino


class btDynamicsWorld;
class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btAxisSweep3;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btTriangleIndexVertexArray;
struct btSoftBodyWorldInfo;

class btCollisionObject;
class btManifoldPoint;
struct btCollisionObjectWrapper;
class btGeneric6DofSpringConstraint;
