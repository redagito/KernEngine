#pragma once

#include <memory>

#include <audio/SoundSystem.h>

#include "gfx/Window.h"

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
   private:
    std::shared_ptr<SoundSystem> m_soundSystem;
    std::shared_ptr<Window> m_window;
    std::shared_ptr<SoundEmitter> m_backgroundSoundEmitter;
    float m_dtime = 0.f;

    virtual bool setup() = 0;
    virtual void render() = 0;
};