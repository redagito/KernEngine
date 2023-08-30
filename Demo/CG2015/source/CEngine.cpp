#include "CEngine.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fmtlog/fmtlog.h>

#include <cassert>
#include <string>
#include <vector>

#include "CFreeFlightCameraController.h"
#include "kern/foundation/IniFile.h"
#include "kern/foundation/JsonUtil.h"
#include "kern/foundation/StringUtil.h"
#include "kern/foundation/TimeStamp.h"
#include "kern/game/GameSystem.h"
#include "kern/graphics/animation/AnimationWorld.h"
#include "kern/graphics/camera/FirstPersonCamera.h"
#include "kern/graphics/camera/FreeCamera.h"
#include "kern/graphics/input/GlfwInputProvider.h"
#include "kern/graphics/io/SceneLoader.h"
#include "kern/graphics/renderer/DeferredRenderer.h"
#include "kern/graphics/renderer/ForwardRenderer.h"
#include "kern/graphics/renderer/core/RendererCoreConfig.h"
#include "kern/graphics/renderer/debug/RendererDebug.h"
#include "kern/graphics/resource/GraphicsResourceManager.h"
#include "kern/graphics/resource/Resource.h"
#include "kern/graphics/scene/Scene.h"
#include "kern/graphics/system/GraphicsSystem.h"
#include "kern/graphics/window/GlfwWindow.h"
#include "state/CDemoState.h"
#include "state/CGamePlayState.h"
#include "state/CLoadState.h"
#include "state/CLoseState.h"
#include "state/CTitleState.h"
#include "state/CWinState.h"

CEngine::CEngine() {}

CEngine::~CEngine() { m_gameSystem = nullptr; }

bool CEngine::init(const char *configFile)
{
    // Load configuration file
    if (!load(configFile, m_engineConfig))
    {
        logw("Failed to load config file {}. Starting with default settings.", configFile);
        // TODO Return if no config exists?
    }

    // Create window for rendering
    if (!initWindow(m_engineConfig.m_windowWidth, m_engineConfig.m_windowHeight, m_engineConfig.m_windowTitle))
    {
        loge("Failed to initialize window.");
        return false;
    }
    // TODO GLFW handle not properly wrapped away, GFLW should not be used
    // directly
    m_inputProvider = std::make_shared<GlfwInputProvider>(m_window->getGlfwHandle());

    // Create central resource manager
    m_resourceManager.reset(createResourceManager());
    if (m_resourceManager == nullptr)
    {
        loge("Failed to initialize resource manager.");
        return false;
    }

    // Create and initialize graphics system
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    if (!m_graphicsSystem->init(*m_resourceManager))
    {
        loge("Failed to initialize graphics system.");
        return false;
    }

    // Legacy stuff to keep demo mode working
    // TODO Should be removed
    if (!m_engineConfig.m_sceneFile.empty() && m_engineConfig.m_modeType != "game")
    {
        if (!initDemo(m_engineConfig.m_sceneFile))
        {
            loge("Failed to initialize demo mode.");
            return false;
        }
    }
    else if (!m_engineConfig.m_gameFile.empty())  // Create and initialize game system
    {
        if (!initGameSystem(m_engineConfig.m_gameFile))
        {
            loge("Failed to initialize game system.");
            return false;
        }
    }
    else
    {
        loge("No scene file or game file specified");
        return false;
    }
    return true;
}

