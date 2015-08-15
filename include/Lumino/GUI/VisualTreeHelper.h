
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief	�r�W���A���c���[�̗v�f�ɃA�N�Z�X���邽�߂̃��[�e�B���e�B�ł��B
*/
class VisualTreeHelper
{
public:
	static UIElement* FindChildByName(UIElement* parent, const TCHAR* name);
};

} // namespace GUI
} // namespace Lumino
