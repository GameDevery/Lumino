
#pragma once
#include "Common.h"
#include "Painter.h"	// for Brush

LN_NAMESPACE_BEGIN
class DrawingContextImpl;


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

namespace detail
{
enum class DrawingClass : uint32_t
{
	Unknown,
	LineList,
	TriangleList,
	GryphRun,
};


struct DrawingState
{
	Matrix			transform;
	RefPtr<Brush>	brush;
	RefPtr<Pen>		pen;
	RefPtr<Font>	font;
	float			opacity;
	ToneF			tone;
};

}


/**
	@brief	�}�`��摜�A�e�L�X�g��`�悷�邽�߂̋@�\��񋟂��܂��B
*/
class DrawingContext
	: public RefObject
{
public:

	void SetViewProjection(const Matrix& view, const Matrix& proj);

	void SetTransform(const Matrix& matrix);
	void SetBrush(Brush* brush);
	void SetPen(Pen* pen);
	void SetOpacity(float opacity);	// 0~1
	void SetTone(const ToneF& tone);
	void SetFont(Font* font);

	/**
		@brief		������`�悵�܂��B
	*/
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& color);

	/**
		@brief		��`��`�悵�܂��B
	*/
	void DrawRectangle(const RectF& rect, const ColorF& color);

	void Flush();

LN_INTERNAL_ACCESS:
	DrawingContext();
	virtual ~DrawingContext();
	void Initialize(GraphicsManager* manager);

private:
	void AddCommand(const void* command, size_t size);
	void FlushInternal();
	void SetDrawingClassInternal(detail::DrawingClass dc);
	void CheckFlush();

	GraphicsManager*		m_manager;
	DrawingContextImpl*		m_internal;
	ByteBuffer				m_commandsBuffer;
	size_t					m_commandsUsingByte;
	detail::DrawingClass	m_currentDrawingClass;
	detail::DrawingState	m_currentState;
	bool					m_stateModified;
};

LN_NAMESPACE_END
