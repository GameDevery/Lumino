
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "BitmapTextRenderer.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// BitmapTextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapTextRenderer::BitmapTextRenderer()
	: m_manager(nullptr)
	, m_tmpGlyphRun(nullptr)
	, m_textAlignment(TextAlignment::Left)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapTextRenderer::~BitmapTextRenderer()
{
	LN_SAFE_RELEASE(m_tmpGlyphRun);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapTextRenderer::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;

	m_tmpGlyphRun = LN_NEW GlyphRun();
	m_tmpGlyphRun->Initialize(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapTextRenderer::DrawGlyphRun(Bitmap* target, GlyphRun* glyphRun, const Color& fillColor, const Color& strokeColor, int strokeThickness)
{
	LN_CHECK_ARGS_RETURN(target != nullptr);
	LN_CHECK_ARGS_RETURN(glyphRun != nullptr);

	Font* font = glyphRun->GetFont();

	auto& items = glyphRun->RequestLayoutItems();
	auto& renderSize = glyphRun->GetRenderSize();

	Point offset(0, 0);
	switch (m_textAlignment)
	{
		case TextAlignment::Left:
		{
			break;
		}
		case TextAlignment::Center:
		{
			offset.X = (m_renderArea.Width - renderSize.Width) / 2;
			break;
		}
		case TextAlignment::Right:
		{
			offset.X = m_renderArea.GetRight() - renderSize.Width;
			break;
		}
		case TextAlignment::Justify:
			LN_THROW(0, NotImplementedException);
			break;
	}


	Point pos(m_renderArea.X + offset.X, m_renderArea.Y + offset.Y);
	for (auto& item : items)
	{
		FontGlyphBitmap* gb = font->LookupGlyphBitmap(item.Char, strokeThickness);
		Rect dstRect;
		Rect srcRect;

		// �g���p�r�b�g�}�b�v������ꍇ�͐�ɕ`�悷��
		if (gb->OutlineBitmap != nullptr)
		{
			dstRect.Set(
				pos.X + item.Location.OutlineBitmapTopLeftPosition.X,
				pos.Y + item.Location.OutlineBitmapTopLeftPosition.Y,
				item.Location.BitmapSize.Width,
				item.Location.BitmapSize.Height);
			srcRect.Set(0, 0, gb->OutlineBitmap->GetSize());
			target->BitBlt(dstRect, gb->OutlineBitmap, srcRect, strokeColor, true);
		}

		// ���� (or �ʏ�) �����̕`��
		if (gb->GlyphBitmap != nullptr)
		{
			dstRect.Set(
				pos.X + item.Location.BitmapTopLeftPosition.X,
				pos.Y + item.Location.BitmapTopLeftPosition.Y,
				item.Location.BitmapSize.Width,
				item.Location.BitmapSize.Height);
			srcRect.Set(0, 0, gb->GlyphBitmap->GetSize());
			target->BitBlt(dstRect, gb->GlyphBitmap, srcRect, fillColor, true);
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
