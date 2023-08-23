#pragma once

#include <memory>

#include <audio/SoundSystem.h>

#include "gfx/Window.h"
#include "gfx/Camera.h"

/**
 * Provides basic rendering context and window creation.
 */
class RenderApplication
{
   public:
    RenderApplication();
    virtual ~RenderApplication();

    int run();

   protected:
    // Delta time in seconds
    float getTimeDelta() const;

    const Window& getWindow() const;
    const Camera& getCamera() const;

    void setCameraMovementSpeed(float speed);
    void setCameraSensitivitySpeed(float sens);
   private:
    void processInput();

    std::shared_ptr<SoundSystem> m_soundSystem;
    std::shared_ptr<Window> m_window;
    std::shared_ptr<SoundEmitter> m_backgroundSoundEmitter;
    float m_dtime = 0.f;

    // Default camera
    Camera m_camera;
    float m_cameraSpeed = 1.f;
    float m_cameraSensitivty = 2.f;
    float m_cameraYaw = -90.f;
    float m_cameraPitch = 0.f;

    virtual bool setup() = 0;
    virtual void render() = 0;
};