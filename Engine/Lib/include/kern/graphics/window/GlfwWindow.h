#pragma once

#include <list>
#include <set>
#include <unordered_map>
#include <string>

#include "kern/graphics/IWindow.h"
#include "kern/graphics/input/IInputProvider.h"

struct GLFWwindow;

class IGlfwWindowListener
{
   public:
    virtual void handleResizeEvent(int width, int height) = 0;
};

/**
 * \brief GLFW based implementation of the window interface.
 */
class GlfwWindow : public IWindow
{
   public:
    GlfwWindow();
    ~GlfwWindow();

    bool init(unsigned int width, unsigned int height, const std::string &name);

    void setWidth(unsigned int width);
    void setHeight(unsigned int height);

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setActive() const;

    bool isOpen() const;

    /**
     * \brief Swaps back and front buffers.
     */
    void swapBuffer();

    // Event processing
    void processEvents();

    /**
     * \brief Toggles mouse capture state.
     */
    void toggleMouseCapture();

    GLFWwindow *getGlfwHandle() const;

    void addListener(IGlfwWindowListener *l);
    void removeListener(IGlfwWindowListener *l);

   private:
    static void resizeCallback(GLFWwindow *window, int width, int height);
    void handleResize(int width, int height);

    static std::unordered_map<GLFWwindow *, GlfwWindow *>
        s_windows; /**< GLFW window to window object mapping. */

    GLFWwindow *m_window = nullptr; /**< Window resource. */
    unsigned int m_width = 0;       /**< Current window width. */
    unsigned int m_height = 0;      /**< Current window height. */

    bool m_mouseCaptured = false; /**< Current mouse capture state. */
    std::set<IGlfwWindowListener *> m_listeners;
};
