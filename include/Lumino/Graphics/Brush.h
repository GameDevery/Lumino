
#pragma once
#include "Common.h"
#include "../Graphics/Text/Font.h"
#include "Texture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

enum BrushWrapMode
{
	BrushWrapMode_Stretch = 0,	///< �C���[�W�S�̂��������΂�
	BrushWrapMode_Tile,			///< �C���[�W����ׂĕ\������

};

class Brush
	: public Object
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
class ColorBrush	// TODO: SolidColorBrush
	: public Brush
{
public:
	static ColorBrush*	White;
	static ColorBrush*	Black;
	static ColorBrush*	Gray;
	static ColorBrush*	Red;
	static ColorBrush*	Green;
	static ColorBrush*	Blue;
	static ColorBrush*	DimGray;

public:
	ColorBrush(const Color& color);
	ColorBrush(const ColorF& color);
	ColorBrush(float r, float g, float b, float a);
	virtual ~ColorBrush();

public:
	void SetColor(const ColorF& color) { m_color = color; }
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

	static RefPtr<TextureBrush> Create(const StringRef& filePath);

public:
	TextureBrush();
	virtual ~TextureBrush();

public:
	void Create(const TCHAR* filePath, GraphicsManager* manager);
	void Create(Texture* texture);
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


class FrameTextureBrush
	: public Brush
{
public:
	static FrameTextureBrushPtr Create(const TCHAR* filePath);
	static FrameTextureBrushPtr Create(Texture* texture);

public:
	void SetTexture(Texture* texture) { m_texture = texture; }
	Texture* GetTexture() { return m_texture; }

	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	const Rect& GetSourceRect() const { return m_srcRect; }

	void SetInnerAreaSourceRect(const Rect& rect) { m_innerSrcRect = rect; }
	const Rect& GetInnerAreaSourceRect() const { return m_innerSrcRect; }


	void SetWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }
	BrushWrapMode GetWrapMode() const { return m_wrapMode; }


	void SetThickness(int thickness) { m_frameThickness = thickness; }
	int GetThickness() const { return m_frameThickness; }

	virtual BrushType GetType() const { return BrushType_FrameTexture; }

public:
	FrameTextureBrush();
	virtual ~FrameTextureBrush();

private:
	RefPtr<Texture>		m_texture;
	Rect				m_srcRect;	///< �����l�� (0, 0, INT_MAX, INT_MAX) �ŁA�S�̂�]�����邱�Ƃ�\��
	Rect				m_innerSrcRect;
	int					m_frameThickness;
	BrushWrapMode		m_wrapMode;
};

/**
	@brief	�}�`�̘g���̕`����@��\���܂��B
*/
class Pen
	: public RefObject
{
public:
	Pen()
		: m_thickness(0)
	{}

	void SetBrush(Brush* brush) { m_brush = brush; }
	Brush* GetBrush() const { return m_brush; }

	void SetThickness(float thickness) { m_thickness = thickness; }
	float GetThickness() const { return m_thickness; }

private:
	RefPtr<Brush>	m_brush;
	float			m_thickness;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

