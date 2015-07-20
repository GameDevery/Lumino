/*
	FreeType �̓X���b�h�Z�[�t�ł͂Ȃ��B2.6 �ł� FaceNew �������I�ɔr����������炵�����A
	�����ł� FT_Library ���X���b�h���Ƃɍ��̂���ԊȒP�ƌ����Ă���B
	�ł�����ł͑S�������Ǘ��N���X��2�ł��邱�ƂɂȂ�A�g�p��������������B
*/
#include "../Internal.h"
#include <Lumino/Imaging/BitmapPainter.h>
#include <Lumino/Graphics/TextRenderer.h>
#include "TextRendererImplemented.h"

namespace Lumino
{
namespace Graphics
{
namespace detail
{


	//class CharTextureCache
	//{
	//private:
	//	struct CachedGlyphInfo
	//	{
	//		int		Index;
	//		Size	Size;
	//	};

	//	typedef std::map<UTF32, CachedGlyphInfo> CachedGlyphInfoMap;

	//	CachedGlyphInfoMap			m_cachedGlyphInfoMap;
	//	RefPtr<Device::ITexture>	m_glyphCacheTexture;
	//	RefPtr<Imaging::Font>		m_font;


	//public:

	//	CharTextureCache()
	//		: m_cachedGlyphInfoMap()
	//		, m_glyphCacheTexture()
	//		, m_font(NULL)
	//	{
	//	}

	//	void SetFont(Imaging::Font* font)
	//	{
	//		m_font = font;
	//	}

	//	void DrawCharSprite(UTF32 ch)
	//	{

	//	}
	//};












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
		(float)((info.Index % m_glyphWidthCount) * m_glyphMaxBitmapSize.Width),
		(float)((info.Index / m_glyphWidthCount) * m_glyphMaxBitmapSize.Height),
		(float)info.Size.Width, (float)info.Size.Height);