void CEngine::run()
{
    // Fn key cooldown values
    double f1Cooldown = 0.0;
    double f2Cooldown = 0.0;
    double f3Cooldown = 0.0;
    double f4Cooldown = 0.0;
    double f5Cooldown = 0.0;
    double f6Cooldown = 0.0;
    double f7Cooldown = 0.0;
    double f8Cooldown = 0.0;
    double f9Cooldown = 0.0;

    // Numeric key cooldown values
    double k1Cooldown = 0.0;
    double k2Cooldown = 0.0;
    double k3Cooldown = 0.0;
    double k4Cooldown = 0.0;
    double k5Cooldown = 0.0;

    double timeDiff = 0.0;
    bool running = true;

    // Activate mouse capture as default
    m_window->toggleMouseCapture();

    do
    {
        double startTime = glfwGetTime();

        // Cooldown calculations
        f1Cooldown -= timeDiff;
        f2Cooldown -= timeDiff;
        f3Cooldown -= timeDiff;
        f4Cooldown -= timeDiff;
        f5Cooldown -= timeDiff;
        f6Cooldown -= timeDiff;
        f7Cooldown -= timeDiff;
        f8Cooldown -= timeDiff;
        f9Cooldown -= timeDiff;

        k1Cooldown -= timeDiff;
        k2Cooldown -= timeDiff;
        k3Cooldown -= timeDiff;
        k4Cooldown -= timeDiff;
        k5Cooldown -= timeDiff;

        // Global key mappings
        // Key F1
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F1) == GLFW_PRESS && f1Cooldown <= 0.f)
        {
            f1Cooldown = 0.3f;
            // Not implemented
        }

        // Key F2
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F2) == GLFW_PRESS && f2Cooldown <= 0.f)
        {
            f2Cooldown = 0.3f;
            // Turns debug overlay on / off
            m_graphicsSystem->toggleDebugOverlay();
        }

        // Key F3
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F3) == GLFW_PRESS && f3Cooldown <= 0.f)
        {
            f3Cooldown = 0.3f;
            // Turns wireframe mode on / off
            m_graphicsSystem->toggleWireframeMode();
        }

        // Key F4
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F4) == GLFW_PRESS && f4Cooldown <= 0.f)
        {
            f4Cooldown = 0.3f;
            // Toggles global texture filtering between nearest neighbor and bilinear
            // TODO Implement
        }

        // Key F5
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F5) == GLFW_PRESS && f5Cooldown <= 0.f)
        {
            f5Cooldown = 0.3f;
            // Toggles global texture mip map filtering between off, nearest neighbor
            // and bilinear
            // TODO Implement
        }

        // Key F6
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F6) == GLFW_PRESS && f6Cooldown <= 0.f)
        {
            f6Cooldown = 0.3f;
            // Unused
        }

        // Key F7
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F7) == GLFW_PRESS && f7Cooldown <= 0.f)
        {
            f7Cooldown = 0.3f;
            // Unused
        }

        // Key F8
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F8) == GLFW_PRESS && f8Cooldown <= 0.f)
        {
            f8Cooldown = 0.3f;
            // Toggles view frustum culling on/off
            m_graphicsSystem->toggleViewFrustumCulling();
        }

        // Key F9
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F9) == GLFW_PRESS && f9Cooldown <= 0.f)
        {
            f9Cooldown = 0.3f;
            m_graphicsSystem->toggleDebugOverlayTransparency();
        }

        // Key 1
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_1) == GLFW_PRESS && k1Cooldown <= 0.f)
        {
            k1Cooldown = 0.3f;
            // Turns mouse capture on/off
            m_window->toggleMouseCapture();
        }

        // Key 2
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_2) == GLFW_PRESS && k2Cooldown <= 0.f)
        {
            k2Cooldown = 0.3f;
            // Sets active rendering device to deferred renderer
            m_graphicsSystem->setActiveRenderer("deferred");
        }

        // Key 3
        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_3) == GLFW_PRESS && k3Cooldown <= 0.f)
        {
            k3Cooldown = 0.3f;
            // Sets active rendering device to forward renderer
            m_graphicsSystem->setActiveRenderer("forward");
        }

        // Game system update
        if (!m_gameSystem->update((float)timeDiff))
        {
            // On return false quit application
            running = false;
        }

        // Draw active scene from active camera with active rendering device
        m_graphicsSystem->draw(*m_window);

        // Swap buffers after draw
        m_window->swapBuffer();

        // Update input
        glfwPollEvents();

        // Frame time
        timeDiff = glfwGetTime() - startTime;

    } while (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(m_window->getGlfwHandle()) == 0 && running);

    glfwTerminate();
    return;
}

bool CEngine::initWindow(unsigned int width, unsigned int height, const std::string &title)
{
    // Check if already initialized
    if (m_window != nullptr)
    {
        return true;
    }

    logi("Initializing application window.");
    logi("Window width: {}.", width);
    logi("Window height: {}.", height);
    logi("Window title: {}.", title.c_str());

    // Create window
    GlfwWindow *window = new GlfwWindow;
    if (!window->init(width, height, title))
    {
        loge("Failed to initialize GLFW window wrapper.");
        delete window;
        return false;
    }

    // Set pointer
    m_window.reset(window);
    return true;
}

bool CEngine::initGameSystem(const std::string &gameFile)
{
    // Create and set game system
    m_gameSystem = std::make_shared<GameSystem>();

    // TODO Load from game file
    m_gameSystem->addState("load", new CLoadState("data/world/load_1.json", 10.f));
    m_gameSystem->addState("title", new CTitleState("data/world/intro_1.json"));
    m_gameSystem->addState("game", new CGamePlayState());
    m_gameSystem->addState("lose", new CLoseState("data/world/lose.json"));
    m_gameSystem->addState("win", new CWinState("data/world/win.json"));
    if (!m_gameSystem->init("load", m_graphicsSystem.get(), m_inputProvider.get(), m_resourceManager.get()))
    {
        loge("Failed to initialize game system.");
        return false;
    }

    return true;
}

bool CEngine::initDemo(const std::string &sceneFile)
{
    // Create and set game system
    m_gameSystem = std::make_shared<GameSystem>();

    // Create demo state from with scene file
    m_gameSystem->addState("demo", new CDemoState(sceneFile));
    if (!m_gameSystem->init("demo", m_graphicsSystem.get(), m_inputProvider.get(), m_resourceManager.get()))
    {
        loge("Failed to initialize game system.");
        return false;
    }
    return true;
}
