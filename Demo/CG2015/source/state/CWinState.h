#pragma once

#include "application/game/IGameState.h"
#include "application/game/CGameSystem.h"

#include "graphics/animation/CAnimationWorld.h"
#include "graphics/graphics/IScene.h"
#include "graphics/graphics/IGraphicsSystem.h"
#include "graphics/graphics/ICamera.h"
#include "graphics/resource/IResourceManager.h"
#include "graphics/input/IInputProvider.h"

/**
* \brief Lose state, similar to demo state with static camera.
*/
class CWinState : public IGameState
{
public:
  CWinState(const std::string &sceneFile);
  ~CWinState();

  bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
            IResourceManager *resourceManager);

  void onEnter();
  bool update(float dtime);
  void onExit();
  const std::string &getNextState() const;

private:
  float m_fadeInTime;
  std::string m_sceneFile;
  IInputProvider *m_inputProvider = nullptr;
  std::shared_ptr<ICamera> m_camera = nullptr;
  IScene *m_scene = nullptr;
  IGraphicsSystem *m_graphicsSystem = nullptr;
  IResourceManager *m_resourceManager = nullptr;
  CAnimationWorld m_animationWorld;
  std::string m_nextState; /**< Next state. */
};
