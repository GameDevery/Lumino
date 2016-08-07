
#pragma once
#include <Lumino/Graphics/Common.h>
#include "Vertex.h"
#include "Material.h"

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class StaticMeshModel;
using StaticMeshModelPtr = RefPtr<StaticMeshModel>;

/** ���b�V�������I�v�V���� */
LN_ENUM_FLAGS(MeshCreationFlags)
{
	None					= 0x0000,
	DynamicBuffers			= 0x0001,	/**< ���_�o�b�t�@�y�уC���f�b�N�X�o�b�t�@�̍쐬�� ResourceUsage::Dynamic ���g�p����B*/
	ReverseFaces			= 0x0002,	/**< �ʕ����𔽓]����B*/
};
LN_ENUM_FLAGS_DECLARE(MeshCreationFlags);

/// ���b�V���̑���
struct MeshAttribute
{
	int		MaterialIndex;  ///< �Ή�����}�e���A���ԍ�
	int		StartIndex;     ///< �J�n�C���f�b�N�X
	int		PrimitiveNum;   ///< �`��v���~�e�B�u�� (�O�p�`�̐�)
};
typedef Array<MeshAttribute>		MeshAttributeList;

/**
	@brief
*/
class MeshResource
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:


	void SetMaterialCount(int count);

	Material* GetMaterial(int index) const;

	void SetPosition(int index, const Vector3& position);
	void SetUV(int index, const Vector2& uv);
	const Vector3& GetPosition(int index);

	//void FlipFaces();

LN_INTERNAL_ACCESS:
	MeshResource();
	virtual ~MeshResource();
	void Initialize(GraphicsManager* manager);
	void CreateBox(const Vector3& size);
	void CreateSphere(float radius, int slices, int stacks, MeshCreationFlags flags);
	void CreateSquarePlane(const Vector2& size, const Vector3& front, MeshCreationFlags flags);
	void CreateScreenPlane();

	int GetSubsetCount() const { return m_attributes.GetCount(); }

	void TryLockBuffers();
	void CommitRenderData(VertexDeclaration** decls, VertexBuffer** vb, IndexBuffer** ib);


private:
	void CreateBuffers(int vertexCount, int indexCount, int attributeCount, MeshCreationFlags flags);
	void PostGenerated(Vertex* vb, void* ib, MeshCreationFlags flags);

LN_INTERNAL_ACCESS:	// TODO:
	GraphicsManager*			m_manager;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	RefPtr<VertexBuffer>		m_vertexBuffer;
	RefPtr<IndexBuffer>			m_indexBuffer;
	int							m_vertexCount;
	int							m_indexCount;
	Vertex*						m_lockedVertexBuffer;
	void*						m_lockedIndexBuffer;

	RefPtr<MaterialList>		m_materials;
	MeshAttributeList			m_attributes;
};


/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	int GetSubsetCount() const;
	//Material* GetMaterial(int index) const;

	//void SetPosition(int index, const Vector3& position);
	//void SetUV(int index, const Vector2& uv);
	//const Vector3& GetPosition(int index);

LN_INTERNAL_ACCESS:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(GraphicsManager* manager, MeshResource* sharingMesh);
	void InitializeBox(GraphicsManager* manager, const Vector3& size);
	void InitializeSphere(GraphicsManager* manager, float radius, int slices, int stacks, MeshCreationFlags flags);
	void InitializeSquarePlane(GraphicsManager* manager, const Vector2& size, const Vector3& front, MeshCreationFlags flags);
	void InitializeScreenPlane(GraphicsManager* manager);

LN_INTERNAL_ACCESS:	// TODO:
	RefPtr<MeshResource>	m_meshResource;
	RefPtr<MaterialList>	m_materials;
};

LN_NAMESPACE_END
