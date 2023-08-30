#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Window
{
   public:
    Window(unsigned int width, unsigned int height, const char* title);
    ~Window();

    using KeyCallback = void (*)(Window&, int, int, int, int);
    using MousePositionCallback = void (*)(Window&, double, double);

    void swapBuffers();

    void setVSync(bool sync);

    void pollEvents();

    void close();

    void setKeyCallback(const KeyCallback& callback);

    bool isOpen() const;

    // Get key state
    int getKey(int code) const;

    float getAspectRation() const;
    glm::mat4 getProjection(float fieldOfView, float zNear, float zFar) const;

    // Movement delta since last frame
    const glm::vec2& getCursorMovement() const;
    const glm::vec2& getScrollOffset() const;

    // Capture mouse cursor in window area
    void setCursorCapture(bool capture);
    bool getCursorCapture() const;

    GLFWwindow* getGLFWWindow() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

   private:
    void onResize(unsigned int width, unsigned int height);
    void onCursorMove(double x, double y);
    void onScroll(double xOffset, double yOffset);
    void onKeyPress(int key , int code, int action, int mods);

    unsigned int m_width = 0;
    unsigned int m_height = 0;

    // Scrolling offset
    glm::vec2 m_scrollOffset = glm::vec2{0.f};

    // Mouse capture, default off, toggle with f1
    bool m_cursorCaptured = false;

    glm::vec2 m_cursorPosition = glm::vec2(0.f);
    // First mouse position / movement
    // To prevent huge move vector on first call
    bool m_firstCursorMove = true;
    // Mouse movement delta
    glm::vec2 m_cursorMovement = glm::vec2{0.f};

    KeyCallback m_keyCallback = nullptr;
    GLFWwindow* m_window = nullptr;

    static Window* s_window;
};