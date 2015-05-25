
#pragma once

#include <Lumino/Graphics/Material.h>

namespace Lumino
{
namespace Modeling
{
class ModelManager;
class ModelCore;
class ModelBoneCore;
class ModelIKCore;
class ModelMorphCore;
class ModelRigidBodyCore;
class ModelJointCore;

typedef ArrayList<Graphics::Material>	MaterialList;
typedef ArrayList<ModelBoneCore*>		ModelBoneCoreList;
typedef ArrayList<ModelIKCore*>			ModelIKCoreList;
typedef ArrayList<ModelMorphCore*>		ModelMorphCoreList;
typedef ArrayList<ModelRigidBodyCore*>	ModelRigidBodyCoreList;
typedef ArrayList<ModelJointCore*>		ModelJointCoreList;

class ModelBone;
typedef ArrayList<ModelBone*>			ModelBoneList;


/// ���f���t�@�C���̃t�H�[�}�b�g
enum ModelFormat
{
	ModelFormat_Unknown = 0,
	ModelFormat_PMD,
	ModelFormat_PMX,
};

/// ���b�V���̑���
struct MeshAttribute
{
	int		MaterialIndex;  ///< �Ή�����}�e���A���ԍ�
	int		StartIndex;     ///< �J�n�C���f�b�N�X
	int		PrimitiveNum;   ///< �`��v���~�e�B�u�� (�O�p�`�̐�)
};
typedef ArrayList<MeshAttribute>		MeshAttributeList;

/// �ގ��̕`��I�v�V����
enum MMDDrawingFlags
{
	MMDDrawingFlag_CullingDouble = 0x01,	///< ���ʕ`��
	MMDDrawingFlag_GroundShadow = 0x02,		///< �n�ʉe
	MMDDrawingFlag_SelfShadowMap = 0x04,	///< �Z���t�V���h�E�}�b�v�ւ̕`��
	MMDDrawingFlag_SelfShadow = 0x08,		///< �Z���t�V���h�E�̕`��
	MMDDrawingFlag_Edge = 0x10,				///< �G�b�W�`��
};

/// �{�[���t���O �ڑ���(PMD�q�{�[���w��)�\�����@
enum BoneConnectType
{
	BoneConnectType_PositionOffset = 0,		///< ���W�I�t�Z�b�g�Ŏw��
	BoneConnectType_Bone,					///< �{�[���Ŏw��
};

/// �{�[���t���O ���[�J���t�^ 
enum LocalProvideType
{
	LocalProvideType_UserTransformValue = 0,	///< ���[�U�[�ό`�l�^IK�����N�^���d�t�^
	LocalProvideType_ParentLocalTransformValue,	///< �e�̃��[�J���ό`��
};

/// ���[�t���
enum ModelMorphType
{
	ModelMorphType_Vertex = 0,
	ModelMorphType_UV,
	ModelMorphType_AdditionalUV1,
	ModelMorphType_AdditionalUV2,
	ModelMorphType_AdditionalUV3,
	ModelMorphType_AdditionalUV4,
	ModelMorphType_Bone,
	ModelMorphType_Matrial,
	ModelMorphType_Group,
	ModelMorphType_Flip,
	ModelMorphType_Impulse,
};

/// ���̂̌`��
enum CollisionShapeType
{
	CollisionShapeType_Sphere = 0,		///< ��
	CollisionShapeType_Box,				///< ��
	CollisionShapeType_Capsule,			///< �J�v�Z��    
};

/// ���̂̉��Z���
enum RigidBodyType
{
	RigidBodyType_ControlledByBone = 0,	///< Bone�Ǐ]
	RigidBodyType_Physics,				///< �������Z
	RigidBodyType_PhysicsAlignment,		///< �������Z(Bone�ʒu����)
};

// PMX ���f���̒��_���
struct PMX_Vertex
{
public:

	Vector3	Position;			///< �ʒu
	float	BlendWeights[4];	///< �{�[���u�����h��
	float	BlendIndices[4];	///< �{�[���C���f�b�N�X
	Vector3	Normal;				///< �@��
	Vector2	TexUV;				///< �e�N�X�`��UV

	Vector4	AdditionalUV[4];	///< �ǉ�UV
	Vector4	SdefC;				///< Sdef - C�l
	Vector3	SdefR0;				///< Sdef - R0�l
	Vector3	SdefR1;				///< Sdef - R1�l

	float	EdgeWeight;			///< �G�b�W�E�F�C�g
	float	Index;				///< ���_�C���f�b�N�X�l

	/// ���_���C�A�E�g
	static Graphics::VertexElement* Elements()
	{
		static Graphics::VertexElement elements[] = {
			{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Position, 0 },
			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_BlendWeight, 0 },
			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_BlendIndices, 0 },
			{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Normal, 0 },
			{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 0 },

			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 1 },
			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 2 },
			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 3 },
			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 4 },
			{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_TexCoord, 5 },
			{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_TexCoord, 6 },
			{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_TexCoord, 7 },

			{ 0, Graphics::VertexElementType_Float1, Graphics::VertexElementUsage_TexCoord, 8 },
			{ 0, Graphics::VertexElementType_Float1, Graphics::VertexElementUsage_PointSize, 15 },
		};
		return elements;
	}
	static const int ElementCount = 14;
};

} // namespace Modeling
} // namespace Lumino
