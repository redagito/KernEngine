#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "graphics/graphics/IGraphicsSystem.h"
#include "graphics/input/IInputProvider.h"

#include "application/game/IGameState.h"

/**
* \brief Manages game objects
*
* High level system for managing and updating the game objects and logic
*/
class CGameSystem
{
public:
  /**
  * \brief Default constructor.
  */
  CGameSystem();

  /**
  * \brief Cleans up all game objects.
  */
  ~CGameSystem();

  /**
  * \brief Updates the game objects
  */
  bool update(float dtime);

  /**
  * \brief Initialize game system with startup config file.
  */
  bool init(const std::string &initialState, IGraphicsSystem *graphicsSystem,
            IInputProvider *inputProvider, IResourceManager *resourceManager);

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

  /**
  * \brief Check if a state with the specified name exists
  */
  bool hasState(const std::string& name) const;

private:
  std::unordered_map<std::string, std::unique_ptr<IGameState>>
      m_gameStates;          /**< Game states stored by name. */
  IGameState *m_activeState; /**< Current game state. */
};
