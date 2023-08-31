#include "state/DemoState.h"

#include <fmtlog/fmtlog.h>

#include "kern/graphics/camera/FirstPersonCamera.h"
#include "kern/graphics/io/SceneLoader.h"

const std::string exitStr = "exit";

DemoState::DemoState(const std::string &sceneFile) : m_sceneFile(sceneFile) {}

DemoState::~DemoState()
{
    // Empty
}

bool DemoState::init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider, IResourceManager *resourceManager,
                     SoundSystem *soundSystem)
{
    m_graphicsSystem = graphicsSystem;
    // TODO Refactor, camera movement should be implemented with a single camera
    // and camera controllers.
    m_camera = std::make_shared<FirstPersonCamera>(
        glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), 45.f,
        4.f / 3.f, 0.01f, 1000.f);

    m_cameraController = std::make_shared<FreeFlightCameraController>();
    m_cameraController->setCamera(m_camera);
    m_cameraController->setInputProvider(inputProvider);
    m_camera->getFeatureInfoForWrite().lenseFlareActive = false;

    // Create new scene object
    m_scene = m_graphicsSystem->createScene();

    // Load scene from file
    logi("Loading scene from file {}.", m_sceneFile.c_str());
    SceneLoader loader(*resourceManager);
    if (!loader.load(m_sceneFile, *m_scene, m_animationWorld))
    {
        loge("Failed to load scene file {}.", m_sceneFile.c_str());
        return false;
    }
    return true;
}

void DemoState::onEnter()
{
    m_graphicsSystem->setActiveCamera(m_camera.get());
    m_graphicsSystem->setActiveScene(m_scene);
}

bool DemoState::update(float dtime)
{
    // Key F5 starts replay of camera movement
    // Disabled and to be moved into game state
    // if (glfwGetKey(m_window->getGlfwHandle(), GLFW_KEY_F5) == GLFW_PRESS &&
    // f5Cooldown <= 0.f)
    // {
    //     f5Cooldown = 0.3f;
    //     m_cameraController->loadSequence("data/democam.json");
    // }
    m_cameraController->animate((float)dtime);
    m_animationWorld.update(dtime);
    return true;
}

void DemoState::onExit()
{
    // Nothing
}

const std::string &DemoState::getNextState() const { return exitStr; }