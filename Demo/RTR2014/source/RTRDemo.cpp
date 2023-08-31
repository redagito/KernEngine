#include "RTRDemo.h"

#include <fmtlog/fmtlog.h>

#include <string>
#include <vector>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <kern/audio/SoundSystem.h>
#include <kern/graphics/animation/AnimationWorld.h>
#include <kern/graphics/camera/FirstPersonCamera.h>
#include <kern/graphics/input/GlfwInputProvider.h>
#include <kern/graphics/io/SceneLoader.h>
#include <kern/graphics/renderer/DeferredRenderer.h>
#include <kern/graphics/renderer/ForwardRenderer.h>
#include <kern/graphics/renderer/RendererCoreConfig.h>
#include <kern/graphics/resource/GraphicsResourceManager.h>
#include <kern/graphics/scene/Scene.h>
#include <kern/graphics/window/GlfwWindow.h>
#include <kern/resource/ResourceManager.h>

#include "CameraController.h"

RTRDemo::RTRDemo() 
{ 
    m_soundSystem = std::make_unique<SoundSystem>("data/audio");
    m_bgMusic = m_soundSystem->createEmitter();
    m_bgSfx = m_soundSystem->createEmitter();
}

RTRDemo::~RTRDemo() {}

int RTRDemo::init(const std::string& configFile)
{
    if (!m_config.load(configFile))
    {
        logw("Failed to load config file {}, starting with default settings.", configFile.c_str());
    }

    // Create window for rendering
    if (!initWindow())
    {
        loge("Failed to initialize window.");
        return 1;
    }

    m_inputProvider = std::make_shared<GlfwInputProvider>(m_window->getGlfwHandle());

    // Create resource manager
    m_resourceManager = std::make_shared<ResourceManager>();
    if (m_resourceManager == nullptr)
    {
        loge("Failed to initialize resource manager.");
        return 1;
    }

    // Create animation world
    m_animationWorld = std::make_shared<AnimationWorld>();

    // Graphics resource manager, listens to resource manager
    GraphicsResourceManager* manager = new GraphicsResourceManager;
    m_resourceManager->addResourceListener(manager);
    m_graphicsResourceManager.reset(manager);

    // Create renderer
    if (!initRenderer())
    {
        loge("Failed to initialize renderer.");
        return 1;
    }

    if (!initScene())
    {
        loge("Failed to initialize scene.");
        return 1;
    }

    m_camera = std::make_shared<FirstPersonCamera>(glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(0.f, 0.f, 1.f),
                                                   glm::vec3(0.f, 1.f, 0.f), 45.f, 4.f / 3.f, 0.01f, 1000.f);

    m_cameraController = std::make_shared<CameraController>();
    m_cameraController->setCamera(m_camera);
    m_cameraController->setInputProvider(m_inputProvider.get());

    m_window->addListener(m_cameraController.get());

    // Sound
    m_soundSystem->getManager()->registerSound("demobgm", "inspiring-cinematic-ambient-116199.mp3");
    auto soundBgm = m_soundSystem->getManager()->getSound("demobgm");
    m_bgMusic->setSound(soundBgm);
    m_bgMusic->setVolume(0.8);
    m_bgMusic->setLooping(true);
    
    m_soundSystem->getManager()->registerSound("demosfx", "electric-windmill-74468.mp3");
    auto soundSfx = m_soundSystem->getManager()->getSound("demosfx");
    m_bgSfx->setSound(soundSfx);
    m_bgSfx->setVolume(0.3);
    m_bgSfx->setLooping(true);

    return 0;
}

