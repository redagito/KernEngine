#include "RenderApplication.h"

#include <audio/SoundSystem.h>

#include <chrono>

RenderApplication::RenderApplication()
{
    m_window = std::make_shared<Window>(800, 600, "Render Test");
    m_window->setVSync(true);
    m_window->setKeyCallback(
        [](Window& w, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                w.close();
            }
            if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
            {
                w.setCursorCapture(!w.getCursorCapture());
            }
        });

    m_soundSystem = std::make_shared<SoundSystem>("data/sounds");
    m_soundSystem->getManager()->registerSound("bg", "WATRWave_Sea waves (ID 0266)_BSB.wav");

    m_backgroundSoundEmitter = m_soundSystem->createEmitter();
    m_backgroundSoundEmitter->setLooping(true);
    m_camera.setPosition(glm::vec3(0.f, 0.f, 5.f));
    m_camera.setUp(glm::vec3(0.f, 1.f, 0.f));
}

RenderApplication::~RenderApplication()
{
    m_backgroundSoundEmitter = nullptr;
    m_soundSystem = nullptr;
    m_window = nullptr;
}

int RenderApplication::run()
{
    // Setup resources
    if (!setup())
        return 1;

    std::chrono::high_resolution_clock hpc;
    auto start = hpc.now();

    // Start background sound
    if (m_soundSystem->getManager()->hasSound("bg"))
    {
        auto bgsound = m_soundSystem->getManager()->getSound("bg");
        m_backgroundSoundEmitter->play(bgsound);
    }

    while (m_window->isOpen())
    {
        // Receive user input
        m_window->pollEvents();

        // Draw scene
        render();

        // Update sounds
        m_soundSystem->update(m_dtime);

        // Process user input
        processInput();

        // Swap screen buffers
        m_window->swapBuffers();

        // Time delta
        auto end = hpc.now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        m_dtime = ((float)diff.count()) / 1000.f;

        start = end;
    }
    return 0;
}

float RenderApplication::getTimeDelta() const { return m_dtime; }

const Window& RenderApplication::getWindow() const { return *m_window; }
const Camera& RenderApplication::getCamera() const { return m_camera; }

void RenderApplication::setCameraMovementSpeed(float speed) { m_cameraSpeed = speed; }
void RenderApplication::setCameraSensitivitySpeed(float sens) { m_cameraSensitivty = sens; }

void RenderApplication::processInput()
{
    auto dTime = getTimeDelta();
    // Camera view direction
    // Cursor offset
    auto offset = getWindow().getCursorMovement();
    const auto sensitivity = 5.0f;
    offset *= sensitivity * dTime;
    // Prevent losing precision by constraining yaw values
    m_cameraYaw = glm::mod(m_cameraYaw + offset.x, 360.f);
    m_cameraPitch += offset.y;

    // Camera constraints
    if (m_cameraPitch > 89.0f)
        m_cameraPitch = 89.0f;
    else if (m_cameraPitch < -89.0f)
        m_cameraPitch = -89.0f;

    // Calculates front vector
    m_camera.setDirection(m_cameraYaw, m_cameraPitch);

    // Forwards, backward
    if (getWindow().getKey(GLFW_KEY_W) == GLFW_PRESS)
        m_camera.setPosition(m_camera.getPosition() + m_camera.getDirection() * m_cameraSpeed * dTime);

    if (getWindow().getKey(GLFW_KEY_S) == GLFW_PRESS)
        m_camera.setPosition(m_camera.getPosition() - m_camera.getDirection() * m_cameraSpeed * dTime);
    
    // Left, right
    if (getWindow().getKey(GLFW_KEY_A) == GLFW_PRESS)
        m_camera.setPosition(m_camera.getPosition() - m_camera.getRight() * m_cameraSpeed * dTime);

    if (getWindow().getKey(GLFW_KEY_D) == GLFW_PRESS)
        m_camera.setPosition(m_camera.getPosition() + m_camera.getRight() * m_cameraSpeed * dTime);

    // Up / down
    if (getWindow().getKey(GLFW_KEY_E) == GLFW_PRESS)
        m_camera.setPosition(m_camera.getPosition() + m_camera.getUp() * m_cameraSpeed * dTime);

    if (getWindow().getKey(GLFW_KEY_Q) == GLFW_PRESS)
        m_camera.setPosition(m_camera.getPosition() - m_camera.getUp() * m_cameraSpeed * dTime);

}