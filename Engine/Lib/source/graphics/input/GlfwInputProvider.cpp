#include "kern/graphics/input/GlfwInputProvider.h"

#include <cassert>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fmtlog/fmtlog.h>

#include "kern/graphics/input/IInputListener.h"

std::unordered_map<GLFWwindow *, GlfwInputProvider *> GlfwInputProvider::s_instances;

void GlfwInputProvider::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto it = s_instances.find(window);
    if (it != s_instances.end())
    {
        EKeyEventType eventType;

        switch (action)
        {
        case GLFW_PRESS:
            eventType = EKeyEventType::KEY_PRESSED;
            break;
        case GLFW_REPEAT:
            eventType = EKeyEventType::KEY_REPEAT;
            break;
        case GLFW_RELEASE:
            eventType = EKeyEventType::KEY_RELEASED;
            break;
        default:
            logw("Ignored unknown action '{}' for glfwKeyCallback.", action);
            return;
        }

        for (IInputListener *listener : it->second->m_listeners)
        {
            listener->handleKeyEvent(eventType, key);
        }
    }
}

void GlfwInputProvider::glfwCursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    auto it = s_instances.find(window);
    if (it != s_instances.end())
    {
        for (IInputListener *listener : it->second->m_listeners)
        {
            listener->handleMouseMovementEvent((int)xpos, (int)ypos);
        }
    }
}

// void glfwCursorEnterCallback(GLFWwindow* window, int entered)
//{
//    auto it = s_instances.find(window);
//    if (it != s_instances.end())
//    {
//        for (IInputListener* listener : it->second.m_listeners)
//        {
//            listener->handleMouseMovementEvent(xpos, ypos);
//        }
//    }
//}

void GlfwInputProvider::glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    auto it = s_instances.find(window);
    if (it != s_instances.end())
    {
        EMouseButtonEventType eventType;

        switch (action)
        {
        case GLFW_PRESS:
            eventType = EMouseButtonEventType::MOUSE_BUTTON_PRESSED;
            break;
        case GLFW_RELEASE:
            eventType = EMouseButtonEventType::MOUSE_BUTTON_RELEASED;
            break;
        default:
            logw("Ignored unknown action '%{}' for glfwMouseButtonCallback.", action);
            return;
        }

        for (IInputListener *listener : it->second->m_listeners)
        {
            listener->handleMouseButtonEvent(eventType, button);
        }
    }
}

// void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    auto it = s_instances.find(window);
//    if (it != s_instances.end())
//    {
//        for (IInputListener* listener : it->second.m_listeners)
//        {
//            listener->handleScrollEvent(xoffset, yoffset);
//        }
//    }
//}

// ===
// class GlfwInputProvider
// ===

GlfwInputProvider::GlfwInputProvider(GLFWwindow *window) : m_window(window)
{
    assert(m_window != nullptr);
    assert(s_instances.find(m_window) == s_instances.end());

    // Set callbacks
    glfwSetKeyCallback(m_window, GlfwInputProvider::glfwKeyCallback);
    glfwSetCursorPosCallback(m_window, GlfwInputProvider::glfwCursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, GlfwInputProvider::glfwMouseButtonCallback);
    // Create mapping for callbacks
    s_instances[m_window] = this;
}

GlfwInputProvider::~GlfwInputProvider()
{
    // Remove mapping
    s_instances.erase(m_window);
}

void GlfwInputProvider::addInputListener(IInputListener *listener) { m_listeners.push_back(listener); }

void GlfwInputProvider::removeInputListener(IInputListener *listener) { m_listeners.remove(listener); }

bool GlfwInputProvider::isKeyPressed(int keyCode)
{
    if (m_window != nullptr)
    {
        return glfwGetKey(m_window, keyCode) == GLFW_PRESS;
    }
    return false;
}