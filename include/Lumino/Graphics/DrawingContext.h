
#pragma once
#include "Common.h"
#include "Painter.h"	// for Brush

LN_NAMESPACE_BEGIN
class DrawingContextImpl;

namespace detail
{
enum class DrawingClass : uint32_t
{
	Unknown,
	LineList,
	TriangleList,
	GryphRun,
};
}

/**
	@brief	�}�`��摜�A�e�L�X�g��`�悷�邽�߂̋@�\��񋟂��܂��B
*/
class DrawingContext
	: public RefObject
{
public:
	
	/**
		@brief		������`�悵�܂��B
	*/
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& color);

	void Flush();

protected:
	DrawingContext();
	virtual ~DrawingContext();
	void Initialize(GraphicsManager* manager);

private:
	void AddCommand(const void* command, size_t size);
	void FlushInternal();
	void CheckFlush(detail::DrawingClass newDrawingClass);

	GraphicsManager*		m_manager;
	DrawingContextImpl*		m_internal;
	ByteBuffer				m_commandsBuffer;
	size_t					m_commandsUsingByte;
	detail::DrawingClass	m_currentDrawingClass;
};

LN_NAMESPACE_END
