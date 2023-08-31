#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "kern/graphics/IGraphicsSystem.h"
#include "kern/graphics/input/IInputProvider.h"
#include "kern/audio/SoundSystem.h"

#include "kern/game/IGameState.h"

/**
 * \brief Manages game objects
 *
 * High level system for managing and updating the game objects and logic
 */
class GameSystem
{
   public:
    /**
     * \brief Default constructor.
     */
    GameSystem();

    /**
     * \brief Cleans up all game objects.
     */
    ~GameSystem();

    /**
     * \brief Updates the game objects
     */
    bool update(float dtime);

    /**
     * \brief Initialize game system with startup config file.
     */
    bool init(const std::string &initialState, IGraphicsSystem *graphicsSystem,
              IInputProvider *inputProvider, IResourceManager *resourceManager, SoundSystem* soundSystem);

    /**
     * \brief Add game state by name and initializes the state.
     * \return True if the state could be initialized and added. False otherwise.
     * State must be heap allocated, ownership is transferred to the game system.
     */
    bool addState(const std::string &name, IGameState *state);

    /**
     * \brief Removes state from list and deletes state object.
     */
    void removeState(const std::string &name);

   private:
    std::unordered_map<std::string, std::unique_ptr<IGameState>>
        m_gameStates;          /**< Game states stored by name. */
    IGameState *m_activeState; /**< Current game state. */
};
