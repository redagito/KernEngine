#include "graphics/game/state/CDemoState.h"

#include <foundation/debug/Log.h>

#include "graphics/graphics/camera/CFirstPersonCamera.h"
#include "graphics/io/CSceneLoader.h"

const std::string exitStr = "exit";

CDemoState::CDemoState(const std::string &sceneFile) : m_sceneFile(sceneFile) {}

CDemoState::~CDemoState()
{
  // Empty
}

bool CDemoState::init(IGraphicsSystem *graphicsSystem,
                      IInputProvider *inputProvider,
                      IResourceManager *resourceManager)
{
  m_graphicsSystem = graphicsSystem;
  // TODO Refactor, camera movement should be implemented with a single camera
  // and camera controllers.
  m_camera = std::make_shared<CFirstPersonCamera>(
      glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(0.f, 0.f, 1.f),
      glm::vec3(0.f, 1.f, 0.f), 45.f, 4.f / 3.f, 0.01f, 1000.f);

  m_cameraController = std::make_shared<CFreeFlightCameraController>();
  m_cameraController->setCamera(m_camera);
  m_cameraController->setInputProvider(inputProvider);
  m_camera->getFeatureInfoForWrite().lenseFlareActive = false;

  // Create new scene object
  m_scene = m_graphicsSystem->createScene();

  // Load scene from file
  LOG_INFO("Loading scene from file %s.", m_sceneFile.c_str());
  CSceneLoader loader(*resourceManager);
  if (!loader.load(m_sceneFile, *m_scene, m_animationWorld))
  {
    LOG_ERROR("Failed to load scene file %s.", m_sceneFile.c_str());
    return false;
  }
  return true;
}

void CDemoState::onEnter()
{
  m_graphicsSystem->setActiveCamera(m_camera.get());
  m_graphicsSystem->setActiveScene(m_scene);
}

bool CDemoState::update(float dtime)
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

void CDemoState::onExit()
{
  // Nothing
}

const std::string &CDemoState::getNextState() const { return exitStr; }