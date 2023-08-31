#pragma once

#include "kern/game/GameSystem.h"

class GameApplication
{
   public:
    GameApplication();
    virtual ~GameApplication();
    bool init();
    void run();

   protected:
    // Manages game states and state transitions
    std::unique_ptr<GameSystem> m_gameSystem;
};