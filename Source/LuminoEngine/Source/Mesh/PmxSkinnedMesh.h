/*
	[x] ���b�V��
	[x] �{�[��
	[ ]
	[ ] �t�^
*/
#pragma once
#include <Lumino/Mesh/Mesh.h>

LN_NAMESPACE_BEGIN
class PmxSkinnedMeshResource;

/// ���f���t�@�C���̃t�H�[�}�b�g
enum ModelFormat
{
	ModelFormat_Unknown = 0,
	ModelFormat_PMD,
	ModelFormat_PMX,
};

LN_ENUM_FLAGS(BoneType)
{
	LinkTargetByBoneIndex = 1,
	Rotate,
	XYZ = 4,
	draw = 8,
	Operatable = 16,
	IK = 32,
	IK_Child = 64,
	Unvisible = 128,
	InhereRotate = 256, // ��]�t�^
	InhereXYZ = 512,       // �ړ��t�^
	FixAxis = 1024,
	LocalAxis = 2048,
	TransformAfterPhysics = 4096,
	ParentTransform = 8192
};
LN_ENUM_FLAGS_DECLARE(BoneType);

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

/// ���L�}�e���A���f�[�^
class PmxMaterialResource
	: public RefObject
{
public:

	/// �`��I�v�V����
	enum DrawingFlags
	{
		DrawingFlag_CullingDouble = 0x01,	///< ���ʕ`��
		DrawingFlag_GroundShadow = 0x02,	///< �n�ʉe
		DrawingFlag_SelfShadowMap = 0x04,	///< �Z���t�V���h�E�}�b�v�ւ̕`��
		DrawingFlag_SelfShadow = 0x08,		///< �Z���t�V���h�E�̕`��
		DrawingFlag_Edge = 0x10,			///< �G�b�W�`��
	};

	/// �X�t�B�A�e�N�X�`���̍������[�h
	enum SphereMode
	{
		SphereMode_Disable = 0,				///< ����
		SphereMode_Mul = 1,					///< ��Z(sph)
		SphereMode_Add = 2,					///< ���Z(spa)
		SphereMode_SubTexture = 3,			///< �T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
	};

public:
	// TODO: �ގ��� �Ȃǂ�

	Color						Diffuse;			///< ���̂̐F
	Color						Ambient;			///< ����
	Color						Specular;			///< ����
	Color						Emissive;			///< ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
	float						Power;				///< ����̋���

	Ref<Shader>				Shader;				///< �V�F�[�_
	Ref<ln::Texture>			Texture;			///< �e�N�X�`��
	Ref<ln::Texture>			ToonTexture;		///< [PMD] �g�D�[���e�N�X�`��
	Ref<ln::Texture>			SphereTexture;		///< [PMD] �X�t�B�A�e�N�X�`��

	Color						ToonColor;			///< [PMD] �g�D�[���J���[
	Color						EdgeColor;			///< [PMX] �G�b�W�J���[
	float						EdgeSize;			///< [PMX] �G�b�W�T�C�Y
	Color						TextureCoe;			///< [PMX] �e�N�X�`���W��
	Color						SphereTextureCoe;	///< [PMX] �X�t�B�A�e�N�X�`���W��
	Color						ToonTextureCoe;		///< [PMX] Toon�e�N�X�`���W��
	uint32_t					DrawingFlags;		///< [PMX] �`��I�v�V���� (MMDDrawingFlags �̑g�ݍ��킹)
	SphereMode					SphereMode;			///< [PMX] �X�t�B�A�e�N�X�`���̍������[�h

public:

	PmxMaterialResource()
	{
		Diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
		Ambient.set(0.0f, 0.0f, 0.0f, 0.0f);
		Specular.set(0.5f, 0.5f, 0.5f, 0.5f);
		Emissive.set(0.0f, 0.0f, 0.0f, 0.0f);
		Power = 50.0f;
		ToonColor.set(1.0f, 1.0f, 1.0f, 1.0f);
		EdgeColor.set(0.0f, 0.0f, 0.0f, 1.0f);
		EdgeSize = 0.0f;
		SphereMode = SphereMode_Disable;
		DrawingFlags = 0;
	}

	Ref<Material> MakeCommonMaterial() const;
};

