﻿
#ifdef LN_GLFW

#include "Internal.hpp"
#include "GLFWContext.hpp"
#include "../Platform/GLFWPlatformWindowManager.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace ln {
namespace detail {

//=============================================================================
// GLFWSwapChain

GLFWSwapChain::GLFWSwapChain(OpenGLDevice* device, GLFWPlatformWindow* window)
	: GLSwapChain(device) 
	, m_window(window)
{
}

void GLFWSwapChain::getBackendBufferSize(SizeI* outSize)
{
	m_window->getFramebufferSize(&outSize->width, &outSize->height);
}

//=============================================================================
// GLFWContext

void GLFWContext::init(OpenGLDevice* device, PlatformWindow* window)
{
	m_device = device;
	auto* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(window);
	LN_CHECK(glfwWindow);
	m_mainWindow = glfwWindow;

	glfwMakeContextCurrent(m_mainWindow->glfwWindow());
}

Ref<GLSwapChain> GLFWContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(window);
	LN_CHECK(glfwWindow);

	auto ptr = makeRef<GLFWSwapChain>(m_device, glfwWindow);
	ptr->genBackbuffer(backbufferSize.width, backbufferSize.height);
	return ptr;
}

void GLFWContext::makeCurrent(GLSwapChain* swapChain)
{
	glfwMakeContextCurrent(static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
}

void GLFWContext::swap(GLSwapChain* swapChain)
{
	glfwSwapBuffers(static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
}

} // namespace detail
} // namespace ln

#endif
