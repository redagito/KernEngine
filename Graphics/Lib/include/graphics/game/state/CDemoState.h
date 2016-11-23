#pragma once

#include "graphics/game/IGameState.h"

#include "graphics/animation/CAnimationWorld.h"

#include "graphics/graphics/IScene.h"
#include "graphics/graphics/IGraphicsSystem.h"
#include "graphics/graphics/camera/IControllableCamera.h"

#include "graphics/engine/CFreeFlightCameraController.h"

#include "graphics/resource/IResourceManager.h"

/**
* \brief Represents the demo state of the engine.
*
* The demo state is used for graphical demos and has no game logic.
* A scene is loaded form a file and rendered with a freely movable camera
* controlled by the user.
*/
class CDemoState : public IGameState
{
public:
  CDemoState(const std::string &sceneFile);
  ~CDemoState();

  bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
            IResourceManager *resourceManager);

  void onEnter();
  bool update(float dtime);
  void onExit();
  const std::string &getNextState() const;

private:
  std::string m_sceneFile;

  std::shared_ptr<CFreeFlightCameraController> m_cameraController = nullptr;
  std::shared_ptr<IControllableCamera> m_camera = nullptr;
  IScene *m_scene = nullptr;
  IGraphicsSystem *m_graphicsSystem = nullptr;
  CAnimationWorld m_animationWorld;
};