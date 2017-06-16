﻿
/*
	書式設定されたテキストの描画
	https://msdn.microsoft.com/ja-jp/library/ms752098%28v=vs.110%29.aspx
*/

#include "../Internal.h"
#include <algorithm>
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Rendering/Rendering.h>
#include "../Device/GraphicsDriverInterface.h"
#include "../RHIRenderingContext.h"
#include "../RenderingCommand.h"
#include "SpriteTextRenderFeature.h"
#include "FontManager.h"
#include "TextLayoutEngine.h"
#include "FontGlyphTextureCache.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// TextRendererCore
//==============================================================================

static const byte_t g_TextRenderer_fx_Data[] =
{
#include "../Resource/TextRenderer.fx.h"
};
static const size_t g_TextRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_TextRenderer_fx_Data);

//------------------------------------------------------------------------------
TextRendererCore::TextRendererCore()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

//------------------------------------------------------------------------------
TextRendererCore::~TextRendererCore()
{
	releaseDeviceResources();
}

//------------------------------------------------------------------------------
void TextRendererCore::initialize(GraphicsManager* manager)
{
	GraphicsResourceObject::initialize();
	createDeviceResources();
}

//------------------------------------------------------------------------------
void TextRendererCore::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
		releaseDeviceResources();
	else
		createDeviceResources();
}

//------------------------------------------------------------------------------
void TextRendererCore::createDeviceResources()
{
	const int DefaultFaceCount = 512;

	auto* device = m_manager->getGraphicsDevice();
	m_vertexDeclaration.attach(device->createVertexDeclaration(Vertex::Elements(), Vertex::ElementCount));
	m_vertexBuffer = device->createVertexBuffer(sizeof(Vertex) * DefaultFaceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->createIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);

	m_vertexCache.reserve(DefaultFaceCount * 4);
	m_indexCache.reserve(DefaultFaceCount * 6);

	ShaderCompileResult r;
	m_shader.shader = device->createShader(g_TextRenderer_fx_Data, g_TextRenderer_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);
	m_shader.technique = m_shader.shader->getTechnique(0);
	m_shader.pass = m_shader.technique->getPass(0);
	m_shader.varWorldMatrix = m_shader.shader->getVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->getVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTone = m_shader.shader->getVariableByName(_T("g_tone"));
	m_shader.varTexture = m_shader.shader->getVariableByName(_T("g_texture"));
	//m_shader.varGlyphMaskSampler = m_shader.shader->getVariableByName(_T("g_glyphMaskTexture"));
	m_shader.varPixelStep = m_shader.shader->getVariableByName(_T("g_pixelStep"));
}

//------------------------------------------------------------------------------
void TextRendererCore::releaseDeviceResources()
{
	LN_SAFE_RELEASE(m_shader.shader);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void TextRendererCore::setState(const Matrix& world, const Matrix& viewProj, const SizeI& viewPixelSize)
{
	m_shader.varWorldMatrix->setMatrix(world);
	m_shader.varViewProjMatrix->setMatrix(viewProj);
	if (m_shader.varPixelStep != nullptr)
		m_shader.varPixelStep->setVector(Vector4(0.5f / viewPixelSize.width, 0.5f / viewPixelSize.height, 0, 0));
}

//------------------------------------------------------------------------------
void TextRendererCore::render(const GlyphRunData* dataList, int dataCount, FontGlyphTextureCache* cache, Brush* fillBrush)
{
	//Color color = Color::White;
	//if (fillBrush != nullptr)
	//{
	Color color = fillBrush->getColor();
	//}

	Driver::ITexture* srcTexture = cache->getGlyphsFillTexture();
	Size texSizeInv(1.0f / srcTexture->getRealSize().width, 1.0f / srcTexture->getRealSize().height);
	for (int i = 0; i < dataCount; ++i)
	{
		// TODO: 以下、srcFillRect, srcOutlineRectを使った方が良い気がする
		const GlyphRunData& data = dataList[i];
		Rect uvSrcRect((float)data.srcRect.x, (float)data.srcRect.y, (float)data.srcRect.width, (float)data.srcRect.height);
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		Rect dstRect(data.Position, (float)data.srcRect.width, (float)data.srcRect.height);
		internalDrawRectangle(data.transform, dstRect, uvSrcRect, color);
	}

	flush(cache);
}

//------------------------------------------------------------------------------
void TextRendererCore::internalDrawRectangle(const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color)
{
	if (rect.isEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float lu = srcUVRect.getLeft();
	float tv = srcUVRect.getTop();
	float ru = srcUVRect.getRight();
	float bv = srcUVRect.getBottom();

	uint16_t i = m_vertexCache.getCount();
	m_indexCache.add(i + 0);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 3);

	Vertex v;
	v.color = color;
	v.position.set(rect.getLeft(), rect.getTop(), 0);	v.uv.set(lu, tv);	// 左上
	v.position.transformCoord(transform);
	m_vertexCache.add(v);
	v.position.set(rect.getLeft(), rect.getBottom(), 0); v.uv.set(lu, bv);	// 左下
	v.position.transformCoord(transform);
	m_vertexCache.add(v);
	v.position.set(rect.getRight(), rect.getTop(), 0);	v.uv.set(ru, tv);	// 右上
	v.position.transformCoord(transform);
	m_vertexCache.add(v);
	v.position.set(rect.getRight(), rect.getBottom(), 0); v.uv.set(ru, bv);	// 右下
	v.position.transformCoord(transform);
	m_vertexCache.add(v);
}

//------------------------------------------------------------------------------
void TextRendererCore::flush(FontGlyphTextureCache* cache)
{
	if (m_indexCache.getCount() == 0) { return; }

	auto* renderer = m_manager->getGraphicsDevice()->getRenderer();

	// ビットマップフォントからの描画なので、アルファブレンドONでなければ真っ白矩形になってしまう。
	// ・・・が、TextRendererCore のような低レベルでステートを強制変更してしまうのはいかがなものか・・・。
	RenderState oldState = renderer->getRenderState();
	RenderState newState = oldState;
	newState.alphaBlendEnabled = true;
	newState.sourceBlend = BlendFactor::SourceAlpha;
	newState.destinationBlend = BlendFactor::InverseSourceAlpha;
	renderer->setRenderState(newState);



	// 描画する
	m_vertexBuffer->setSubData(0, m_vertexCache.getBuffer(), m_vertexCache.getBufferUsedByteCount());
	m_indexBuffer->setSubData(0, m_indexCache.getBuffer(), m_indexCache.getBufferUsedByteCount());
	m_shader.varTone->setVector((Vector4&)m_tone);
	m_shader.varTexture->setTexture(cache->getGlyphsFillTexture());
	//m_shader.varGlyphMaskSampler->setTexture(m_glyphsMaskTexture);
	renderer->setShaderPass(m_shader.pass);
	renderer->setVertexDeclaration(m_vertexDeclaration);
	renderer->setVertexBuffer(0, m_vertexBuffer);
	renderer->setIndexBuffer(m_indexBuffer);
	renderer->drawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);

	// キャッシュクリア
	m_vertexCache.clear();
	m_indexCache.clear();

	// 変更したステートを元に戻す
	renderer->setRenderState(oldState);
}


