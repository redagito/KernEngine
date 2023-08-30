#include "state/CLoseState.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <fmtlog/fmtlog.h>

#include "kern/graphics/camera/FirstPersonCamera.h"
#include "kern/graphics/io/SceneLoader.h"

const std::string gameStr = "game";

CLoseState::CLoseState(const std::string &sceneFile) : m_sceneFile(sceneFile) {}

CLoseState::~CLoseState()
{
    // Empty
}

bool CLoseState::init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
                      IResourceManager *resourceManager)
{
    m_graphicsSystem = graphicsSystem;
    m_inputProvider = inputProvider;
    m_resourceManager = resourceManager;

    // TODO Refactor, camera movement should be implemented with a single camera
    // and camera controllers.
    m_camera = std::make_shared<FirstPersonCamera>(
        glm::vec3(-4.4f, 5.f, 14.5), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 45.f,
        4.f / 3.f, 0.01f, 1000.f);

    m_camera->getFeatureInfoForWrite().godRayActive = false;

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

void CLoseState::onEnter()
{
    m_fadeInTime = 3.f;
    m_graphicsSystem->setActiveCamera(m_camera.get());
    m_graphicsSystem->setActiveScene(m_scene);
}

bool CLoseState::update(float dtime)
{
    // Ignore user input for some time
    if (m_fadeInTime > 0.0f)
    {
        m_fadeInTime -= dtime;
    }

    m_animationWorld.update(dtime);
    return true;
}

void CLoseState::onExit()
{
    // Nothing
}

const std::string &CLoseState::getNextState() const { return gameStr; }