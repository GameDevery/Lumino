﻿
#pragma once
#include <Lumino/Imaging/Font.h>
#include <Lumino/Imaging/FontManager.h>

namespace Lumino
{
namespace Imaging
{
class FontManager;


struct FreeTypeGlyphLocation
	: public FontGlyphLocation
{
	int				NextBaseX;			///< 内部データ
	int				NextBaseY;			///< 内部データ
	uint32_t		PreviousGlyphIndex;	///< ひとつ前の文字の GlyphIndex
};

struct FreeTypeGlyphBitmap
	: public FontGlyphBitmap
{
	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている

	FreeTypeGlyphBitmap()
	{
		CopyGlyph = NULL;
		CopyOutlineGlyph = NULL;
	}
	~FreeTypeGlyphBitmap()
	{
		ReleaseGlyph();
	}

	void ReleaseGlyph();
};

// [Obsolete]
struct FreeTypeGlyphData
	: public FontGlyphData
{
	//int             BitmapRealDataSize;    ///< 内部データ
	int				NextBaseX;      ///< 内部データ
	int				NextBaseY;      ///< 内部データ
	uint32_t		Previous;       ///< 内部データ
	FT_Glyph		CopyGlyph;			///< GlyphBitmap のバッファ本体はこれが持っている
	FT_Glyph		CopyOutlineGlyph;	///< OutlineBitmap のバッファ本体はこれが持っている

	void ReleaseGlyph();
};


/**
	@brief		フォントのクラスです。
*/
class FreeTypeFont
	: public Font
{
public:
	FreeTypeFont(FontManager* manager);
	virtual ~FreeTypeFont();

public:
	virtual void SetName(const String& fontName) { m_fontName = fontName; m_modified = true; }
	virtual const String& GetName() const { return m_fontName; }
	virtual void SetSize(int size) { m_fontSize = size; m_modified = true; }
	virtual int GetSize() const  { return m_fontSize; }
	virtual void SetEdgeSize(int size) { m_edgeSize = size; m_modified = true; }
	virtual int GetEdgeSize() const  { return m_edgeSize; }
	virtual void SetBold(bool enabled) { m_isBold = enabled; m_modified = true; }
	virtual bool IsBold() const  { return m_isBold; }
	virtual void SetItalic(bool enabled) { m_isItalic = enabled; m_modified = true; }
	virtual bool IsItalic() const { return m_isItalic; }
	virtual void SetAntiAlias(bool enabled) { m_isAntiAlias = enabled; m_modified = true; }
	virtual bool IsAntiAlias() const { return m_isAntiAlias; }

	virtual Font* Copy() const;
	virtual int GetLineHeight() { UpdateFont(); return m_lineHeight; }
	virtual Size GetTextSize(const char* text, int length);
	virtual Size GetTextSize(const wchar_t* text, int length);
	virtual Size GetTextSize(const UTF32* text, int length);
	virtual FontGlyphLocation* AdvanceKerning(UTF32 utf32code, FontGlyphLocation* prevData);
	virtual FontGlyphBitmap* LookupGlyphBitmap(UTF32 utf32code);
	virtual FontGlyphData* LookupGlyphData(UTF32 utf32code, FontGlyphData* prevData);

	virtual FontManager* GetManager() const { return m_manager; }

private:
	void Dispose();
	void UpdateFont();
	void RefreshBitmap(Bitmap* bitmap, FT_Bitmap* ftBitmap);

private:
	FontManager*		m_manager;
	String				m_fontName;
	int					m_fontSize;
	int					m_edgeSize;
	bool				m_isBold;
	bool				m_isItalic;
	bool				m_isAntiAlias;
	bool				m_modified;

	FTC_FaceID			m_ftFaceID;			///< キャッシュから FT_Face を検索するためのキー値
	FT_Face				m_ftFace;			///< フォント本体
	FT_Int				m_ftCacheMapIndex;	///< m_ftFace 内で現在アクティブな FT_CharMap の番号
	FT_Stroker			m_ftStroker;		///< エッジの生成情報
	FTC_ImageTypeRec	m_ftImageType;		///< キャッシュからグリフビットマップを取りだすための情報
	int					m_lineHeight;

	ByteBuffer			m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)

	FreeTypeGlyphLocation	m_fontGlyphLocation;
	FreeTypeGlyphBitmap		m_fontGlyphBitmap;

	FreeTypeGlyphData	m_glyphData;		///< LookupGlyphData() の戻り値として公開されるデータ
	RefPtr<Bitmap>		m_glyphBitmap;		///< LookupGlyphData() で生成する一時グリフデータ
	RefPtr<Bitmap>		m_outlineBitmap;	///< LookupGlyphData() で生成する一時グリフデータ
};

} // namespace Imaging
} // namespace Lumino
