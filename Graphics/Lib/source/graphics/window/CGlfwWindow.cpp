#include "graphics/graphics/window/CGlfwWindow.h"

#include <foundation/debug/Log.h>
#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "graphics/graphics/renderer/core/RendererCoreConfig.h"

std::unordered_map<GLFWwindow *, CGlfwWindow *> CGlfwWindow::s_windows; /**< Window mapping. */

CGlfwWindow::CGlfwWindow() : m_window(nullptr), m_width(0), m_height(0), m_mouseCaptured(false)
{
    return;
}

CGlfwWindow::~CGlfwWindow()
{
    // Remove mapping
    s_windows.erase(m_window);
    if (s_windows.empty())
    {
        glfwTerminate();
    }
}

bool CGlfwWindow::init(unsigned int width, unsigned int height, const std::string &name)
{
    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW.");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, FLEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, FLEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window from parameters
    LOG_INFO("Creating GLFW window for OpenGL version %u.%u.", FLEXT_MAJOR_VERSION,
             FLEXT_MINOR_VERSION);
    m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (m_window == nullptr)
    {
        LOG_ERROR("Failed to create GLFW window.");
        glfwTerminate();
        return false;
    }

    int framebufferWidth;
    int framebufferHeight;

    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);

    m_width = framebufferWidth;
    m_height = framebufferHeight;

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSwapInterval(0);

// Load OpenGL extensions
#ifndef __APPLE__
    if (flextInit() != GL_TRUE)
    {
        glfwTerminate();
        LOG_ERROR("Failed to initialize flextGL.");
        return false;
    }
#endif

    // Set window resize callback
    glfwSetFramebufferSizeCallback(m_window, &CGlfwWindow::resizeCallback);

    // Add mapping
    s_windows[m_window] = this;

    return true;
}

void CGlfwWindow::setWidth(unsigned int width) { m_width = width; }

void CGlfwWindow::setHeight(unsigned int height) { m_height = height; }

unsigned int CGlfwWindow::getWidth() const { return m_width; }

unsigned int CGlfwWindow::getHeight() const { return m_height; }

void CGlfwWindow::setActive() const { glfwMakeContextCurrent(m_window); }

bool CGlfwWindow::isOpen() const { return !glfwWindowShouldClose(m_window); }

void CGlfwWindow::swapBuffer() { glfwSwapBuffers(m_window); }

void CGlfwWindow::toggleMouseCapture()
{
    // Set capture mode
    if (m_mouseCaptured)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    // Toggle
    m_mouseCaptured = !m_mouseCaptured;
}

GLFWwindow *CGlfwWindow::getGlfwHandle() const { return m_window; }

void CGlfwWindow::addListener(IGlfwWindowListener *l) { m_listeners.insert(l); }

void CGlfwWindow::removeListener(IGlfwWindowListener *l) { m_listeners.erase(l); }

void CGlfwWindow::resizeCallback(GLFWwindow *window, int width, int height)
{
    assert(width > 0);
    assert(height > 0);
    if (s_windows.count(window) != 0)
    {
        CGlfwWindow *win = s_windows.at(window);
        win->handleResize(width, height);
    }
}

void CGlfwWindow::handleResize(int width, int height)
{
    setWidth(width);
    setHeight(height);

    for (auto &l : m_listeners)
    {
        l->handleResizeEvent(width, height);
    }
}
