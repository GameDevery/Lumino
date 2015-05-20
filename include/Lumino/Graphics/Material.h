
#pragma once

#include "Color.h"
#include "Texture.h"
#include "Shader.h"

namespace Lumino
{
namespace Graphics
{
/**
	@brief	�}�e���A���̃N���X�ł��B
*/
class Material
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
	ColorF						Diffuse;			///< ���̂̐F
	ColorF						Ambient;			///< ����
	ColorF						Specular;			///< ����
	ColorF						Emissive;			///< ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
	float						Power;				///< ����̋���

	RefPtr<Graphics::Shader>	Shader;				///< �V�F�[�_
	RefPtr<Graphics::Texture>	Texture;			///< �e�N�X�`��
	RefPtr<Graphics::Texture>	ToonTexture;		///< [PMD] �g�D�[���e�N�X�`�� (getToonTexture() setToonTexture()�ŃA�N�Z�X����)
	RefPtr<Graphics::Texture>	SphereTexture;		///< [PMD] �X�t�B�A�e�N�X�`�� (getSphereTexture() setSphereTexture()�ŃA�N�Z�X����)

	ColorF						ToonColor;			///< [PMD] �g�D�[���J���[
	ColorF						EdgeColor;			///< [PMX] �G�b�W�J���[
	float						EdgeSize;			///< [PMX] �G�b�W�T�C�Y
	ColorF						TextureCoe;			///< [PMX] �e�N�X�`���W��
	ColorF						SphereTextureCoe;	///< [PMX] �X�t�B�A�e�N�X�`���W��
	ColorF						ToonTextureCoe;		///< [PMX] Toon�e�N�X�`���W��
	uint32_t					DrawingFlags;		///< [PMX] �`��I�v�V���� (MMDDrawingFlags �̑g�ݍ��킹)
	SphereMode					SphereMode;			///< [PMX] �X�t�B�A�e�N�X�`���̍������[�h

public:

	/**
		@brief		�}�e���A�������������܂��B
	*/
	Material()
	{
		Diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
		Ambient.Set(0.0f, 0.0f, 0.0f, 0.0f);
		Specular.Set(0.5f, 0.5f, 0.5f, 0.5f);
		Emissive.Set(0.0f, 0.0f, 0.0f, 0.0f);
		Power = 50.0f;
		ToonColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		EdgeColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		EdgeSize = 0.0f;
		SphereMode = SphereMode_Disable;
		DrawingFlags = 0;
	}
};

} // namespace Graphics
} // namespace Lumino
