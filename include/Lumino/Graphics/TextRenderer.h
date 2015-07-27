/**
	@file	TextRenderer.h
*/
#pragma once
#include <Lumino/Base/Cache.h>
#include "Common.h"
#include "Color.h"
#include "Texture.h"
#include "../Imaging/Font.h"
#include "../../src/Imaging/TextLayoutEngine.h"	// TODO
#include "SpriteRenderer.h"

namespace Lumino
{
namespace Graphics
{
class TextRendererImplemented;



/**
	@file	������̕`����s���N���X�ł��B

	TODO: ���̃N���X�� internal �ɂ������BPainter �ɔC����B
*/
class TextRenderer
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:

	/**
		@brief		TextRenderer �̃C���X�^���X���쐬���܂��B
	*/
	static TextRenderer* Create(GraphicsManager* manager);

public:
	void SetTransform(const Matrix& matrix);
	void SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize);


	void SetFont(Imaging::Font* font) { m_font = font; m_layoutEngine.SetFont(font); m_fontFaceModified = true; }
	void SetForeColor(Color color) { m_foreColor = color; }
	void SetStrokeColor(Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; m_fontFaceModified = true; }
	void SetTextAlignment(Imaging::TextAlignment align) { m_layoutEngine.SetTextAlignment(align); }
	void SetTextTrimming(Imaging::TextTrimming triming) { m_layoutEngine.SetTextTrimming(triming); }
	void SetFlowDirection(Imaging::FlowDirection dir) { m_layoutEngine.SetFlowDirection(dir); }
	//void SetDrawingArea(const Rect& area) { m_drawingArea = area; }	// ����Ȃ�����

	/**
		@brief		
	*/
	void Measure(const UTF32* text, int length, Imaging::TextLayoutResult* outResult);

	/**
		@brief		
	*/
	void DrawText(const char* text, int length = -1);
	void DrawText(const wchar_t* text, int length = -1);
	void DrawChar(UTF32 ch);

	/**
		@brief		�v������Ă���X�v���C�g��S�ĕ`�悵�܂��B
	*/
	void Flash();

private:
	struct CachedGlyphInfo
	{
		int		Index;
		Size	Size;
	};

	void DrawSprite(const CachedGlyphInfo& info, const Point& point);
	void CheckResetCache();

private:
	TextRenderer(GraphicsManager* manager);
	virtual ~TextRenderer();
	//GraphicsManager*			m_manager;

	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	RefPtr<SpriteRenderer>	m_spriteRenderer;

	RefPtr<Imaging::Font>	m_font;
	Color				m_foreColor;
	Color				m_strokeColor;
	int					m_strokeSize;
	Imaging::TextLayoutEngine	m_layoutEngine;
	//TextAlignment		m_textAlignment;
	//TextTrimming		m_textTrimming;
	//FlowDirection		m_flowDirection;
	//Rect				m_drawingArea;

	Imaging::FontGlyphLocation*	m_prevGlyphLocationData;

	// �L���b�V���n���
	RefPtr<Texture>		m_glyphCacheTexture;
	CachedGlyphInfoMap	m_cachedGlyphInfoMap;
	int					m_glyphWidthCount;
	Size				m_glyphMaxBitmapSize;	///< ���݂̃t�H���g����1�������̃r�b�g�}�b�v�̍ő�T�C�Y
	Stack<int>			m_indexStack;			///< �󂫃L���b�V���C���f�b�N�X
	RefPtr<Imaging::Bitmap>	m_tmpBitmap;
	bool				m_fontFaceModified;
};


class FontGlyphTextureCache
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	FontGlyphTextureCache(GraphicsManager* manager, Imaging::Font* font);	// TODO: �X�g���[�N����pen�ŕ\���Ȃ瑾�����̈����������邱�ƂɂȂ�B

	void LookupGlyph(UTF32 ch, Texture** texture, Rect* srcRect);

	uint64_t CalcFontSettingHash() const;

	void Measure(const UTF32* text, int length, Imaging::TextLayoutResult* outResult);

private:
	struct CachedGlyphInfo
	{
		int		Index;
		Size	Size;
	};

	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	GraphicsManager*		m_manager;
	RefPtr<Imaging::Font>	m_font;
	RefPtr<Texture>			m_glyphCacheTexture;
	CachedGlyphInfoMap		m_cachedGlyphInfoMap;
	int						m_glyphWidthCount;
	Size					m_glyphMaxBitmapSize;	///< ���݂̃t�H���g����1�������̃r�b�g�}�b�v�̍ő�T�C�Y
	Stack<int>				m_indexStack;			///< �󂫃L���b�V���C���f�b�N�X
	RefPtr<Imaging::Bitmap>	m_tmpBitmap;

	Imaging::TextLayoutEngine	m_layoutEngine;
};

} // namespace Graphics
} // namespace Lumino
