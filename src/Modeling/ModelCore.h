
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include "Common.h"

namespace Lumino
{
namespace Modeling
{
class ModelManager;


typedef ArrayList<Graphics::Material>	MaterialList;
typedef ArrayList<MeshAttribute>		MeshAttributeList;

typedef ArrayList<ModelBoneCore*>		ModelBoneCoreList;
typedef ArrayList<ModelIKCore*>			ModelIKCoreList;
typedef ArrayList<ModelMorphCore*>		ModelMorphCoreList;
typedef ArrayList<ModelRigidBodyCore*>	ModelRigidBodyCoreList;
typedef ArrayList<ModelJointCore*>		ModelJointCoreList;

/// ���L�}�e���A���f�[�^
class ModelMaterialCore
	: public RefObject
{
public:
	void PostInitialize();

	//ModelMaterialCore* copy() const
	//{
	//	ModelMaterialCore* m = LN_NEW ModelMaterialCore();
	//	m->Materials = Materials;
	//	m->Attributes = Attributes;
	//	return m;
	//}

public:
	//String			Name;				///< �ގ���
	//String			EnglishName;		///< �ގ���(�p)

	MaterialList		Materials;			///< �}�e���A�����X�g
	MeshAttributeList	Attributes;			///< �������X�g

	// PostInitialize() �Őݒ�
	MaterialList		OriginalMaterials;	///< �ގ����[�t�p�I���W�i���}�e���A��
};

/// ���L�{�[���f�[�^
class ModelBoneCore
	: public RefObject
{
public:
	ModelBoneCore(ModelCore* owner, int boneIndex);

public:
	String		Name;							///< �{�[����
	//String	EnglishName;					///< �{�[���p��
	Vector3		OrgPosition;					///< ���f�����_����̈ʒu
	int			ParentBoneIndex;				///< �e�{�[���̃C���f�b�N�X (-1 �͖���)
	int			TransformLevel;					///< �ό`�K�w  TOOD: IK�{�[���y�т���IK��e�Ƃ��Ă���n��̕ό`�K�w�� 0��1�A��]�e�����ȉ��̕ό`�K�w�� 0��2 (SortVal �̍l�����ȋC������BMMM �ł͎g���ĂȂ��݂���)

	// ���{�[���t���O
	BoneConnectType		BoneConnect;			///< �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��
	bool				CanRotate;				///< ��]�\
	bool				CanMove;				///< �ړ��\
	bool				IsVisible;				///< �\��
	bool				CanOperate;				///< �����
	bool				IsIK;					///< IK
	LocalProvideType	LocalProvide;			///< ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
	bool				IsRotateProvided;		///< ��]�t�^
	bool				IsMoveProvided;			///< �ړ��t�^
	bool				IsFixAxis;				///< ���Œ�
	bool				IsLocalAxis;			///< ���[�J����
	bool				TransformAfterPhysics;	///< ������ό`
	bool				ParentTransform;		///< �O���e�ό`
	// ���{�[���t���O

	// ���{�[���t���O�ɂ��g�p����f�[�^
	Vector3		PositionOffset;					///< [�ڑ���:0 �̏ꍇ] ���W�I�t�Z�b�g, �{�[���ʒu����̑��Ε�
	int			ConnectedBoneIndex;				///< [�ڑ���:1 �̏ꍇ] �ڑ���{�[���̃{�[��Index
	int			ProvidedParentBoneIndex;		///< [��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ] �t�^�e�{�[���̃{�[��Index
	float		ProvidedRatio;					///< [��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ] �t�^��
	Vector3		AxisDirectionVector;			///< [���Œ�:1 �̏ꍇ] ���̕����x�N�g��
	Vector3		DimentionXDirectionVector;		///< [���[�J����:1 �̏ꍇ] X���̕����x�N�g��
	Vector3		DimentionZDirectionVector;		///< [���[�J����:1 �̏ꍇ] Z���̕����x�N�g��
	int			KeyValue;						///< [�O���e�ό`:1 �̏ꍇ] Key�l

private:
	ModelCore*	m_owner;
	int			m_boneIndex;
};


/// ���LIK�f�[�^
class ModelIKCore
	: public RefObject
{
public:
	struct IKLink
	{
		int			LinkBoneIndex;		///< IK�\���{�[���ԍ�
		bool		IsRotateLimit;		///< ��]���������邩
		Vector3		MinLimit;			///< ����
		Vector3		MaxLimit;			///< ���
	};

public:
	/* �@ - �A - �B    �C
	*
	*   �@�A : IK�e�����{�[��
	*   �B	: IKTargetBoneIndex (PMX �d�l���ł́uIK�^�[�Q�b�g�{�[���v�AMMDX ���ł́u�G�t�F�N�^�v�ƌĂ΂��)
	*   �C	: IKBone (PMX �d�l�� �ł́uIK�{�[���v�AMMDX ���ł́u�^�[�Q�b�g�{�[���v�ƌĂ΂��B�����Ɠ�)
	*/

	int			IKBoneIndex;			///< IK�{�[�� (PMX �ł́A���� IK �������{�[�����w��)
	int			IKTargetBoneIndex;		///< IK�^�[�Q�b�g�{�[��
	int			LoopCount;				///< ���Z��
	float		IKRotateLimit;			///< IK���[�v�v�Z����1�񂠂���̐����p�x -> ���W�A���p | PMD��IK�l�Ƃ�4�{�قȂ�̂Œ���

	ArrayList<IKLink> IKLinks;			///< IK�e���{�[���Ɛ����̃��X�g

	/* PMD �̏ꍇ�� IKRotateLimit �͈ȉ��̌v�Z���ʂ��i�[����
	*		PI * Fact * (iLink + 1)
	*/
};


/// ���L���[�t�f�[�^
class ModelMorphCore
	: public RefObject
{
//public:
//	struct MorphVertex
//	{
//		Vector3		Position;
//		uint32_t	TargetIndex;
//		uint32_t	TempBufferTargetIndex;
//	};

public:
	ModelMorphCore();
	virtual ~ModelMorphCore();

public:


	String		Name;						///< ���[�t��
	//String	EnglishName;				///< ���[�t�p��

	int				OperationPanel;			///< ����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��
	ModelMorphType	MorphType;

	union MorphOffset
	{
		struct
		{
			int			MorphIndex;			///< ���[�tIndex
			float		MorphRatio;			///< ���[�t�� : �O���[�v���[�t�̃��[�t�l * ���[�t�� = �Ώۃ��[�t�̃��[�t�l
		} GroupMorphOffset;
		struct
		{
			int			VertexIndex;		///< ���_Index
			float		PositionOffset[3];	///< ���W�I�t�Z�b�g��(x,y,z)
		} VertexMorphOffset;
		struct
		{
			int			BoneIndex;			///< �{�[��Index
			float		Moving[3];			///< �ړ���(x,y,z)
			float		Rotating[4];		///< ��]��-�N�H�[�^�j�I��(x,y,z,w)
		} BoneMorphOffset;
		struct
		{
			int			VertexIndex;		///< ���_Index
			float		UVOffset[4];		///< UV�I�t�Z�b�g��(x,y,z,w) ���ʏ�UV��z,w���s�v���ڂɂȂ邪���[�t�Ƃ��Ẵf�[�^�l�͋L�^���Ă���
		} UVMorphOffset;
		struct
		{
			int			MaterialIndex;		///< �ގ�Index -> -1:�S�ގ��Ώ�
			uint8_t		OffsetCalcType;		///< �I�t�Z�b�g���Z�`�� | 0:��Z, 1:���Z - �ڍׂ͌�q
			float		Diffuse[4];			///< Diffuse (R,G,B,A) - ��Z:1.0�^���Z:0.0 �������l�ƂȂ�(���ȉ�)
			float		Specular[3];		///< Specular (R,G,B)
			float		SpecularCoe;		///< Specular�W��
			float		Ambient[3];			///< Ambient (R,G,B)
			float		EdgeColor[4];		///< �G�b�W�F (R,G,B,A)
			float		EdgeSize;			///< �G�b�W�T�C�Y
			float		TextureCoe[4];		///< �e�N�X�`���W�� (R,G,B,A)
			float		SphereTextureCoe[4];///< �X�t�B�A�e�N�X�`���W�� (R,G,B,A)
			float		ToonTextureCoe[4];	///< Toon�e�N�X�`���W�� (R,G,B,A)
		} MaterialMorphOffset;
		struct
		{
			int			MorphIndex;			///< ���[�tIndex
			float		MorphValue;
		} FlipMorphOffset;
		struct
		{
			int			RigidIndex;
			uint8_t		LocalFlag;
			float		Moving[3];
			float		Rotating[3];
		} ImpulseMorphOffset;
	};

	ArrayList<MorphOffset>	MorphOffsets;
};

/// ���L���̃f�[�^
class ModelRigidBodyCore
	: public RefObject
{
public:

	/// �`��f�[�^
	struct CollisionShapeData
	{
		CollisionShapeType Type;
		union
		{
			struct
			{
				float Radius;
			} Sphere;

			struct
			{
				float Width;
				float Height;
				float Depth;
			} Box;

			struct
			{
				float Radius;
				float Height;
			} Capsule;
		};
	};

public:
	String				Name;				///< ���̖�
	//String			EnglishName;		///< ���̉p��
	int					RelatedBoneIndex;	///< �֘A�{�[��
	uint16_t			Group;				///< �Փ˃O���[�v
	uint16_t			GroupMask;			///< �Փ˃O���[�v�}�X�N
	RigidBodyType		RigidBodyType;		///< �`��
	CollisionShapeData	ColShapeData;		///< �`��T�C�Y
	Matrix				InitialTransform;	///< �{�[�����W��ԓ��ł̏����p�� (����� [RelatedBone �̃A�j���[�V�����K�p�ς� Position] �� [���f���̃��[���h�s��] ����Z����ƁA�������[���h���� WorldTransform �ɂȂ�)



	LMatrix				BoneOffset;			///< �{�[���s�� �� ���̍s��ϊ��p (�O���[�o���s��ł͂Ȃ�)
	LMatrix				InvBoneOffset;		///< ���̍s�� �� �{�[���s��ϊ��p


	float				Mass;				///< ����
	float				LinearDamping;		///< �ړ���
	float				AngularDamping;		///< ��]��
	float				Restitution;		///< ������
	float				Friction;			///< ���C��

};

/// ���L�W���C���g�f�[�^
class ModelJointCore
	: public RefObject
{
public:
	String				Name;						///< �W���C���g��
	//String			EnglishName;				///< �W���C���g�p��
	int					RigidBodyAIndex;			///< �������� A
	int					RigidBodyBIndex;			///< �������� B
	Vector3				Position;					///< �ʒu(���f�����_���S)
	Vector3				Rotation;					///< ��](radian)
	Vector3				PositionLimitLower;			///< �ړ�����1
	Vector3				PositionLimitUpper;			///< �ړ�����2
	Vector3				RotationLimitLower;			///< ��]����1
	Vector3				RotationLimitUpper;			///< ��]����2
	Vector3				SpringPositionStiffness;	///< �΂ˈړ�
	Vector3				SpringRotationStiffness;	///< �΂ˉ�]
};

/// ���f���̋��L�f�[�^�N���X
class ModelCore
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	virtual ~ModelCore();

public:
	ModelManager*			Manager;

	String					Name;				///< ���f����
	//String				EnglishName;		///< ���f���p��
	//String				Comment;			///< �R�����g
	//String				EnglishComment;		///< �R�����g�p

	RefPtr<Graphics::VertexBuffer>	VertexBuffer;///< ���_�o�b�t�@
	RefPtr<Graphics::IndexBuffer>	IndexBuffer;///< �C���f�b�N�X�o�b�t�@

	ModelMaterialCore		Material;			///< �}�e���A��
	ModelBoneCoreList		Bones;				///< �{�[�����X�g
	ModelIKCoreList			IKs;				///< IK ���X�g
	ModelMorphCoreList		Morphs;				///< ���[�t���X�g

	//ModelFrameCoreArray	RootFrames;			///< ���[�g�t���[�� (PMD �͐e�̂��Ȃ��{�[��������������̂ŕ����Ǘ�)

	ModelRigidBodyCoreList	RigidBodys;			///< ���̃��X�g
	ModelJointCoreList		Joints;				///< �W���C���g���X�g

};

} // namespace Modeling
} // namespace Lumino
