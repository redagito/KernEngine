#pragma once

#include "graphics/game/IGameState.h"
#include "graphics/game/CGameWorld.h"

/**
* \brief Abstract game state base class.
*
* Provides common functionality for all game states.
*/
class AGameState : public IGameState
{
public:
  virtual ~AGameState();

  CGameWorld &getGameWorld();
  const CGameWorld &getGameWorld() const;

  virtual bool init(IGraphicsSystem *graphicsSystem,
                    IInputProvider *inputProvider,
                    IResourceManager *resourceManager);
  virtual void onEnter();
  virtual bool update(float dtime);
  virtual void onExit();
  virtual const std::string &getNextState() const;

  void triggerStateTransition(const std::string &nextState);
  bool isStateTransitionTriggered() const;

private:
  bool m_stateTransitionTriggered = false;
  std::string m_nextStateId = "exit";
  CGameWorld m_gameWorld;
};