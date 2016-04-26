
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class PlatformManager;
class WindowManagerBase;
class Win32WindowManager;
class PlatformWindow;
class Win32PlatformWindow;

/** �E�B���h�E�V�X�e���� API�E���C�u���� */
enum class WindowSystemAPI
{
	Default,
	Win32API,
	X11,
	//GLFW,
};

namespace detail
{
	class MouseCursorVisibility;

} // namespace detail

LN_NAMESPACE_END

#include "PlatformEventArgs.h"
