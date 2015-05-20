
#pragma once

#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

namespace Lumino
{
namespace Scene
{

/// Sprite
class Sprite
	: public VisualNode
{
public:
	Sprite();
	virtual ~Sprite();

public:

	/// �쐬
	void Create(SpriteCoord spriteCoord = SpriteCoord_RX, SceneGraphManager* manager = NULL);

	/// �e�N�X�`���̐ݒ�
	void SetTexture(Graphics::Texture* texture);

	/// �e�N�X�`���̎擾
	Graphics::Texture* GetTexture() const;

	/// �T�C�Y�̐ݒ�
	void SetSize(const SizeF& size) { m_size = size; UpdateVertexData(); }

	/// �e�N�X�`���̓]����`�̐ݒ� (�s�N�Z���P��)
	void SetSrcRect(const Rect& rect);

	/// �e�N�X�`���̓]����`�̎擾 (�s�N�Z���P��)
	const Rect& GetSrcRect() const { return m_srcRect; }

	/// ���]���@�̐ݒ�
	void SetFlipMode(FlipMode mode) { m_flipMode = mode; UpdateTexUV(); }

	/// ���]���@�̎擾
	FlipMode GetFlipMode() const { return m_flipMode; }

private:
	void UpdateTexUV();
	void UpdateVertexData();

	/// �s�N�Z���P�ʂ̓]����`���e�N�X�`��UV�̋�` (0.0�`1.0) �ɕϊ�����
	static void NormalizeSrcRect(const Rect& srcRect, const Size& textureSize, float* l, float* t, float* r, float* b);

protected:
	virtual void DrawSubset(RenderingParams& params, int subsetIndex);

private:
	SpriteCoord	m_spriteCoord;
	SizeF		m_size;
	Rect		m_srcRect;
	FlipMode	m_flipMode;

	Vector3		m_upperLeft;
	Vector3		m_lowerRight;
	Vector2		m_upperLeftUV;
	Vector2		m_lowerRightUV;
};

} // namespace Scene
} // namespace Lumino