int RTRDemo::run()
{
    double f1Cooldown = 0.0;
    double f2Cooldown = 0.0;
    double f3Cooldown = 0.0;
    double f5Cooldown = 0.0;
    double k1Cooldown = 0.0;
    double timeDiff = 0.0;

    double fpsCoolDown = 1.f;
    unsigned int lastFrameCount = 0;
    unsigned int currentFrameCount = 0;

    bool displayDebugInfo = false;

    m_window->toggleMouseCapture();
    m_bgMusic->play();
    m_bgSfx->play();

    do
    {
        double startTime = glfwGetTime();

        // Cooldowns
        f1Cooldown -= timeDiff;
        f2Cooldown -= timeDiff;
        f3Cooldown -= timeDiff;
        f5Cooldown -= timeDiff;
        k1Cooldown -= timeDiff;
        fpsCoolDown -= timeDiff;

        if (fpsCoolDown < 0)
        {
            fpsCoolDown += 1.f;
            lastFrameCount = currentFrameCount;
            currentFrameCount = 0;
        }

        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_1) == GLFW_PRESS && k1Cooldown <= 0.f)
        {
            k1Cooldown = 0.3f;
            displayDebugInfo = !displayDebugInfo;
        }

        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F2) == GLFW_PRESS && f2Cooldown <= 0.f)
        {
            f2Cooldown = 0.3f;
            m_renderer = m_deferredRenderer;
        }

        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F3) == GLFW_PRESS && f3Cooldown <= 0.f)
        {
            f3Cooldown = 0.3f;
            m_renderer = m_forwardRenderer;
        }

        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F5) == GLFW_PRESS && f5Cooldown <= 0.f)
        {
            f5Cooldown = 0.3f;
            m_cameraController->loadSequence("data/democam.json");
        }

        m_cameraController->animate((float)timeDiff);

        m_renderer->draw(*m_scene.get(), *m_camera.get(), *m_window.get(), *m_graphicsResourceManager.get());

        // Perform animation update
        m_animationWorld->update((float)timeDiff);

        m_window->swapBuffer();

        // Update input
        glfwPollEvents();

        if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F1) == GLFW_PRESS && f1Cooldown <= 0.f)
        {
            // Reset cooldown
            f1Cooldown = 0.5f;
            // Capure/uncapture mouse
            m_window->toggleMouseCapture();
        }

        timeDiff = glfwGetTime() - startTime;
        ++currentFrameCount;

    } while (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(m_window->getGlfwHandle()) == 0);

    glfwTerminate();

    return 0;
}

bool RTRDemo::initWindow()
{
    // Check if already initialized
    if (m_window != nullptr)
    {
        return true;
    }

    logi("Initializing application window.");

    // Read config values
    unsigned int width = m_config.getValue("window", "width", 800);
    unsigned int height = m_config.getValue("window", "height", 600);
    std::string title = m_config.getValue("window", "title", "Demo");

    logi("Window width: {}.", width);
    logi("Window height: {}.", height);
    logi("Window title: {}.", title);

    // Create window
    GlfwWindow* window = new GlfwWindow;
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

bool RTRDemo::initRenderer()
{
    if (m_renderer != nullptr || m_deferredRenderer != nullptr || m_forwardRenderer != nullptr)
    {
        logi("Renderer already initialized and re-initialization is not supported.");
        return true;
    }

    // Initialize deferred renderer
    logi("Initializing deferred renderer.");
    m_deferredRenderer.reset(DeferredRenderer::create(*m_resourceManager));
    if (m_deferredRenderer == nullptr)
    {
        loge("Failed to initialize deferred renderer.");
        return false;
    }

    // Initialize forward renderer
    logi("Initializing forward renderer.");
    m_forwardRenderer.reset(ForwardRenderer::create(*m_resourceManager));
    if (m_forwardRenderer == nullptr)
    {
        loge("Failed to initialize forward renderer.");
        return false;
    }

    // Set renderer
    std::string rendererType = m_config.getValue("renderer", "type", "forward");
    logi("Initial renderer type set to {}.", rendererType.c_str());

    // Set renderer object
    if (rendererType == "forward")
    {
        m_renderer = m_forwardRenderer;
    }
    else if (rendererType == "deferred")
    {
        m_renderer = m_deferredRenderer;
    }
    else
    {
        // Should not happen as default renderer is set to forward in config call
        logw("Invalid renderer type {} specified. Fallback to forward renderer.", rendererType.c_str());
        m_renderer = m_forwardRenderer;
    }

    // Initialize renderer resources
    if (m_renderer == nullptr)
    {
        loge("Failed to initialize renderer.");
        return false;
    }
    return true;
}

bool RTRDemo::initScene()
{
    m_scene = std::make_shared<Scene>(m_graphicsResourceManager.get());
    SceneLoader loader(*m_resourceManager);

    // Get startup scene from config
    std::string sceneFile = m_config.getValue("scene", "file", "data/scene/test_1.json");
    logi("Loading initial scene from file {}.", sceneFile.c_str());
    if (!loader.load(sceneFile, *m_scene, *m_animationWorld))
    {
        loge("Failed to load scene file {}.", sceneFile.c_str());
        m_scene = nullptr;
        return false;
    }
    return true;
}