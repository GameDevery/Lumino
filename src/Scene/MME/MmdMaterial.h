
#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Scene/VisualNode.h>

LN_NAMESPACE_BEGIN

/** �`��I�v�V���� */
LN_ENUM_FLAGS(MmdDrawingFlags)
{
	None			= 0x0000,		/**< �w�薳�� */
	CullingDouble	= 0x0001,		/**< ���ʕ`�� */
	GroundShadow	= 0x0002,		/**< �n�ʉe */
	SelfShadowMap	= 0x0004,		/**< �Z���t�V���h�E�}�b�v�ւ̕`�� */
	SelfShadow		= 0x0008,		/**< �Z���t�V���h�E�̕`�� */
	Edge			= 0x0010,		/**< �G�b�W�`�� */
};
LN_ENUM_FLAGS_DECLARE(MmdDrawingFlags);

/** �X�t�B�A�e�N�X�`���̍������[�h */
enum class MmdSphereMode
{
	Disable			= 0,			/**< ���� */
	Mul				= 1,			/**< ��Z(sph) */
	Add				= 2,			/**< ���Z(spa) */
	SubTexture		= 3,			/**< �T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��) */
};

/**
	@brief
*/
class MmdMaterialInstance
	: public detail::MaterialInstance
{
public:
	//void SetToonTexture(Texture* texture);
	//Texture* GetToonTexture() const { return m_toonTexture; }
	//void SetSphereTexture(Texture* texture);
	//Texture* GetSphereTexture() const { return m_sphereTexture; }

	virtual void OnCombine(Material* owner, Material* parent) override;

LN_INTERNAL_ACCESS:
	MmdMaterialInstance();
	virtual ~MmdMaterialInstance();

public:	// TODO:
	Color		m_diffuse;			// ���̂̐F
	Color		m_ambient;			// ����
	Color		m_specular;			// ����
	Color		m_emissive;			// ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
	float		m_power;			// ����̋���

	RefPtr<Texture>	m_materialTexture;
	RefPtr<Texture>	m_toonTexture;
	RefPtr<Texture>	m_sphereTexture;

	Color						ToonColor;			///< [PMD] �g�D�[���J���[
	Color						EdgeColor;			///< [PMX] �G�b�W�J���[
	float						EdgeSize;			///< [PMX] �G�b�W�T�C�Y
	Color						TextureCoe;			///< [PMX] �e�N�X�`���W��
	Color						SphereTextureCoe;	///< [PMX] �X�t�B�A�e�N�X�`���W��
	Color						ToonTextureCoe;		///< [PMX] Toon�e�N�X�`���W��
	uint32_t					DrawingFlags;		///< [PMX] �`��I�v�V���� (MMDDrawingFlags �̑g�ݍ��킹)
	MmdSphereMode				SphereMode;			///< [PMX] �X�t�B�A�e�N�X�`���̍������[�h
};

LN_NAMESPACE_END
