
#include "../Internal.h"
#include <Lumino/Graphics/TextRenderer.h>
#include "TextRendererImplemented.h"

namespace Lumino
{
namespace Graphics
{
namespace detail
{

//=============================================================================
// TextRendererImplemented
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRendererImplemented::TextRendererImplemented(GraphicsManager* manager)
{
	int maxCharacters = 2048;// TODO �萔�Ȃ̂͂Ȃ�Ƃ�������
	m_spriteRenderer.Attach(LN_NEW SpriteRendererImpl(manager, maxCharacters));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRendererImplemented::~TextRendererImplemented()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererImplemented::DrawChar(UTF32 ch, const Rect& area)
{
	CachedGlyphInfoMap::iterator itr = m_cachedGlyphInfoMap.find(ch);
	if (itr != m_cachedGlyphInfoMap.end())
	{
		// �`��
		DrawSprite(itr->second, area.X, area.Y);
	}
	else
	{
		if (m_indexStack.GetCount() == 0) {
			// TODO: �Â��L���b�V���j��
			LN_THROW(0, NotImplementedException);
		}

		// �V��������ăL���b�V���ɓo�^
		Imaging::FontGlyphData* glyhp = m_font->LookupGlyphData(ch, NULL);

		int cacheIndex = m_indexStack.GetTop();
		m_indexStack.Pop();

		{
			Device::ITexture::ScopedLock lock(m_glyphCacheTexture);

			Rect destRect(
				(cacheIndex % m_glyphWidthCount) * m_glyphMaxBitmapSize.Width,
				(cacheIndex / m_glyphWidthCount) * m_glyphMaxBitmapSize.Height,
				m_glyphMaxBitmapSize);
			Rect srcRect(
				0, 0,
				glyhp->GlyphBitmap->GetSize());
			lock.GetBitmap()->BitBlt(destRect, glyhp->GlyphBitmap, srcRect, Color::White, false);
		}

		// �L���b�V���}�b�v�ɓo�^
		CachedGlyphInfo e;
		e.Index = cacheIndex;
		e.Size = glyhp->GlyphBitmap->GetSize();
		m_cachedGlyphInfoMap[ch] = e;

		// �`��
		DrawSprite(e, area.X, area.Y);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererImplemented::Flash()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererImplemented::DrawSprite(const CachedGlyphInfo& info, int x, int y)
{
	RectF srcRect(
		(info.Index % m_glyphWidthCount) * m_glyphMaxBitmapSize.Width,
		(info.Index / m_glyphWidthCount) * m_glyphMaxBitmapSize.Height,
		info.Size.Width, info.Size.Height);

	const ColorF c[4] = { ColorF::White, ColorF::White, ColorF::White, ColorF::White };
	m_spriteRenderer->DrawRequest2D(
		Vector3(x, y, 0),
		Vector3::Zero,
		Vector2(info.Size.Width, info.Size.Height),
		m_glyphCacheTexture,
		srcRect,
		c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererImplemented::Reset()
{
	int maxCharacters = 2048;// TODO �萔�Ȃ̂͂Ȃ�Ƃ�������

	// �������ɕ��ׂ鐔
	// +1.0 �͐؂�̂đ΍�B�e�N�X�`���T�C�Y��maxCharacters�����܂�傫���ł���Ηǂ��B
	// (�������Ȃ����OK)
	m_glyphWidthCount = (int)(sqrt((double)maxCharacters) + 1.0);
	int w = m_glyphWidthCount * m_font->GetLineHeight();	//TODO �r�b�g�}�b�v�����܂�T�C�Y�͗v�`�F�b�N

	// �L���b�V���p�e�N�X�`���쐬
	Device::IGraphicsDevice* device = m_spriteRenderer->GetManager()->GetGraphicsDevice()->GetDeviceObject();
	m_glyphCacheTexture.Attach(device->CreateTexture(Size(w, w), 1, TextureFormat_R8G8B8A8));

	// �󂫃L���b�V���C���f�b�N�X�쐬
	for (int i = 0; i < maxCharacters; i++) {
		m_indexStack.Push(i);
	}

	m_glyphMaxBitmapSize = m_font->GetGlyphMaxSize();
}

} // namespace detail

//=============================================================================
// TextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer* TextRenderer::Create(GraphicsManager* manager)
{
	return LN_NEW TextRenderer(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::TextRenderer(GraphicsManager* manager)
{
	m_manager = manager;
	//m_impl = LN_NEW TextRendererImplemented(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
	//LN_SAFE_RELEASE(m_impl);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const char* text, int length)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const wchar_t* text, int length)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::Flash()
{
}

} // namespace Graphics
} // namespace Lumino
