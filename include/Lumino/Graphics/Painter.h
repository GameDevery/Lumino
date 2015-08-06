
#pragma once
#include "../Graphics/Font.h"
#include "Texture.h"
#include "GlyphRun.h"

namespace Lumino
{
namespace Graphics
{



enum BrushWrapMode
{
	BrushWrapMode_Stretch = 0,	///< �C���[�W�S�̂��������΂�
	BrushWrapMode_Tile,			///< �C���[�W����ׂĕ\������

};

class Brush
	: public CoreObject
{
public:
	Brush();
	virtual ~Brush();

public:
	virtual BrushType GetType() const = 0;	// TODO: ��virtual �ɂ�����
};


/**
	@brief		
*/
class ColorBrush
	: public Brush
{
public:
	static ColorBrush*	White;
	static ColorBrush*	Black;
	static ColorBrush*	Gray;
	static ColorBrush*	Red;
	static ColorBrush*	Green;
	static ColorBrush*	Blue;

public:
	ColorBrush(const Color& color);
	ColorBrush(const ColorF& color);
	virtual ~ColorBrush();

public:
	const ColorF& GetColor() const { return m_color; }

	virtual BrushType GetType() const { return BrushType_SolidColor; }

private:
	ColorF	m_color;
};

/// Bitmap �̓\�t�g�I�ȕ`�揈�����s�����߁ABitmapBrush �Ƃ������O�͂���Ȃ�
class TextureBrush
	: public Brush
{
public:
	TextureBrush();
	virtual ~TextureBrush();

public:
	void Create(const TCHAR* filePath, GraphicsManager* manager);
	void SetTexture(Texture* texture) { m_texture = texture; }
	Texture* GetTexture() { return m_texture; }

	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	const Rect& GetSourceRect() const { return m_srcRect; }

	void SetWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }
	BrushWrapMode GetWrapMode() const { return m_wrapMode; }


	virtual BrushType GetType() const { return BrushType_Texture; }

private:
	RefPtr<Texture>		m_texture;
	Rect				m_srcRect;	///< �����l�� (0, 0, INT_MAX, INT_MAX) �ŁA�S�̂�]�����邱�Ƃ�\��
	BrushWrapMode		m_wrapMode;
};

//struct StringLayout
//{
//	TextAlignment	Alignment;	///< ������̐��������̔z�u
//
//	static StringLayout	CenterAlignment;
//
//	StringLayout();
//
//	StringLayout(TextAlignment alignment);
//};

LN_ENUM_FLAGS(StringFormatFlags)
{
	LeftAlignment	= 0x0001,
	CenterAlignment	= 0x0002,
	RightAlignment	= 0x0004,

	Default = LeftAlignment,
};
LN_ENUM_FLAGS_DECLARE(StringFormatFlags);

/**
	@brief		
*/
class Painter
	: public RefObject
{
public:
	Painter(GraphicsManager* manager);
	virtual ~Painter();

public:
	void Begin();
	void End();

	void SetProjection(const Size& viewSize, float nearZ, float farZ);
	void SetProjection(const SizeF& viewSize, float nearZ, float farZ);

	void SetBrush(Brush* brush);
	void SetSolidColor(const ColorF& color);
	void SetTexture(Texture* texture, const Rect& srcRect);	///< ���[�e�B���e�B�BTextureBrush ���Z�b�g����
	void SetOpacity(float opacity);	// 0~1
	void SetFont(Font* font) { m_currentFont = font; }

	void DrawRectangle(const RectF& rect);
	void DrawRectangle(float x, float y, float width, float height) { DrawRectangle(RectF(x, y, width, height)); }



	//void DrawRectangle(const RectF& rect);	// TODO: �ς���
	void DrawFrameRectangle(const RectF& rect, float frameWidth);

	/// �Z�b�g����Ă��� Brush �͉e�����܂���
	//void DrawTexture(const RectF& dstRect, Texture* texture, const Rect& srcRect);

	void DrawGlyphRun(const Point& position, GlyphRun* glyphRun);
	void DrawGlyphRun(const PointF& position, GlyphRun* glyphRun);	// SetFont ����

	void DrawString(const String& str, const PointF& position);
	void DrawString(const TCHAR* str, int length, const PointF& position);
	void DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags);

private:
	void DrawGlyphs(const PointF& position, const TextLayoutResult* result, Internal::FontGlyphTextureCache* cache);

private:
	GraphicsManager*		m_manager;
	PainterEngine*			m_internal;
	RefPtr<Brush>			m_currentBrush;
	ByteBuffer				m_tempBuffer;
	RefPtr<Font>			m_currentFont;
};

} // namespace Graphics
} // namespace Lumino