/// ���L�{�[���f�[�^
class PmxBoneResource
	: public RefObject
{
public:
	PmxBoneResource(PmxSkinnedMeshResource* owner, int boneIndex);
	void refreshInitialValues();
	int getBoneIndex() const { return m_boneIndex; }
	const Vector3& getOffsetFromParent() const { return m_offsetFromParent; }
	const Matrix& getInitialTranstormInv() const { return m_initialTranstormInv; }

public:
	String				Name;						///< �{�[����
	//String			EnglishName;				///< �{�[���p��
	Vector3				OrgPosition;				///< ���f�����_����̈ʒu
	int					ParentBoneIndex;			///< �e�{�[���̃C���f�b�N�X (-1 �͖���)
	int					TransformLevel;				///< �ό`�K�w  TOOD: IK�{�[���y�т���IK��e�Ƃ��Ă���n��̕ό`�K�w�� 0��1�A��]�e�����ȉ��̕ό`�K�w�� 0��2 (SortVal �̍l�����ȋC������BMMM �ł͎g���ĂȂ��݂���)

	// ���{�[���t���O
	BoneConnectType		BoneConnect;				///< �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��
	bool				CanRotate;					///< ��]�\
	bool				CanMove;					///< �ړ��\
	bool				isVisible;					///< �\��
	bool				CanOperate;					///< �����
	bool				IsIK;						///< IK
	LocalProvideType	LocalProvide;				///< ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
	bool				IsRotateProvided;			///< ��]�t�^
	bool				IsMoveProvided;				///< �ړ��t�^
	bool				IsFixAxis;					///< ���Œ�
	bool				IsLocalAxis;				///< ���[�J����
	bool				TransformAfterPhysics;		///< ������ό`
	bool				ParentTransform;			///< �O���e�ό`
	// ���{�[���t���O

	// ���{�[���t���O�ɂ��g�p����f�[�^
	Vector3				PositionOffset;				///< [�ڑ���:0 �̏ꍇ] ���W�I�t�Z�b�g, �{�[���ʒu����̑��Ε� (�G�f�B�^�Ń{�[����\������Ƃ��́A��[�����B�Q�[���Ƃ��Ă͕K�v�Ȃ��B�f�o�b�O���Ƃ��ă{�[����\������Ƃ��ɗ��p�ł���)
	int					ConnectedBoneIndex;			///< [�ڑ���:1 �̏ꍇ] �ڑ���{�[���̃{�[��Index (�G�f�B�^�Ń{�[����\������Ƃ��́A��[�����B�Q�[���Ƃ��Ă͕K�v�Ȃ��B�f�o�b�O���Ƃ��ă{�[����\������Ƃ��ɗ��p�ł���)
	int					ProvidedParentBoneIndex;	///< [��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ] �t�^�e�{�[���̃{�[��Index
	float				ProvidedRatio;				///< [��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ] �t�^��
	Vector3				AxisDirectionVector;		///< [���Œ�:1 �̏ꍇ] ���̕����x�N�g�� (TODO: �P����̐���������BPMXE �̃{�[�����X�g�ł͎��̃A�C�R���ŕ\�������BMMF �ł͖��Ή�������)
	Vector3				DimentionXDirectionVector;	///< [���[�J����:1 �̏ꍇ] X���̕����x�N�g�� (TODO: �C���p���X���[�t�Ŏg�p����悤���B���͖��Ή�)
	Vector3				DimentionZDirectionVector;	///< [���[�J����:1 �̏ꍇ] Z���̕����x�N�g�� (TODO: �C���p���X���[�t�Ŏg�p����悤���B���͖��Ή�)
	int					KeyValue;					///< [�O���e�ό`:1 �̏ꍇ] Key�l

private:
	PmxSkinnedMeshResource*	m_owner;
	int						m_boneIndex;
	Vector3					m_offsetFromParent;			///< (refreshInitialValues() �Őݒ肳���) �e�{�[���̂���̑��Έʒu (�eOrgPosition - OrgPosition)
	Matrix					m_initialTranstormInv;		///< (refreshInitialValues() �Őݒ肳���) ���f�����W�n���̏����p���̋t�s��
};


