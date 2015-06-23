/**
	@file	TextRenderer.h
*/
#pragma once
#include "Common.h"
#include "Color.h"
#include "../Imaging/Font.h"

namespace Lumino
{
namespace Graphics
{
class TextRendererImplemented;

/**
	@file	������̕`����s���N���X�ł��B
*/
class TextRenderer
	: public RefObject
{
public:

	/**
		@brief		TextRenderer �̃C���X�^���X���쐬���܂��B
	*/
	static TextRenderer* Create(GraphicsManager* manager);

public:

	void SetForeColor(Graphics::Color color) { m_foreColor = color; }
	void SetStrokeColor(Graphics::Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetDrawingArea(const Rect& area) { m_drawingArea = area; }

	/**
		@brief		
	*/
	void DrawText(const char* text, int length);
	void DrawText(const wchar_t* text, int length);

	/**
		@brief		�v������Ă���X�v���C�g��S�ĕ`�悵�܂��B
	*/
	void Flash();

private:
	TextRenderer(GraphicsManager* manager);
	virtual ~TextRenderer();
	GraphicsManager*			m_manager;
	Imaging::Font*		m_font;
	Color				m_foreColor;
	Color				m_strokeColor;
	int					m_strokeSize;
	TextAlignment		m_textAlignment;
	TextTrimming		m_textTrimming;
	FlowDirection		m_flowDirection;
	Rect				m_drawingArea;
};

} // namespace Graphics
} // namespace Lumino
