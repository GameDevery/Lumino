#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	�����_�����O�Ɋւ���@�\��񋟂��܂��B
*/
class Graphics
{
public:

	static void Set2DRenderingMode(float minZ = 0.0f, float maxZ = 1.0f);

	static void SetBrush(Brush* brush);
	static void SetOpacity(float opacity);	// 0~1

	static void DrawPoint(const Vector3& point, const ColorF& color);

	static void DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color);

	static void DrawRectangle(const RectF& rect, const ColorF& color);

	static void DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color);

	static void Flush();

	/**
		@brief		�����_�����O���J�n���܂��B
		@details	Application::UpdateFrame() ���g�p����ꍇ�A���̊֐����Ăяo���K�v�͂���܂���B
	*/
	//void BeginRendering();
	
	/**
		@brief		�����_�����O���I�����܂��B
	*/
	//void EndRendering();

	static void ChangeDirectX9Device(void* id3d9device);
};

LN_NAMESPACE_END
