
#include "../Internal.h"
#include "ModelManager.h"
#include "PMXLoader.h"

namespace Lumino
{
namespace Modeling
{

//=============================================================================
// PMXLoader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PMXLoader::~PMXLoader()
{
	LN_FOREACH(Graphics::Texture* tex, m_textureTable) {
		LN_SAFE_RELEASE(tex);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelCore* PMXLoader::Load(ModelManager* manager, Stream* stream, const PathName& baseDir, bool isDynamic)
{
	m_manager = manager;
	m_isDynamic = isDynamic;

	BinaryReader reader(stream);
	m_modelCore.Attach(LN_NEW ModelCore(manager, ModelFormat_PMX), false);
	
	//-----------------------------------------------------
	// �w�b�_
	reader.Read( &m_pmxHeader, sizeof(PMX_Header) );
	if (m_pmxHeader.Magic[0] != 'P' ||
		m_pmxHeader.Magic[1] != 'M' ||
		m_pmxHeader.Magic[2] != 'X' ||
		m_pmxHeader.Magic[3] != ' ')
	{
		return NULL;
	}
	if (m_pmxHeader.Version < 2.0f) return NULL;
#if 0
	_p( pmxHeader.Version );
	printf( "DataSize              : %d\n", pmxHeader.DataSize );
	printf( "�G���R�[�h����        : %d\n", pmxHeader.Data[0]);
	printf( "�ǉ�UV��              : %d\n", pmxHeader.Data[1]);
	printf( "���_Index�T�C�Y       : %d\n", pmxHeader.Data[2]);
	printf( "�e�N�X�`��Index�T�C�Y : %d\n", pmxHeader.Data[3]);
	printf( "�ގ�Index�T�C�Y       : %d\n", pmxHeader.Data[4]);
	printf( "�{�[��Index�T�C�Y     : %d\n", pmxHeader.Data[5]);
	printf( "���[�tIndex�T�C�Y     : %d\n", pmxHeader.Data[6]);
	printf( "����Index�T�C�Y       : %d\n", pmxHeader.Data[7]);
#endif
		
	// ���f�����
	LoadModelInfo( &reader );

	// ���_
	LoadVertices( &reader );

	// ���_�C���f�b�N�X
	LoadIndices( &reader );

	// �e�N�X�`���e�[�u��
	LoadTextureTable( &reader, baseDir );

	// �}�e���A��
	LoadMaterials( &reader );

	// �{�[��
	LoadBones( &reader );

	// ���[�t
	LoadMorphs( &reader );

	// �\���g
	LoadDisplayFrame( &reader );

	// ����
	LoadRigidBodys( &reader );

	// �W���C���g
	LoadJoints( &reader );

	m_modelCore.SafeAddRef();
	return m_modelCore;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadModelInfo(BinaryReader* reader)
{
	// ���f����
	m_modelCore->Name = ReadString(reader);

	// ���f�����p
	/*m_modelCore->EnglishName = */ReadString(reader);

	// �R�����g
	/*m_modelCore->Comment = */ReadString(reader);

	// �R�����g�p
	/*m_modelCore->EnglishComment = */ReadString(reader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadVertices(BinaryReader* reader)
{
	// ���_��
	int vertexCount = reader->ReadInt32();

	// ���_�o�b�t�@�쐬
	m_modelCore->VertexBuffer.Attach(Graphics::VertexBuffer::Create(
		m_manager->GetGraphicsManager(), PMX_Vertex::Elements(), PMX_Vertex::ElementCount, vertexCount, NULL, Graphics::DeviceResourceUsage_Dynamic));

	// �f�[�^�𗬂�����
	struct BaseVertex
	{
		Vector3	Position;
		Vector3	Normal;
		Vector2	TexUV;
	} baseVertex;
	PMX_Vertex* vertices = (PMX_Vertex*)m_modelCore->VertexBuffer->Lock();
	for (int i = 0; i < vertexCount; ++i)
	{
		PMX_Vertex* v = &vertices[i];

		// ���_�A�@���A�e�N�X�`��UV
		reader->Read(&baseVertex, sizeof(BaseVertex));
		v->Position = baseVertex.Position;
		v->Normal = baseVertex.Normal;
		v->TexUV = baseVertex.TexUV;

		// �ǉ�UV
		for (int j = 0; j < getAdditionalUVCount(); i++)
		{
			v->AdditionalUV[i].Set(
				reader->ReadFloat(),
				reader->ReadFloat(),
				reader->ReadFloat(),
				reader->ReadFloat());
		}

		// �u�����h�E�F�C�g
		int defType = reader->ReadInt8();
		switch (defType)
		{
		case 0:	// BDEF1
			v->BlendIndices[0] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = 0.0f;
			v->BlendIndices[2] = 0.0f;
			v->BlendIndices[3] = 0.0f;
			v->BlendWeights[0] = 1.0f;
			v->BlendWeights[1] = 0.0f;
			v->BlendWeights[2] = 0.0f;
			v->BlendWeights[3] = 0.0f;
			break;
		case 1:	// BDEF2
			v->BlendIndices[0] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[2] = 0.0f;
			v->BlendIndices[3] = 0.0f;
			v->BlendWeights[0] = reader->ReadFloat();
			v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
			v->BlendWeights[2] = 0.0f;
			v->BlendWeights[3] = 0.0f;
			break;
		case 2:	// BDEF4
			v->BlendIndices[0] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[2] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[3] = reader->ReadInt(getBoneIndexSize());
			v->BlendWeights[0] = reader->ReadFloat();
			v->BlendWeights[1] = reader->ReadFloat();
			v->BlendWeights[2] = reader->ReadFloat();
			v->BlendWeights[3] = reader->ReadFloat();
			break;
		case 3:	// SDEF
			v->BlendIndices[0] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[1] = reader->ReadInt(getBoneIndexSize());
			v->BlendIndices[2] = 0.0f;
			v->BlendIndices[3] = 0.0f;
			v->BlendWeights[0] = reader->ReadFloat();
			v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
			v->BlendWeights[2] = 0.0f;
			v->BlendWeights[3] = 0.0f;
			reader->Read(&v->SdefC, sizeof(float) * 3);
			reader->Read(&v->SdefR0, sizeof(float) * 3);
			reader->Read(&v->SdefR1, sizeof(float) * 3);	// TODO:���C���l��v�v�Z
			break;
		}

		// �G�b�W�{��
		v->EdgeWeight = reader->ReadFloat();
	}

	m_modelCore->VertexBuffer->Unlock();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadIndices(BinaryReader* reader)
{
	// �C���f�b�N�X��
	int indexCount = reader->ReadInt32();

	Graphics::IndexBufferFormat format = Graphics::IndexBufferFormat_UInt16;
	if (getVertexIndexSize() > 2) {
		format = Graphics::IndexBufferFormat_UInt32;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	m_modelCore->IndexBuffer.Attach(Graphics::IndexBuffer::Create(
		m_manager->GetGraphicsManager(), indexCount, NULL, format, Graphics::DeviceResourceUsage_Dynamic));

	// 1 �o�C�g�C���f�b�N�X
	if (getVertexIndexSize() == 1)
	{
		// TODO:���Ή�
		LN_THROW(0, NotImplementedException);
	}
	// 2 or 4 �o�C�g�C���f�b�N�X
	else
	{
		// ���̂܂܃R�s�[
		byte_t* indices = (byte_t*)m_modelCore->IndexBuffer->Lock();
		reader->Read(indices, getVertexIndexSize() * indexCount);
		m_modelCore->IndexBuffer->Unlock();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadTextureTable(BinaryReader* reader, const PathNameW& baseDir)
{
	// �e�N�X�`����
	int textureCount = reader->ReadInt32();

	// �e�N�X�`���e�[�u��
	m_textureTable.Reserve(textureCount);

	for (int i = 0; i < textureCount; ++i)
	{
		// �e�N�X�`����
		String name = ReadString(reader);

		// �쐬
		PathNameW filePath(baseDir, name);
		m_textureTable.Add(Graphics::Texture::Create(filePath));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadMaterials(BinaryReader* reader)
{
	// �}�e���A����
	int materialCount = reader->ReadInt32();

	// �������m��
	m_modelCore->Material.Materials.Resize(materialCount);
	m_modelCore->Material.Attributes.Resize(materialCount);

	int indexAttrOffset = 0;
	for (int i = 0; i < materialCount; ++i)
	{
		Graphics::Material* m = &m_modelCore->Material.Materials[i];

		// �ގ���
		/*m_modelCore->Material.Name = */ReadString(reader);

		// �ގ��p��
		/*m_modelCore->Material.EnglishName = */ReadString(reader);

		// Diffuse
		reader->Read(&m->Diffuse, sizeof(float) * 4);

		// Specular
		reader->Read(&m->Specular, sizeof(float) * 3);
		m->Specular.A = 1.0f;

		// Specular�W��
		m->Power = reader->ReadFloat();

		// Ambient
		reader->Read(&m->Ambient, sizeof(float) * 3);
		m->Ambient.A = 1.0f;

		// �`��t���O (MMDDrawingFlags)
		uint8_t bitFlag = reader->ReadInt8();
		m->DrawingFlags = bitFlag;

		// �G�b�W�F
		reader->Read(&m->EdgeColor, sizeof(float) * 4);

		// �G�b�W�T�C�Y
		m->EdgeSize = reader->ReadFloat();

		// �e�N�X�`��
		int normalTexture = reader->ReadInt(getTextureIndexSize());
		if (normalTexture >= 0) {
			m->Texture = m_textureTable[normalTexture];
		}

		// �X�t�B�A�e�N�X�`��
		int sphereTexture = reader->ReadInt(getTextureIndexSize());
		m->SphereMode = (enum Graphics::Material::SphereMode)reader->ReadInt8();

		// �g�D�[���e�N�X�`��
		int shareToon = reader->ReadInt8();
		if (shareToon == 0)
		{
			int index = reader->ReadInt(getTextureIndexSize());
			if (index >= 0) {
				m->ToonTexture = m_textureTable[normalTexture];
			}
		}
		// ���LToon�e�N�X�`�� (toon01.bmp�`toon10.bmp)
		else
		{
			int index = reader->ReadInt8();
			m->ToonTexture = m_manager->GetMMDDefaultToonTexture(index);
		}

		// ���R��
		uint32_t byteSize = reader->ReadInt32();
		reader->Seek(byteSize);

		// �}�e���A���ɑΉ����钸�_��
		int vc = reader->ReadInt32();

		// �����e�[�u���𖄂߂�
		MeshAttribute* attr = &m_modelCore->Material.Attributes[i];
		attr->MaterialIndex = i;
		attr->StartIndex = indexAttrOffset;
		attr->PrimitiveNum = vc / 3;
		indexAttrOffset += vc;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadBones(BinaryReader* reader)
{
	// �{�[����
	int boneCount = reader->ReadInt32();

	// �e�{�[�����C���f�b�N�X����E�����߁A�܂��͂��ׂăC���X�^���X��
	m_modelCore->Bones.Resize(boneCount);
	for (int i = 0; i < boneCount; ++i) {
		m_modelCore->Bones[i] = LN_NEW ModelBoneCore(m_modelCore, i);
	}

	// �f�[�^�ǂݍ���
	for (int i = 0; i < boneCount; ++i)
	{
		ModelBoneCore* boneCore = m_modelCore->Bones[i];

		// �{�[����
		boneCore->Name = ReadString(reader);

		// �{�[���p��
		/*boneCore->EnglishName = */ ReadString(reader);

		// �����ʒu
		reader->Read(&boneCore->OrgPosition, sizeof(float) * 3);

		// �e�{�[���̃{�[��Index
		boneCore->ParentBoneIndex = reader->ReadInt(getBoneIndexSize());

		// �ό`�K�w
		boneCore->TransformLevel = reader->ReadInt32();

		// �{�[���t���O
		uint32_t flag = reader->ReadUInt16();
		boneCore->BoneConnect = (flag & 0x0001) != 0 ? BoneConnectType_Bone : BoneConnectType_PositionOffset;
		boneCore->CanRotate = (flag & 0x0002) != 0;
		boneCore->CanMove = (flag & 0x0004) != 0;
		boneCore->IsVisible = (flag & 0x0008) != 0;
		boneCore->CanOperate = (flag & 0x0010) != 0;

		boneCore->IsIK = (flag & 0x0020) != 0;
		boneCore->LocalProvide = (flag & 0x0080) != 0 ? LocalProvideType_ParentLocalTransformValue : LocalProvideType_UserTransformValue;

		boneCore->IsRotateProvided = (flag & 0x0100) != 0;
		boneCore->IsMoveProvided = (flag & 0x0200) != 0;

		boneCore->IsFixAxis = (flag & 0x0400) != 0;
		boneCore->IsLocalAxis = (flag & 0x0800) != 0;

		boneCore->TransformAfterPhysics = (flag & 0x1000) != 0;
		boneCore->ParentTransform = (flag & 0x2000) != 0;

		if (boneCore->BoneConnect == BoneConnectType_PositionOffset)
		{
			// ���W�I�t�Z�b�g, �{�[���ʒu����̑��Ε�
			reader->Read(&boneCore->PositionOffset, sizeof(float) * 3);
		}
		else
		{
			// �ڑ���{�[���̃{�[��Index
			boneCore->ConnectedBoneIndex = reader->ReadInt(getBoneIndexSize());
		}

		// ��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ
		if (boneCore->IsRotateProvided || boneCore->IsMoveProvided)
		{
			// �t�^�e�{�[���̃{�[��Index
			boneCore->ProvidedParentBoneIndex = reader->ReadInt(getBoneIndexSize());
			boneCore->ProvidedRatio = reader->ReadFloat();
		}

		// ���Œ�:1 �̏ꍇ
		if (boneCore->IsFixAxis) {
			reader->Read(&boneCore->AxisDirectionVector, sizeof(float) * 3);
		}

		//  ���[�J����:1 �̏ꍇ
		if (boneCore->IsLocalAxis)
		{
			reader->Read(&boneCore->DimentionXDirectionVector, sizeof(float) * 3);
			reader->Read(&boneCore->DimentionZDirectionVector, sizeof(float) * 3);
		}

		// �O���e�ό`:1 �̏ꍇ
		if (boneCore->ParentTransform) {
			boneCore->KeyValue = reader->ReadInt32();
		}

		// IK:1 �̏ꍇ
		if (boneCore->IsIK)
		{
			ModelIKCore* ik = LN_NEW ModelIKCore();
			m_modelCore->IKs.Add(ik);
			ik->IKBoneIndex = i;							// ���ݏ������̃{�[���ԍ�
			ik->IKTargetBoneIndex = reader->ReadInt(getBoneIndexSize());
			ik->LoopCount = reader->ReadInt32();
			ik->IKRotateLimit = reader->ReadFloat();

			int ikLinkCount = reader->ReadInt32();
			for (int i = 0; i < ikLinkCount; i++)
			{
				ModelIKCore::IKLink ikLink;
				ikLink.LinkBoneIndex = reader->ReadInt(getBoneIndexSize());
				ikLink.IsRotateLimit = (reader->ReadInt8() != 0);
				if (ikLink.IsRotateLimit)
				{
					reader->Read(&ikLink.MinLimit, sizeof(float) * 3);
					reader->Read(&ikLink.MaxLimit, sizeof(float) * 3);
				}

				ik->IKLinks.Add(ikLink);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadMorphs(BinaryReader* reader)
{
	// ���[�t��
	int boneCount = reader->ReadInt32();
	m_modelCore->Morphs.Resize(boneCount);

	// �f�[�^�ǂݍ���
	for (int i = 0; i < boneCount; ++i)
	{
		ModelMorphCore* morphCore = LN_NEW ModelMorphCore();
		m_modelCore->Morphs[i] = morphCore;

		// ���[�t��
		morphCore->Name = ReadString(reader);

		// ���[�t�p��
		/*morphCore->EnglishName = */ ReadString(reader);

		// ����p�l��
		morphCore->OperationPanel = reader->ReadInt8();

		// ���[�t���
		int morphType = reader->ReadInt8();

		// ���[�t�I�t�Z�b�g
		int32_t offsetCount = reader->ReadInt32();
		morphCore->MorphOffsets.Resize(offsetCount);
		for (int i = 0; i < offsetCount; i++)
		{
			ModelMorphCore::MorphOffset* mo = &morphCore->MorphOffsets[i];
			switch (morphType)
			{
			case 0:		// �O���[�v���[�t
				morphCore->MorphType = ModelMorphType_Group;
				mo->GroupMorphOffset.MorphIndex = reader->ReadInt(getMorphIndexSize());
				mo->GroupMorphOffset.MorphRatio = reader->ReadFloat();
				break;
			case 1:		// ���_���[�t
				// VertexIndex �̓��f���{�̂̒��_�C���f�b�N�X
				// PositionOffset �͌��̈ʒu����̑��Έʒu
				morphCore->MorphType = ModelMorphType_Vertex;
				mo->VertexMorphOffset.VertexIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->VertexMorphOffset.PositionOffset, sizeof(float) * 3);
				break;
			case 2:		// �{�[�����[�t
				morphCore->MorphType = ModelMorphType_Bone;
				mo->BoneMorphOffset.BoneIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->BoneMorphOffset.Moving, sizeof(float) * 3);
				reader->Read(&mo->BoneMorphOffset.Rotating, sizeof(float) * 4);
				break;
			case 3:		// UV���[�t
				morphCore->MorphType = ModelMorphType_UV;
				mo->UVMorphOffset.VertexIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 4:		// �ǉ�UV���[�t1
				morphCore->MorphType = ModelMorphType_AdditionalUV1;
				mo->UVMorphOffset.VertexIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 5:		// �ǉ�UV���[�t2
				morphCore->MorphType = ModelMorphType_AdditionalUV2;
				mo->UVMorphOffset.VertexIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 6:		// �ǉ�UV���[�t3
				morphCore->MorphType = ModelMorphType_AdditionalUV3;
				mo->UVMorphOffset.VertexIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 7:		// �ǉ�UV���[�t4
				morphCore->MorphType = ModelMorphType_AdditionalUV4;
				mo->UVMorphOffset.VertexIndex = reader->ReadInt(getVertexIndexSize());
				reader->Read(&mo->UVMorphOffset.UVOffset, sizeof(float) * 4);
				break;
			case 8:		// �ގ����[�t
				morphCore->MorphType = ModelMorphType_Matrial;
				mo->MaterialMorphOffset.MaterialIndex = reader->ReadInt(getMaterialIndexSize());
				mo->MaterialMorphOffset.OffsetCalcType = reader->ReadUInt8();
				reader->Read(&mo->MaterialMorphOffset.Diffuse, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.Specular, sizeof(float) * 3);
				reader->Read(&mo->MaterialMorphOffset.SpecularCoe, sizeof(float) * 1);
				reader->Read(&mo->MaterialMorphOffset.Ambient, sizeof(float) * 3);
				reader->Read(&mo->MaterialMorphOffset.EdgeColor, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.EdgeSize, sizeof(float) * 1);
				reader->Read(&mo->MaterialMorphOffset.TextureCoe, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.SphereTextureCoe, sizeof(float) * 4);
				reader->Read(&mo->MaterialMorphOffset.ToonTextureCoe, sizeof(float) * 4);
				break;
			case 9:		// Flip���[�t
				morphCore->MorphType = ModelMorphType_Flip;
				mo->FlipMorphOffset.MorphIndex = reader->ReadInt(getMorphIndexSize());
				mo->FlipMorphOffset.MorphValue = reader->ReadFloat();
				break;
			case 10:	// Impulse���[�t
				morphCore->MorphType = ModelMorphType_Impulse;
				mo->ImpulseMorphOffset.RigidIndex = reader->ReadInt(getMorphIndexSize());
				mo->ImpulseMorphOffset.LocalFlag = reader->ReadUInt8();
				reader->Read(&mo->ImpulseMorphOffset.Moving, sizeof(float) * 3);
				reader->Read(&mo->ImpulseMorphOffset.Rotating, sizeof(float) * 3);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadDisplayFrame(BinaryReader* reader)
{
	// �\���g�͂��ׂēǂݔ�΂�
	int displayFrameCount = reader->ReadInt32();
	for (int i = 0; i < displayFrameCount; i++)
	{
		// �g��
		uint32_t byteSize = reader->ReadInt32();
		reader->Seek(byteSize);

		// �g���p
		byteSize = reader->ReadInt32();
		reader->Seek(byteSize);

		// ����g�t���O
		reader->Seek(sizeof(uint8_t));

		// �g���v�f��
		int frameElementCount = reader->ReadInt32();
		for (int j = 0; j < frameElementCount; j++)
		{
			// �v�f�Ώ�
			uint8_t type = reader->ReadUInt8();
			switch (type)
			{
			case 0:	// �{�[�����^�[�Q�b�g�̏ꍇ
				reader->ReadInt(getBoneIndexSize());
				break;
			case 1:	// ���[�t���^�[�Q�b�g�̏ꍇ
				reader->ReadInt(getMorphIndexSize());
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadRigidBodys(BinaryReader* reader)
{
	// ���̐�
	int bodyCount = reader->ReadInt32();
	m_modelCore->RigidBodys.Resize(bodyCount);

	// �f�[�^�ǂݍ���
	for (int i = 0; i < bodyCount; ++i)
	{
		ModelRigidBodyCore* bodyCore = LN_NEW ModelRigidBodyCore();
		m_modelCore->RigidBodys[i] = bodyCore;

		// ���̖�
		bodyCore->Name = ReadString(reader);

		// ���̉p��
		/*bodyCore->EnglishName =*/ ReadString(reader);

		// �֘A�{�[��Index - �֘A�Ȃ��̏ꍇ��-1
		bodyCore->RelatedBoneIndex = reader->ReadInt(getBoneIndexSize());

		// �O���[�v
		bodyCore->Group = (1 << reader->ReadUInt8());

		// ��Փ˃O���[�v�t���O
		bodyCore->GroupMask = reader->ReadUInt16();

		// �`�� - 0:�� 1:�� 2:�J�v�Z��
		uint8_t type = reader->ReadUInt8();

		// �T�C�Y(x,y,z)
		Vector3 size;
		reader->Read(&size, sizeof(float) * 3);

		switch (type)
		{
		case 0:
			bodyCore->ColShapeData.Type = CollisionShapeType_Sphere;
			bodyCore->ColShapeData.Sphere.Radius = size.X;
			break;
		case 1:
			bodyCore->ColShapeData.Type = CollisionShapeType_Box;
			bodyCore->ColShapeData.Box.Width = size.X;
			bodyCore->ColShapeData.Box.Height = size.Y;
			bodyCore->ColShapeData.Box.Depth = size.Z;
			break;
		case 2:
			bodyCore->ColShapeData.Type = CollisionShapeType_Capsule;
			bodyCore->ColShapeData.Capsule.Radius = size.X;
			bodyCore->ColShapeData.Capsule.Height = size.Y;
			break;
		}

		// �ʒu(x,y,z)
		Vector3 Position;
		reader->Read(&Position, sizeof(float) * 3);

		// ��](x,y,z) -> ���W�A���p
		Vector3 Rotation;
		reader->Read(&Rotation, sizeof(float) * 3);
		if (Math::IsNaN(Rotation.X)) Rotation.X = 0;	// ���f���ɂ���Ă͉��Ă��邱�Ƃ��������̂Ń��Z�b�g���Ă���
		if (Math::IsNaN(Rotation.Y)) Rotation.Y = 0;
		if (Math::IsNaN(Rotation.Z)) Rotation.Z = 0;

		// �I�t�Z�b�g�s��
		bodyCore->InitialTransform = Matrix::RotationYawPitchRoll(Rotation.Y, Rotation.X, Rotation.Z) * Matrix::Translation(Position);
		//Matrix bias;
		//bias.RotationZ(Rotation.Z);
		//bias.RotationX(Rotation.X);
		//bias.RotationY(Rotation.Y);
		//bias.Translation(Position - m_modelCore->FrameArray[bodyCore->RelatedFrameIndex]->OrgPosition);
		//bodyCore->BoneOffset = bias;
		//bodyCore->InvBoneOffset = Matrix::Inverse(bodyCore->BoneOffset);
		LN_THROW(0, InvalidOperationException);	// ���̂�����v����

		// ���̊�{���
		bodyCore->Mass = reader->ReadFloat();
		bodyCore->LinearDamping = reader->ReadFloat();
		bodyCore->AngularDamping = reader->ReadFloat();
		bodyCore->Restitution = reader->ReadFloat();
		bodyCore->Friction = reader->ReadFloat();

		// ���̂̕������Z - 0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹
		uint8_t physicsType = reader->ReadUInt8();
		switch (physicsType)
		{
		case 0:
			bodyCore->RigidBodyType = RigidBodyType_ControlledByBone;
			break;
		case 1:
			bodyCore->RigidBodyType = RigidBodyType_Physics;
			break;
		case 2:
			bodyCore->RigidBodyType = RigidBodyType_PhysicsAlignment;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PMXLoader::LoadJoints(BinaryReader* reader)
{
	// �W���C���g��
	int jointCount = reader->ReadInt32();
	m_modelCore->Joints.Resize(jointCount);

	// �f�[�^�ǂݍ���
	for (int i = 0; i < jointCount; ++i)
	{
		ModelJointCore* jointCore = LN_NEW ModelJointCore();
		m_modelCore->Joints[i] = jointCore;

		// Joint��
		jointCore->Name = ReadString(reader);

		// Joint���p
		/*jointCore->EnglishName =*/ ReadString(reader);

		// Joint��� - 0:�X�v�����O6DOF   | PMX2.0�ł� 0 �̂�(�g���p)
		int type = reader->ReadUInt8();
		LN_THROW(type == 0, InvalidFormatException);

		// ��� PMD �Ɠ���
		jointCore->RigidBodyAIndex = reader->ReadInt(getRigidBodyIndexSize());
		jointCore->RigidBodyBIndex = reader->ReadInt(getRigidBodyIndexSize());
		reader->Read(&jointCore->Position, sizeof(Vector3));
		reader->Read(&jointCore->Rotation, sizeof(Vector3));
		reader->Read(&jointCore->PositionLimitLower, sizeof(Vector3));
		reader->Read(&jointCore->PositionLimitUpper, sizeof(Vector3));
		reader->Read(&jointCore->RotationLimitLower, sizeof(Vector3));
		reader->Read(&jointCore->RotationLimitUpper, sizeof(Vector3));
		reader->Read(&jointCore->SpringPositionStiffness, sizeof(Vector3));
		reader->Read(&jointCore->SpringRotationStiffness, sizeof(Vector3));

		jointCore->SpringRotationStiffness.X = Math::ToRadians(jointCore->SpringRotationStiffness.X);
		jointCore->SpringRotationStiffness.Y = Math::ToRadians(jointCore->SpringRotationStiffness.Y);
		jointCore->SpringRotationStiffness.Z = Math::ToRadians(jointCore->SpringRotationStiffness.Z);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StringW PMXLoader::ReadString(BinaryReader* reader)
{
	uint32_t byteSize = reader->ReadInt32();
	m_tmpBuffer.Resize(byteSize);
	reader->Read(m_tmpBuffer.GetData(), byteSize);

	StringW str;
	if (getEncode() == PMX_Encode_UTF16) {
		str.ConvertFrom(m_tmpBuffer.GetData(), byteSize, Text::Encoding::GetUTF16Encoding());
	}
	else {
		str.ConvertFrom(m_tmpBuffer.GetData(), byteSize, Text::Encoding::GetUTF8Encoding());
	}

	return str;
}

} // namespace Modeling
} // namespace Lumino