//==============================================================================
// TextRenderer
//==============================================================================

//------------------------------------------------------------------------------
TextRenderer::TextRenderer()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_glyphLayoutDataList()
	, m_transform()
	, m_viewProj()
	, m_viewPixelSize()
	, m_font(nullptr)
	, m_fillBrush(nullptr)
	, m_stateModified(false)
	, m_flushRequested(false)
{
}

//------------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
}

//------------------------------------------------------------------------------
void TextRenderer::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_core = m_manager->getTextRendererCore();
	m_font = m_manager->getFontManager()->getDefaultRawFont();
}

//------------------------------------------------------------------------------
void TextRenderer::setTransform(const Matrix& matrix)
{
	m_transform = matrix;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void TextRenderer::setViewInfo(const Matrix& viewProj, const SizeI& viewPixelSize)
{
	if (m_viewProj != viewProj)
	{
		m_viewProj = viewProj;
		m_stateModified = true;
	}
	if (m_viewPixelSize != viewPixelSize)
	{
		m_viewPixelSize = viewPixelSize;
		m_stateModified = true;
	}
}

//------------------------------------------------------------------------------
void TextRenderer::drawGlyphRun(const Matrix& transform, const PointI& position, GlyphRun* glyphRun)
{
	drawGlyphRun(transform, PointF((float)position.x, (float)position.y), glyphRun);
}
void TextRenderer::drawGlyphRun(const Matrix& transform, const PointF& position, GlyphRun* glyphRun)
{
	if (glyphRun == nullptr) return;
	checkUpdateState();
	m_font = glyphRun->getFont();
	DrawGlyphsInternal(transform, position, glyphRun->requestLayoutItems(), glyphRun->lookupFontGlyphTextureCache());
}

//------------------------------------------------------------------------------
void TextRenderer::drawString(const Matrix& transform, const TCHAR* str, int length, const PointF& position)
{
	length = (length < 0) ? StringTraits::tcslen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->getFontManager()->getTCharToUTF32Converter()->convert(str, sizeof(TCHAR) * length);

	FontGlyphTextureCache* cache = m_font->GetGlyphTextureCache();

	// 
	TextLayoutResult result;
	cache->getTextLayoutEngine()->resetSettings();
	cache->getTextLayoutEngine()->layoutText((UTF32*)utf32Buf.getConstData(), utf32Buf.getSize() / sizeof(UTF32), LayoutTextOptions::All, &result);

	DrawGlyphsInternal(transform, position, result.Items, cache);
}

//------------------------------------------------------------------------------
void TextRenderer::drawString(const Matrix& transform, const TCHAR* str, int length, const Rect& rect, StringFormatFlags flags)
{
	length = (length < 0) ? StringTraits::tcslen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->getFontManager()->getTCharToUTF32Converter()->convert(str, sizeof(TCHAR) * length);

	FontGlyphTextureCache* cache = m_font->GetGlyphTextureCache();

	// 
	TextLayoutEngine* layout = cache->getTextLayoutEngine();
	cache->getTextLayoutEngine()->resetSettings();

	if (flags.TestFlag(StringFormatFlags::LeftAlignment)) {
		layout->setTextAlignment(TextAlignment::Left);
	}
	else if (flags.TestFlag(StringFormatFlags::RightAlignment)) {
		layout->setTextAlignment(TextAlignment::Right);
	}
	else if (flags.TestFlag(StringFormatFlags::CenterAlignment)) {
		layout->setTextAlignment(TextAlignment::Center);
	}
	else {
	}

	cache->getTextLayoutEngine()->setDrawingArea(RectI(0, 0, (int)rect.width, (int)rect.height));


	TextLayoutResult result;
	cache->getTextLayoutEngine()->layoutText((UTF32*)utf32Buf.getConstData(), utf32Buf.getSize() / sizeof(UTF32), LayoutTextOptions::All, &result);

	DrawGlyphsInternal(transform, rect.getTopLeft(), result.Items, cache);
}

//------------------------------------------------------------------------------
void TextRenderer::DrawGlyphsInternal(const Matrix& transform, const PointF& position, const List<TextLayoutResultItem>& layoutItems, FontGlyphTextureCache* cache)
{
	checkUpdateState();

	int dataCount = layoutItems.getCount();
	for (int i = 0; i < dataCount; ++i)
	{
		const TextLayoutResultItem& item = layoutItems[i];

		// 必要なグリフを探す。lookupGlyphInfo() の中で、テクスチャにグリフビットマップが blt される。
		CacheGlyphInfo info;
		bool flush;
		cache->lookupGlyphInfo(item.Char, &info, &flush);
		if (flush)
		{
			FlushInternal(cache);
		}

		// TODO: Outline

		TextRendererCore::GlyphRunData layoutData;
		layoutData.transform = transform;
		layoutData.Position.x = position.x + (float)item.Location.OuterTopLeftPosition.x;
		layoutData.Position.y = position.y + (float)item.Location.OuterTopLeftPosition.y;
		layoutData.srcRect = info.srcRect;
		layoutData.outlineOffset = info.outlineOffset;
		m_glyphLayoutDataList.add(layoutData);
	}

	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void TextRenderer::flush()
{
	if (m_flushRequested)
	{
		FontGlyphTextureCache* cache = m_font->GetGlyphTextureCache();
		FlushInternal(cache);
	}
}

//------------------------------------------------------------------------------
void TextRenderer::onSetState(const DrawElementBatch* state)
{
	if (state != nullptr)
	{
		Font* font = state->state.getFont();
		LN_ASSERT(font != nullptr);

		RawFont* rawFont = font->resolveRawFont();
		if (m_font != rawFont)
		{
			m_font = rawFont;

			// TODO: 必要ないかも？
			m_stateModified = true;
		}
		if (m_fillBrush != state->state.getBrush())
		{
			m_fillBrush = state->state.getBrush();

			// TODO: 必要ないかも？
			m_stateModified = true;
		}

		LN_ASSERT(m_fillBrush != nullptr);
	}
}

//------------------------------------------------------------------------------
void TextRenderer::FlushInternal(FontGlyphTextureCache* cache)
{
	int dataCount = m_glyphLayoutDataList.getCount();
	RenderBulkData dataListData(&m_glyphLayoutDataList[0], sizeof(TextRendererCore::GlyphRunData) * dataCount);

	// Texture::blit で転送されるものを Flush する
	cache->getGlyphsFillTexture();

	LN_ENQUEUE_RENDER_COMMAND_5(
		TextRenderer_Flush, m_manager,
		TextRendererCore*, m_core,
		RenderBulkData, dataListData,
		int, dataCount,
		RefPtr<FontGlyphTextureCache>, cache,
		RefPtr<Brush>, m_fillBrush,	// TODO: Brush をそのまま描画スレッドに持ち込むのは危険。変更される。
		{
			m_core->render(
				(TextRendererCore::GlyphRunData*)dataListData.getData(),
				dataCount,
				cache,
				m_fillBrush);
		});

	m_glyphLayoutDataList.clear();
	m_flushRequested = false;
}

//------------------------------------------------------------------------------
void TextRenderer::checkUpdateState()	// あらゆる draw の直前にやりたいこと
{
	if (m_stateModified)
	{
		LN_ENQUEUE_RENDER_COMMAND_4(
			TextRenderer_Flush, m_manager,
			TextRendererCore*, m_core,
			Matrix, m_transform,
			Matrix, m_viewProj,
			SizeI, m_viewPixelSize,
			{
				m_core->setState(m_transform, m_viewProj, m_viewPixelSize);
			});

		m_stateModified = false;
	}
}

} // namespace detail
LN_NAMESPACE_END