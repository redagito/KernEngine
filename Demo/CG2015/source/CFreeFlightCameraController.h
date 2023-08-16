#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/graphics/window/CGlfwWindow.h"

#include "graphics/input/IInputListener.h"

class IControllableCamera;
class IInputProvider;

class CFreeFlightCameraController : public IInputListener, public IGlfwWindowListener
{
   public:
    CFreeFlightCameraController();
    ~CFreeFlightCameraController();

    void setCamera(std::shared_ptr<IControllableCamera> camera);
    void setInputProvider(IInputProvider *provider);

    bool loadSequence(const std::string& file);

    void animate(float dt);

    void handleKeyEvent(EKeyEventType type, int keyCode) override;
    void handleMouseMovementEvent(int x, int y) override;
    void handleMouseButtonEvent(EMouseButtonEventType type, int buttonCode) override;

    void handleResizeEvent(int width, int height) override;

   private:
    // TODO "animate" is a stupid name
    void animateFeatures(float dt);
    void animateSequence(float dt);
    void animateManual(float dt);

    struct SequencePoint
    {
        glm::vec3 position;
        glm::vec3 orientation;
        float timestamp;
        bool fxaaActive;
        bool fogActive;
    };

    bool m_isRunningSequence = false;
    float m_sequenceTime = 0;

    std::vector<SequencePoint> m_sequencePoints;

    float m_speed = 1.0f;
    std::shared_ptr<IControllableCamera> m_camera = nullptr;
    IInputProvider *m_inputProvider = nullptr;
    int m_lastX = 0;
    int m_lastY = 0;
};