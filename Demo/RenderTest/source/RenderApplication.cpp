#include "RenderApplication.h"

#include <chrono>

#include <audio/SoundSystem.h>

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