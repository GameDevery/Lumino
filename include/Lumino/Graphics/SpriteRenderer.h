
#pragma once
#include "Common.h"

namespace Lumino
{
namespace Graphics
{
class SpriteRendererImpl;

/**
	@brief		�X�v���C�g�̕`����s���N���X�ł��B
*/
class SpriteRenderer
	: public RefObject
{
public:

	/**
		@brief		SpriteRenderer �̃C���X�^���X���쐬���܂��B
		@param[in]	maxSpriteCount	: ��x�̃o�b�`�`��ŕ`��ł���ő�X�v���C�g��
		@param[in]	manager			: �쐬�Ɏg�p���� GraphicsManager
	*/
	static SpriteRenderer* Create(int maxSpriteCount, GraphicsManager* manager);

public:

	/**
		@brief		���W�ϊ��s���ݒ肵�܂��B
		@param[in]	matrix		: ���W�ϊ��s��
		@details	���� DrawRequest2D �܂��� DrawRequest3D �ŕ`��v�������X�v���C�g�ɑ΂��ēK�p������W�ϊ��s��ł��B
	*/
	void SetTransform(const Matrix& matrix);

	/**
		@brief		�r���[�y�уv���W�F�N�V�����s���ݒ肵�܂��B
	*/
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	/**
		@brief		�`���̃r���[�T�C�Y��ݒ肵�܂��B
	*/
	void SetViewPixelSize(const Size& size);

	/**
		@brief		�����_�����O�X�e�[�g��ݒ肵�܂��B
		@details	���� DrawRequest2D �܂��� DrawRequest3D �ŕ`��v�������X�v���C�g�ɑ΂��ēK�p���郌���_�����O�X�e�[�g�ł��B
	*/
	void SetRenderState(const RenderState& state);

	/**
		@brief		�X�v���C�g�̃\�[�g�Ɋւ������ݒ肵�܂��B
		@param[in]	flags		: 
		@param[in]	basis		: �\�[�g�̊
		@details	���̐ݒ�͎��� Flash �Ŏg�p���܂��B
	*/
	void SetSortMode(uint32_t flags, SortingDistanceBasis basis);	// TODO: enum flags
	
	/**
		@brief		�X�v���C�g�̕`���v�����܂��B
		@param[in]	position	: 
		@param[in]	center		: 
		@param[in]	size		: 
		@param[in]	texture		: 
		@param[in]	srcRect		: 
		@param[in]	colorTable	: 
	*/
	void DrawRequest2D(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Texture* texture,
		const RectF& srcRect,
		const ColorF* colorTable);

	/**
		@brief		�X�v���C�g�̕`���v�����܂��B
		@param[in]	position	: 
		@param[in]	center		: 
		@param[in]	size		: 
		@param[in]	texture		: 
		@param[in]	srcRect		: 
		@param[in]	colorTable	: 
		@param[in]	front		:
	*/
	void DrawRequest3D(
		const Vector3& position,
		const Vector3& center,
		const Vector2& size,
		Texture* texture,
		const RectF& srcRect,
		const ColorF* colorTable,
		AxisDirection front);

	/**
		@brief		�v������Ă���X�v���C�g��S�ĕ`�悵�܂��B
	*/
	void Flash();

	GraphicsManager* GetManager() const { return m_manager; }

private:
	SpriteRenderer(GraphicsManager* manager, int maxSpriteCount);
	virtual ~SpriteRenderer();
	GraphicsManager*	m_manager;
	SpriteRendererImpl*	m_impl;
};

} // namespace Graphics
} // namespace Lumino
