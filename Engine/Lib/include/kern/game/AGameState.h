#pragma once

#include "kern/game/GameWorld.h"
#include "kern/game/IGameState.h"

/**
 * \brief Abstract game state base class.
 *
 * Provides common functionality for all game states.
 */
class AGameState : public IGameState
{
   public:
    virtual ~AGameState();

    GameWorld &getGameWorld();
    const GameWorld &getGameWorld() const;

    virtual bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
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
    GameWorld m_gameWorld;
};