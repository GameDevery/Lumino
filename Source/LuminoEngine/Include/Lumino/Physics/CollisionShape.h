
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class MeshResource;
class CollisionShape;
class PlaneCollisionShape;
class BoxCollisionShape;
class SphereCollisionShape;
class CapsuleCollisionShape;
class MeshCollisionShape;
using CollisionShapePtr = RefPtr<CollisionShape>;
using PlaneCollisionShapePtr = RefPtr<PlaneCollisionShape>;
using BoxCollisionShapePtr = RefPtr<BoxCollisionShape>;
using SphereCollisionShapePtr = RefPtr<SphereCollisionShape>;
using CapsuleCollisionShapePtr = RefPtr<CapsuleCollisionShape>;
using MeshCollisionShapePtr = RefPtr<MeshCollisionShape>;

/**
	@brief	�Փ˔���̂��߂̃I�u�W�F�N�g�`��̃x�[�X�N���X�ł��B
*/
class CollisionShape
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();

public:

	/** ���� CollisionShape ���Փ˔���̂��߂̃g���K�[�ł��邩��ݒ肵�܂��B�����l�� false �ł��B*/
	void SetTrigger(bool enabled);

	/** ���� CollisionShape ���Փ˔���̂��߂̃g���K�[�ł��邩���擾���܂��B*/
	bool IsTrigger() const;

LN_CONSTRUCT_ACCESS:
	CollisionShape();
	virtual ~CollisionShape();
	void Initialize(btCollisionShape* shape);

LN_INTERNAL_ACCESS:
	btCollisionShape* GetBtCollisionShape() const { return m_shape; }
		
private:
	btCollisionShape*	m_shape;
	Matrix				m_offset;
	bool				m_isTrigger;
};

/**
	@brief	�������ʂ̏Փ˔���`��ł��B
*/
class PlaneCollisionShape
	: public CollisionShape
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		PlaneCollisionShape �I�u�W�F�N�g���쐬���܂��B
		@param[in]	direction	: �ʂ̐��ʕ��� (�ȗ������ꍇ�� Y+ ����)
	*/
	static PlaneCollisionShapePtr Create(const Vector3& direction = Vector3::UnitY);
	
LN_INTERNAL_ACCESS:
	PlaneCollisionShape();
	virtual ~PlaneCollisionShape();
	void Initialize(const Vector3& direction);
};

/**
	@brief	���^�̏Փ˔���`��ł��B
*/
class BoxCollisionShape
	: public CollisionShape
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		BoxCollisionShape �I�u�W�F�N�g���쐬���܂��B
		@param[in]	size	: �e�ӂ̕�
	*/
	static BoxCollisionShapePtr Create(const Vector3& size);
	
	/**
		@brief		BoxCollisionShape �I�u�W�F�N�g���쐬���܂��B
		@param[in]	x, y, z	: �e�ӂ̕�
	*/
	static BoxCollisionShapePtr Create(float x, float y, float z);

LN_INTERNAL_ACCESS:
	BoxCollisionShape();
	virtual ~BoxCollisionShape();
	void Initialize(const Vector3& size);
};

/**
	@brief	���^�̏Փ˔���`��ł��B
*/
class SphereCollisionShape
	: public CollisionShape
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		CapsuleCollisionShape �I�u�W�F�N�g���쐬���܂��B
		@param[in]	radius	: ���a
	*/
	static SphereCollisionShapePtr Create(float radius);

LN_INTERNAL_ACCESS:
	SphereCollisionShape();
	virtual ~SphereCollisionShape();
	void Initialize(float radius);
};

/**
	@brief	�J�v�Z���^�̏Փ˔���`��ł��B
*/
class CapsuleCollisionShape
	: public CollisionShape
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		CapsuleCollisionShape �I�u�W�F�N�g���쐬���܂��B
		@param[in]	radius	: ���a
		@param[in]	height	: ����
	*/
	static CapsuleCollisionShapePtr Create(float radius, float height);

LN_INTERNAL_ACCESS:
	CapsuleCollisionShape();
	virtual ~CapsuleCollisionShape();
	void Initialize(float radius, float height);
};


/**
	@brief	
*/
class MeshCollisionShape
	: public CollisionShape
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		MeshCollisionShape �I�u�W�F�N�g���쐬���܂��B
	*/
	static MeshCollisionShapePtr Create(MeshResource* mesh);

LN_INTERNAL_ACCESS:
	MeshCollisionShape();
	virtual ~MeshCollisionShape();
	void Initialize(MeshResource* mesh);

private:
	btTriangleIndexVertexArray* m_btMeshData;
};

LN_NAMESPACE_END
