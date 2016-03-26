
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class GraphicsManager;
class SpriteRenderer;

/**
	@brief	
*/
class TileMapRenderer
	: public RefObject
{
public:
	void Draw(SpriteRenderer* spriteRenderer, TileMap* tileMap, const RectF& boundingRect, const ViewFrustum& cameraFrustum);

protected:
	void Begin();

	void End();

	void DrawTile(
		const Vector3& position,
		const Vector2& size,
		Texture* texture,
		const Rect& srcRect);

protected:
	TileMapRenderer(GraphicsManager* manager);
	virtual ~TileMapRenderer();

private:
	void DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet, const Rect& renderRange);

	GraphicsManager*	m_graphicsManager;
	VertexBuffer*		m_vertexBuffer;
	IndexBuffer*		m_indexBuffer;

	SpriteRenderer*	m_spriteRenderer;
};

///**
//	@brief	
//*/
//class SpriteTileMapRenderer
//	: public TileMapRenderer
//{
//public:
//	SpriteTileMapRenderer(GraphicsManager* manager);
//	virtual ~SpriteTileMapRenderer();
//
//	/**
//		@brief		���W�ϊ��s���ݒ肵�܂��B
//		@param[in]	matrix		: ���W�ϊ��s��
//	*/
//	void SetTransform(const Matrix& matrix);
//
//	/**
//		@brief		�r���[�y�уv���W�F�N�V�����s���ݒ肵�܂��B
//	*/
//	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);
//
//	/**
//		@brief		�����_�����O�X�e�[�g��ݒ肵�܂��B
//	*/
//	void SetRenderState(const RenderState& state);
//
//protected:
//	virtual void Begin();
//	virtual void End();
//	virtual void DrawTile(
//		const Vector3& position,
//		const Vector2& size,
//		Texture* texture,
//		const Rect& srcRect);
//
//private:
//	const int MaxSpriteCount = 2048;
//	SpriteRenderer*	m_spriteRenderer;
//};

LN_NAMESPACE_END
