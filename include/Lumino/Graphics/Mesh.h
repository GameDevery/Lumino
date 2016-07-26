
#pragma once
#include <Lumino/Graphics/Common.h>
#include "Vertex.h"
#include "Material.h"

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class StaticMeshModel;
using StaticMeshModelPtr = RefPtr<StaticMeshModel>;

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
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	void SetMaterialCount(int count);


LN_INTERNAL_ACCESS:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(GraphicsManager* manager);
	void CreateBox(const Vector3& size);
	void CreateSphere(float radius, int slices, int stacks);

	int GetSubsetCount() const { return m_attributes.GetCount(); }
	void Draw(GraphicsContext* g);

	 
	GraphicsManager*			m_manager;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	RefPtr<VertexBuffer>		m_vertexBuffer;
	RefPtr<IndexBuffer>			m_indexBuffer;

	RefPtr<MaterialList>		m_materials;
	MeshAttributeList			m_attributes;
};

LN_NAMESPACE_END
