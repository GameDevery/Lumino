
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class GraphicsManager;
class SpriteRenderer;

/**
	@brief	
*/
class TileMapRenderer
{
public:
	void Draw(TileMap* tileMap, const RectF& boundingRect);

protected:
	virtual void Begin() = 0;

	virtual void End() = 0;

	virtual void DrawTile(
		const Vector3& position,
		const Vector2& size,
		Texture* texture,
		const Rect& srcRect) = 0;

protected:
	virtual ~TileMapRenderer() = default;

private:
	void DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet);
};

/**
	@brief	
*/
class SpriteTileMapRenderer
	: public TileMapRenderer
{
public:
	SpriteTileMapRenderer(GraphicsManager* manager);
	virtual ~SpriteTileMapRenderer();

	/**
		@brief		���W�ϊ��s���ݒ肵�܂��B
		@param[in]	matrix		: ���W�ϊ��s��
	*/
	void SetTransform(const Matrix& matrix);

	/**
		@brief		�r���[�y�уv���W�F�N�V�����s���ݒ肵�܂��B
	*/
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	/**
		@brief		�����_�����O�X�e�[�g��ݒ肵�܂��B
	*/
	void SetRenderState(const RenderState& state);

protected:
	virtual void Begin();
	virtual void End();
	virtual void DrawTile(
		const Vector3& position,
		const Vector2& size,
		Texture* texture,
		const Rect& srcRect);

private:
	const int MaxSpriteCount = 2048;
	SpriteRenderer*	m_spriteRenderer;
};

LN_NAMESPACE_END
