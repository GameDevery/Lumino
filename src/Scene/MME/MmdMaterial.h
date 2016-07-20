
#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Scene/Material.h>

LN_NAMESPACE_BEGIN

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

	virtual void OnCombine(Material3* owner, Material3* parent) override;

LN_INTERNAL_ACCESS:
	MmdMaterialInstance();
	virtual ~MmdMaterialInstance();

public:	// TODO:
	Color		m_diffuse;			// ���̂̐F
	Color		m_ambient;			// ����
	Color		m_specular;			// ����
	Color		m_emissive;			// ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
	float		m_power;			// ����̋���

	RefPtr<Texture>	m_toonTexture;
	RefPtr<Texture>	m_sphereTexture;

	Color						ToonColor;			///< [PMD] �g�D�[���J���[
	Color						EdgeColor;			///< [PMX] �G�b�W�J���[
	float						EdgeSize;			///< [PMX] �G�b�W�T�C�Y
	Color						TextureCoe;			///< [PMX] �e�N�X�`���W��
	Color						SphereTextureCoe;	///< [PMX] �X�t�B�A�e�N�X�`���W��
	Color						ToonTextureCoe;		///< [PMX] Toon�e�N�X�`���W��
	uint32_t					DrawingFlags;		///< [PMX] �`��I�v�V���� (MMDDrawingFlags �̑g�ݍ��킹)
	int/*Material::SphereMode*/		SphereMode;			///< [PMX] �X�t�B�A�e�N�X�`���̍������[�h
};

LN_NAMESPACE_END
