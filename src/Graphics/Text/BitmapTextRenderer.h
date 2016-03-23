
#pragma once
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class BitmapTextRenderer
	: public RefObject
{
public:
	BitmapTextRenderer();
	virtual ~BitmapTextRenderer();

	void Initialize(GraphicsManager* manager);

	GlyphRun* GetTempGlyphRun() { return m_tmpGlyphRun; }	// �e�N�X�`���� DrawText ���A���[�e�B���e�B�I�Ɏg�������Ƃ��� new �����炷

	void DrawGlyphRun(Bitmap* target, GlyphRun* glyphRun, const Color& fillColor, const Color& strokeColor, int strokeThickness);

private:
	GraphicsManager*	m_manager;
	GlyphRun*			m_tmpGlyphRun;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