/// ���LIK�f�[�^
class PmxIKResource
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
	
		�@�A : IK�e�����{�[��
		�B	: IKTargetBoneIndex (PMX �d�l���ł́uIK�^�[�Q�b�g�{�[���v�AMMDX ���ł́u�G�t�F�N�^�v�ƌĂ΂��)
		�C	: IKBone (PMX �d�l�� �ł́uIK�{�[���v�AMMDX ���ł́u�^�[�Q�b�g�{�[���v�ƌĂ΂��B�����Ɠ��A�u�E��IK�v�̂悤�Ȗ��O�ɂȂ��Ă���)

		IK�{�[�� �̓X�L�j���O�ɉe�����Ȃ��B
		IK�{�[�� ���S�[���Ƃ��ă\���u���� IK�^�[�Q�b�g�{�[�� �y�� IK�e�����{�[�� ���X�L�j���O�ɉe������B

		���������� IK �𑀂肽���Ƃ��́AIK�{�[�� �̈ʒu�𑀍삷�邱�ƂɂȂ�B
		����� MMD ��Ń��[�V��������鎞�������B
	*/

	int			IKBoneIndex;			///< IK�{�[�� (PMX �ł́A���� IK �������{�[�����w��) TODO: ����Ȃ�
	int			IKTargetBoneIndex;		///< IK�^�[�Q�b�g�{�[��
	int			LoopCount;				///< ���Z��
	float		IKRotateLimit;			///< IK���[�v�v�Z����1�񂠂���̐����p�x -> ���W�A���p | PMD��IK�l�Ƃ�4�{�قȂ�̂Œ���

	List<IKLink> IKLinks;			///< IK�e���{�[���Ɛ����̃��X�g

	/* PMD �̏ꍇ�� IKRotateLimit �͈ȉ��̌v�Z���ʂ��i�[����
	*		PI * Fact * (iLink + 1)
	*/
};


/// ���L���[�t�f�[�^
class PmxMorphResource
	: public RefObject
{
public:
	//ModelMorphCore();
	//virtual ~ModelMorphCore();

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

	List<MorphOffset>	MorphOffsets;
};

/// ���L���̃f�[�^
class PmxRigidBodyResource
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
	int					RelatedBoneIndex;	///< �֘A�{�[�� (�֘A�Ȃ��̏ꍇ��-1)
	uint16_t			Group;				///< �Փ˃O���[�v
	uint16_t			GroupMask;			///< �Փ˃O���[�v�}�X�N
	RigidBodyType		RigidBodyType;		///< �`��
	CollisionShapeData	ColShapeData;		///< �`��T�C�Y
	Matrix				InitialTransform;	///< ���f�����W��ԓ��ł̏����p�� (���̂܂܍��̂̏����p���ɂȂ�)



	//Matrix				BoneOffset;			///< �{�[���s�� �� ���̍s��ϊ��p (�O���[�o���s��ł͂Ȃ�)
	//Matrix				InvBoneOffset;		///< ���̍s�� �� �{�[���s��ϊ��p


	float				Mass;				///< ����
	float				LinearDamping;		///< �ړ���
	float				AngularDamping;		///< ��]��
	float				Restitution;		///< ������
	float				Friction;			///< ���C��

};

/// ���L�W���C���g�f�[�^
class PmxJointResource
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
class PmxSkinnedMeshResource
	: public MeshResource
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	virtual ~PmxSkinnedMeshResource();
	void refreshInitialValues();

public:
	ModelFormat				Format;

	String					Name;				///< ���f����
	//String				EnglishName;		///< ���f���p��
	//String				Comment;			///< �R�����g
	//String				EnglishComment;		///< �R�����g�p

	List<Ref<PmxMaterialResource>>	materials;	// �}�e���A�����X�g
	List<Ref<PmxBoneResource>>		bones;		// �{�[�����X�g
	List<Ref<PmxIKResource>>		iks;		// IK ���X�g
	List<Ref<PmxMorphResource>>		morphs;		// ���[�t���X�g
	List<Ref<PmxRigidBodyResource>>	rigidBodys;	// ���̃��X�g
	List<Ref<PmxJointResource>>		joints;		// �W���C���g���X�g
};

LN_NAMESPACE_END

