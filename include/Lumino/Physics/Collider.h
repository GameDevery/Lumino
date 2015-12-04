
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace Physics
{

/**
	@brief	�Փ˔���̂��߂̃I�u�W�F�N�g�`��̃x�[�X�N���X�ł��B
*/
class Collider
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
protected:
	Collider();
	virtual ~Collider();
	void Initialize(PhysicsManager* manager, btCollisionShape* shape);
		
private:
	PhysicsManager*		m_manager;
	btCollisionShape*	m_shape;
};

/**
	@brief	�ÓI�Ȗ������ʂ̏Փ˔���`��ł��B
*/
class PlaneCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		PlaneCollider �I�u�W�F�N�g���쐬���܂��B
		@param[in]	direction	: �ʂ̐��ʕ��� (�ȗ������ꍇ�� Y+ ����)
		@details	�쐬���ꂽ�I�u�W�F�N�g�͎g���I������ Release() ���Ăяo���ĎQ�Ƃ��������K�v������܂��B
	*/
	static PlaneCollider* Create(const Vector3& direction = Vector3::UnitY);
	
protected:
	PlaneCollider();
	virtual ~PlaneCollider();
	void Initialize(PhysicsManager* manager, const Vector3& direction);
};

/**
	@brief	���^�̏Փ˔���`��ł��B
*/
class BoxCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		BoxCollider �I�u�W�F�N�g���쐬���܂��B
		@param[in]	size	: �e�ӂ̕�
		@details	�쐬���ꂽ�I�u�W�F�N�g�͎g���I������ Release() ���Ăяo���ĎQ�Ƃ��������K�v������܂��B
	*/
	static BoxCollider* Create(const Vector3& size);
	
	/**
		@brief		BoxCollider �I�u�W�F�N�g���쐬���܂��B
		@param[in]	x, y, z	: �e�ӂ̕�
		@details	�쐬���ꂽ�I�u�W�F�N�g�͎g���I������ Release() ���Ăяo���ĎQ�Ƃ��������K�v������܂��B
	*/
	static BoxCollider* Create(float x, float y, float z);
	
protected:
	BoxCollider();
	virtual ~BoxCollider();
	void Initialize(PhysicsManager* manager, const Vector3& size);
};

/**
	@brief	���^�̏Փ˔���`��ł��B
*/
class SphereCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		CapsuleCollider �I�u�W�F�N�g���쐬���܂��B
		@param[in]	radius	: ���a
		@details	�쐬���ꂽ�I�u�W�F�N�g�͎g���I������ Release() ���Ăяo���ĎQ�Ƃ��������K�v������܂��B
	*/
	static SphereCollider* Create(float radius);
	
protected:
	SphereCollider();
	virtual ~SphereCollider();
	void Initialize(PhysicsManager* manager, float radius);
};

/**
	@brief	�J�v�Z���^�̏Փ˔���`��ł��B
*/
class CapsuleCollider
	: public Collider
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		CapsuleCollider �I�u�W�F�N�g���쐬���܂��B
		@param[in]	radius	: ���a
		@param[in]	height	: ����
		@details	�쐬���ꂽ�I�u�W�F�N�g�͎g���I������ Release() ���Ăяo���ĎQ�Ƃ��������K�v������܂��B
	*/
	static CapsuleCollider* Create(float radius, float height);
	
protected:
	CapsuleCollider();
	virtual ~CapsuleCollider();
	void Initialize(PhysicsManager* manager, float radius, float height);
};


} // namespace Physics
LN_NAMESPACE_END