	const ColorF c[4] = { ColorF::White, ColorF::White, ColorF::White, ColorF::White };
	m_spriteRenderer->DrawRequest2D(
		Vector3(x, y, 0),
		Vector3::Zero,
		Vector2((float)info.Size.Width, (float)info.Size.Height),
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
	Device::IGraphicsDevice* device = m_spriteRenderer->GetManager()->GetGraphicsDevice();
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
	: m_spriteRenderer()
	, m_font()
	, m_foreColor()
	, m_strokeColor()
	, m_strokeSize(0)
	, m_textAlignment(TextAlignment::Left)
	, m_textTrimming(TextTrimming::None)
	, m_flowDirection(FlowDirection::LeftToRight)
	, m_prevGlyphLocationData(NULL)
	, m_fontFaceModified(true)
{
	//m_manager = manager;
	//m_impl = LN_NEW TextRendererImplemented(manager);
	m_spriteRenderer.Attach(SpriteRenderer::Create(2048, manager));	// TODO �萔
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
void TextRenderer::SetTransform(const Matrix& matrix)
{
	m_spriteRenderer->SetTransform(matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize)
{
	m_spriteRenderer->SetViewProjMatrix(view, proj);
	m_spriteRenderer->SetViewPixelSize(viewPixelSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const char* text, int length)
{
	length = (length < 0) ? strlen(text) : length;
	Text::EncodingConversionResult result;
	const ByteBuffer& utf32Buf = m_font->GetManager()->GetCharToUTF32Converter()->Convert(text, length * sizeof(char), &result);
	const UTF32* utf32 = (const UTF32*)utf32Buf.GetConstData();
	for (size_t i = 0; i < result.CharsUsed; ++i)
	{
		DrawChar(utf32[i]);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const wchar_t* text, int length)
{
	length = (length < 0) ? wcslen(text) : length;
	Text::EncodingConversionResult result;
	const ByteBuffer& utf32Buf = m_font->GetManager()->GetWCharToUTF32Converter()->Convert(text, length * sizeof(wchar_t), &result);
	const UTF32* utf32 = (const UTF32*)utf32Buf.GetConstData();
	for (size_t i = 0; i < result.CharsUsed; ++i)
	{
		DrawChar(utf32[i]);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::Flash()
{
	m_spriteRenderer->Flash();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawChar(UTF32 ch)
{
	CheckResetCache();
	m_prevGlyphLocationData = m_font->AdvanceKerning(ch, m_prevGlyphLocationData);

	CachedGlyphInfoMap::iterator itr = m_cachedGlyphInfoMap.find(ch);
	if (itr != m_cachedGlyphInfoMap.end())
	{
		// �`��
		DrawSprite(itr->second, m_prevGlyphLocationData->OutlineBitmapTopLeftPosition);
	}
	else
	{
		if (m_indexStack.GetCount() == 0) {
			// TODO: �Â��L���b�V���j��
			LN_THROW(0, NotImplementedException);
		}

		// �V��������ăL���b�V���ɓo�^
		//Imaging::FontGlyphData* glyhp = m_font->LookupGlyphData(ch, NULL);
		Imaging::FontGlyphBitmap* glyphBitmap = m_font->LookupGlyphBitmap(ch);

		int cacheIndex = m_indexStack.GetTop();
		m_indexStack.Pop();

		//m_tmpBitmap

		//{
		//	Device::ITexture::ScopedLock lock(m_glyphCacheTexture);

		// m_tmpBitmap �� BitBlt ���邱�ƂŁA�A�E�g���C���̃r�b�g�}�b�v�ƌ������A�t�H�[�}�b�g�����낦��

		if (glyphBitmap->OutlineBitmap != NULL)
		{
			m_tmpBitmap->SetSize(glyphBitmap->OutlineBitmap->GetSize());
			Rect rc(0, 0, glyphBitmap->OutlineBitmap->GetSize());
			m_tmpBitmap->BitBlt(rc, glyphBitmap->OutlineBitmap, rc, Color::Green, false);
		}
		else {
			m_tmpBitmap->SetSize(glyphBitmap->GlyphBitmap->GetSize());
		}
		Rect dst(glyphBitmap->OutlineOffset, glyphBitmap->OutlineOffset, glyphBitmap->GlyphBitmap->GetSize());
		Rect src(0, 0, glyphBitmap->GlyphBitmap->GetSize());
		m_tmpBitmap->BitBlt(dst, glyphBitmap->GlyphBitmap, src, Color::Red, false);
		//Imaging::BitmapPainter painter(m_tmpBitmap);
		//painter.Clear(Color::Blue);




		Point pt(
			(cacheIndex % m_glyphWidthCount) * m_glyphMaxBitmapSize.Width,
			(cacheIndex / m_glyphWidthCount) * m_glyphMaxBitmapSize.Height);
		m_glyphCacheTexture->SetSubData(pt, m_tmpBitmap);

			//Rect destRect(
			//	(cacheIndex % m_glyphWidthCount) * m_glyphMaxBitmapSize.Width,
			//	(cacheIndex / m_glyphWidthCount) * m_glyphMaxBitmapSize.Height,
			//	m_glyphMaxBitmapSize);
			//Rect srcRect(
			//	0, 0,
			//	glyhp->GlyphBitmap->GetSize());
			//lock.GetBitmap()->BitBlt(destRect, glyhp->GlyphBitmap, srcRect, Color::White, false);
		//}

		// �L���b�V���}�b�v�ɓo�^
		CachedGlyphInfo e;
		e.Index = cacheIndex;
		e.Size = glyphBitmap->GlyphBitmap->GetSize();
		m_cachedGlyphInfoMap[ch] = e;

		// �`��
		DrawSprite(e, m_prevGlyphLocationData->OuterTopLeftPosition);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawSprite(const CachedGlyphInfo& info, const Point& point)
{
	RectF srcRect(
		(float)((info.Index % m_glyphWidthCount) * m_glyphMaxBitmapSize.Width),
		(float)((info.Index / m_glyphWidthCount) * m_glyphMaxBitmapSize.Height),
		(float)info.Size.Width, (float)info.Size.Height);

	const ColorF c[4] = { ColorF::White, ColorF::White, ColorF::White, ColorF::White };
	m_spriteRenderer->DrawRequest2D(
		Vector3(point.X, point.Y, 0),
		Vector3::Zero,
		Vector2(info.Size.Width, info.Size.Height),
		m_glyphCacheTexture,
		srcRect,
		c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::CheckResetCache()
{
	if (m_fontFaceModified)
	{
		int maxCharacters = 2048;// TODO �萔�Ȃ̂͂Ȃ�Ƃ�������

		// �������ɕ��ׂ鐔
		// +1.0 �͐؂�̂đ΍�B�e�N�X�`���T�C�Y��maxCharacters�����܂�傫���ł���Ηǂ��B
		// (�������Ȃ����OK)
		m_glyphWidthCount = (int)(sqrt((double)maxCharacters) + 1.0);
		int w = m_glyphWidthCount * m_font->GetLineHeight();	//TODO �r�b�g�}�b�v�����܂�T�C�Y�͗v�`�F�b�N

		// �L���b�V���p�e�N�X�`���쐬
		m_glyphCacheTexture.Attach(Texture::Create(Size(w, w), TextureFormat_R8G8B8A8, 1, m_spriteRenderer->GetManager()));
		//Device::IGraphicsDevice* device = m_spriteRenderer->GetManager()->GetGraphicsDevice()->GetDeviceObject();
		//m_glyphCacheTexture.Attach(device->CreateTexture(Size(w, w), 1, TextureFormat_R8G8B8A8));


		//Device::IGraphicsDevice::ScopedLockContext lock(m_spriteRenderer->GetManager()->GetGraphicsDevice()->GetDeviceObject());
		//Imaging::BitmapPainter painter(m_glyphCacheTexture->GetDeviceObject()->Lock());
		//painter.Clear(Color::Blue);
		//m_glyphCacheTexture->GetDeviceObject()->Unlock();

		// �󂫃L���b�V���C���f�b�N�X�쐬
		for (int i = 0; i < maxCharacters; i++) {
			m_indexStack.Push(i);
		}

		m_glyphMaxBitmapSize = m_font->GetGlyphMaxSize();
		m_tmpBitmap.Attach(LN_NEW Imaging::Bitmap(m_font->GetGlyphMaxSize(), Utils::TranslatePixelFormat(m_glyphCacheTexture->GetFormat())));


		m_prevGlyphLocationData = NULL;
		m_fontFaceModified = false;
	}
}

} // namespace Graphics
} // namespace Lumino
