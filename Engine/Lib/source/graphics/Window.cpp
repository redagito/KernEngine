#include "kern/graphics/Window.h"

#include <cassert>
#include <iostream>

#include <fmtlog/fmtlog.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "kern/graphics/renderer/RendererCoreConfig.h"

static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                   const GLchar *message, const void *userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

std::unordered_map<GLFWwindow *, Window *> Window::s_windows; /**< Window mapping. */

Window::Window() : m_window(nullptr), m_width(0), m_height(0), m_mouseCaptured(false)
{
    return;
}

Window::~Window()
{
    // Remove mapping
    s_windows.erase(m_window);
}

bool Window::init(unsigned int width, unsigned int height, const std::string &name)
{
    if (glfwInit() != GLFW_TRUE)
    {
        loge("Failed to initialize GLFW.");
        return false;
    }

	// Setup error callback
	glfwSetErrorCallback([](int code, const char* error) {
		throw std::runtime_error{ error };
	});

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
    // Enable debugging
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    // Create window from parameters
    logi("Creating GLFW window for OpenGL version {}.{}.", 4,
             6);
    m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        loge("Failed to create GLFW window.");
        glfwTerminate();
        return false;
    }

    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);

    m_width = framebufferWidth;
    m_height = framebufferHeight;

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_FALSE);
    // Vsync ON
    glfwSwapInterval(1);

// Load OpenGL extensions
#ifndef __APPLE__
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        glfwTerminate();
        loge("Failed to initialize GL context using GLAD.");
        return false;
    }
#endif

    // Add mapping
    s_windows[m_window] = this;

    // Set window resize callback
    glfwSetFramebufferSizeCallback(m_window, &Window::resizeCallback);

#ifndef NDEBUG
    // Setup debug vallback
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    return true;
}

void Window::setWidth(unsigned int width) { m_width = width; }

void Window::setHeight(unsigned int height) { m_height = height; }

unsigned int Window::getWidth() const { return m_width; }

unsigned int Window::getHeight() const { return m_height; }

void Window::setActive() const { glfwMakeContextCurrent(m_window); }

bool Window::isOpen() const { return !glfwWindowShouldClose(m_window); }

void Window::swapBuffer() { glfwSwapBuffers(m_window); }

void Window::processEvents() { glfwPollEvents(); }

void Window::toggleMouseCapture()
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

GLFWwindow *Window::getGlfwHandle() const { return m_window; }

void Window::addListener(IWindowListener *l) { m_listeners.insert(l); }

void Window::removeListener(IWindowListener *l) { m_listeners.erase(l); }

void Window::resizeCallback(GLFWwindow *window, int width, int height)
{
    assert(width > 0);
    assert(height > 0);
    if (s_windows.count(window) != 0)
    {
        Window *win = s_windows.at(window);
        win->handleResize(width, height);
    }
}

void Window::handleResize(int width, int height)
{
    setWidth(width);
    setHeight(height);

    for (auto &l : m_listeners)
    {
        l->handleResizeEvent(width, height);
    }
}