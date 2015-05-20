
#pragma once
#include <Lumino/IO/BinaryReader.h>
#include "Common.h"
#include "ModelCore.h"

namespace Lumino
{
namespace Modeling
{

class PMXLoader
{
public:

#pragma pack( push, 1 )

	enum PMX_Encode
	{
		PMX_Encode_UTF16	= 0,
		PMX_Encode_UTF8		= 1,
	};

	struct PMX_Header
	{
		char	Magic[4];		// "PMX " (PMX1.0��"Pmx ")
		float	Version;		// PMD�o�[�W�����ԍ�
		int		DataSize;		// �㑱����f�[�^��̃o�C�g�T�C�Y  PMX2.0�� 8 �ŌŒ�
		byte_t	Data[8];		// [0] - �G���R�[�h����  | 0:UTF16 1:UTF8
								// [1] - �ǉ�UV�� 	| 0�`4 �ڍׂ͒��_�Q��
								// [2] - ���_Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [3] - �e�N�X�`��Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [4] - �ގ�Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [5] - �{�[��Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [6] - ���[�tIndex�T�C�Y | 1,2,4 �̂����ꂩ
								// [7] - ����Index�T�C�Y | 1,2,4 �̂����ꂩ
	};

#pragma pack( pop )

public:
	~PMXLoader();

public:

	/// �ǂݍ���
	///		baseDir	: .pmx �t�@�C���̑��݂���t�H���_�p�X
	ModelCore* Load(ModelManager* manager, Stream* stream, const PathNameW& baseDir, bool isDynamic);

private:
	PMX_Encode getEncode() { return (PMX_Encode)m_pmxHeader.Data[0]; }
	int getAdditionalUVCount() { return m_pmxHeader.Data[1]; }
	int getVertexIndexSize() { return m_pmxHeader.Data[2]; }
	int getTextureIndexSize() { return m_pmxHeader.Data[3]; }
	int getMaterialIndexSize() { return m_pmxHeader.Data[4]; }
	int getBoneIndexSize() { return m_pmxHeader.Data[5]; }
	int getMorphIndexSize() { return m_pmxHeader.Data[6]; }
	int getRigidBodyIndexSize() { return m_pmxHeader.Data[7]; }

private:
	void LoadModelInfo(BinaryReader* reader);
	void LoadVertices(BinaryReader* reader);
	void LoadIndices(BinaryReader* reader);
	void LoadTextureTable(BinaryReader* reader, const PathNameW& baseDir);
	void LoadMaterials(BinaryReader* reader);
	void LoadBones(BinaryReader* reader);
	void LoadMorphs(BinaryReader* reader);
	void LoadDisplayFrame(BinaryReader* reader);
	void LoadRigidBodys(BinaryReader* reader);
	void LoadJoints(BinaryReader* reader);

	StringW ReadString(BinaryReader* reader);

private:
	ModelManager*					m_manager;
	bool							m_isDynamic;
	//uint32_t						mFlags;
	RefPtr<ModelCore>				m_modelCore;		///< �ŏI�o��
	PMX_Header						m_pmxHeader;
	ArrayList<Graphics::Texture*>	m_textureTable;
	ByteBuffer						m_tmpBuffer;
};

} // namespace Modeling
} // namespace Lumino
