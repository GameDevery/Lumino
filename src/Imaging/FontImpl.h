
#pragma once

#include <Lumino/Imaging/Font.h>

namespace Lumino
{
namespace Imaging
{
class Bitmap;

/// �r�b�g�}�b�v�t�H���g
class BitmapFontImpl
	: public Font
{
public:
	BitmapFontImpl(Bitmap* bitmap);
	virtual ~BitmapFontImpl();

};

} // namespace Imaging
} // namespace Lumino
