
#include "../Internal.h"
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Graphics/Mesh/SkinnedMeshModel.h>
#include "../GraphicsManager.h"
#include "../../Modeling/PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SkinnedMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SkinnedMeshModel, Object);

//------------------------------------------------------------------------------
SkinnedMeshModel::SkinnedMeshModel()
{
}

//------------------------------------------------------------------------------
SkinnedMeshModel::~SkinnedMeshModel()
{
}

//------------------------------------------------------------------------------
void SkinnedMeshModel::Initialize(GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh)
{
	if (LN_CHECKEQ_ARG(manager == nullptr)) return;
	if (LN_CHECKEQ_ARG(sharingMesh == nullptr)) return;

	// ���b�V��(�o�b�t�@��)�͋��L����
	m_meshResource = sharingMesh;

	// �}�e���A���̓R�s�[����
	// TODO: �R�s�[�L���̃t���O���������ق������������H
	int count = m_meshResource->materials.GetCount();
	m_materials = RefPtr<MaterialList>::MakeRef();
	m_materials->Resize(count);
	for (int i = 0; i < count; ++i)
	{
		m_materials->SetAt(i, m_meshResource->materials.GetAt(i)->MakeCommonMaterial());
	}
}

LN_NAMESPACE_END
