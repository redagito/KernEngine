#include "state/LoadState.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fmtlog/fmtlog.h>

#include "kern/graphics/camera/FirstPersonCamera.h"
#include "kern/graphics/io/SceneLoader.h"

const std::string titleStr = "title";

LoadState::LoadState(const std::string &sceneFile, float transitionTime)
    : m_transitionTime(transitionTime), m_sceneFile(sceneFile)
{
}

LoadState::~LoadState()
{
    // Empty
}

bool LoadState::init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
                      IResourceManager *resourceManager)
{
    m_graphicsSystem = graphicsSystem;
    m_resourceManager = resourceManager;

    // TODO Refactor, camera movement should be implemented with a single camera
    // and camera controllers.
    m_camera = std::make_shared<FirstPersonCamera>(
        glm::vec3(6.f, 0.f, -1.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 45.f,
        4.f / 3.f, 0.01f, 1000.f);

    // Disable lense flare
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

void LoadState::onEnter()
{
    m_timeLeft = m_transitionTime;
    m_graphicsSystem->setActiveCamera(m_camera.get());
    m_graphicsSystem->setActiveScene(m_scene);
}

bool LoadState::update(float dtime)
{
    // Ignore user input for some time
    if (m_timeLeft > 0.0f)
    {
        m_timeLeft -= dtime;
    }
    else
    {
        // Transition to title state
        return false;
    }
    m_animationWorld.update(dtime);
    return true;
}

void LoadState::onExit()
{
    // Nothing
}

const std::string &LoadState::getNextState() const { return titleStr; }
