
#pragma once

#include <Lumino/Graphics/Color.h>
#include "Bitmap.h"

namespace Lumino
{
namespace Imaging
{

/**
	@brief	 �r�b�g�}�b�v�ւ̕`����s���N���X�ł��B
*/
class BitmapPainter
	: public NonCopyable
{
public:

	/**
		@brief		�w�肵�� Bitmap �֕`����s�� BitmapPainter �����������܂��B
	*/
	BitmapPainter(Bitmap* targetBitmap);

	~BitmapPainter();

public:

	/**
		@brief		�w�肵���F�Ńr�b�g�}�b�v�S�̂�h��Ԃ��܂��B
	*/
	void Clear(const Graphics::Color& color);

	/**
		@brief		��`��h��Ԃ��܂��B
	*/
	void FillRectangle(const Rect& rect, const Graphics::Color& color);

private:
	uint32_t GetColorByteSec(const Graphics::Color& color, PixelFormat format);

public:
	Bitmap*	m_bitmap;

};

} // namespace Imaging
} // namespace Lumino
