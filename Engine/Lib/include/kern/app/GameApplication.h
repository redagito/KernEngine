#pragma once

#include "kern/resource/IResourceManager.h"
#include "kern/game/GameSystem.h"
#include "kern/audio/SoundSystem.h"

class GameApplication
{
   public:
    GameApplication();
    virtual ~GameApplication();

    bool init();
    void run();

   protected:
    // Resource manager
    std::unique_ptr<IResourceManager> m_resourceManager;
    // Manages game states and state transitions
    std::unique_ptr<GameSystem> m_gameSystem;
    // Creates and manages sounds and sound emitters
    std::unique_ptr<SoundSystem> m_soundSystem;
};