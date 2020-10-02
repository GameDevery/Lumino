﻿
#include "Internal.hpp"
#include "EmptyPlatformWindowManager.hpp"
#include "GLFWPlatformWindowManager.hpp"
#include "Win32PlatformWindowManager.hpp"
#include "PlatformManager.hpp"

namespace ln {
namespace detail {

PlatformManager::PlatformManager()
	: m_windowManager()
    , m_glfwWithOpenGLAPI(true)
{
}

Result PlatformManager::init(const Settings& settings)
{
#ifdef LN_GLFW
    if (settings.useGLFWWindowSystem) {
        if (!m_windowManager) {
            auto windowManager = ln::makeRef<GLFWPlatformWindowManager>(this);
            if (!windowManager->init()) {
                return false;
            }
            m_windowManager = windowManager;
        }
    }
#endif
#ifdef LN_OS_WIN32
    if (!m_windowManager) {
        auto windowManager = ln::makeRef<Win32PlatformWindowManager>(this);
        if (!windowManager->init()) {
            return false;
        }
        m_windowManager = windowManager;
    }
#endif

    m_glfwWithOpenGLAPI = settings.glfwWithOpenGLAPI;

	if (!m_windowManager)
	{
		auto windowManager = ln::makeRef<EmptyPlatformWindowManager>(this);
        if (!windowManager->init()) {
            return false;
        }
		m_windowManager = windowManager;
	}

	m_mainWindow = m_windowManager->createMainWindow(settings.mainWindowSettings);

    return true;
}

void PlatformManager::dispose()
{
	if (m_mainWindow) {
		m_windowManager->destroyWindow(m_mainWindow);
		m_mainWindow = nullptr;
	}
	if (m_windowManager) {
		m_windowManager->dispose();
		m_windowManager = nullptr;
	}
}

OpenGLContext* PlatformManager::openGLContext() const
{
    return m_windowManager->getOpenGLContext();
}

} // namespace detail
} // namespace ln

