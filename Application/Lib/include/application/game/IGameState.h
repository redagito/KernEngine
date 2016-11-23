#pragma once

#include <string>

class IGraphicsSystem;
class IInputProvider;
class IResourceManager;

/**
* \brief Game state interface class.
*
* A game state represents a high level state in a game.
* Several states can be chained together to form the high level game flow.
*/
class IGameState
{
public:
  /**
  * \brief Virtual destructor.
  */
  virtual ~IGameState();

  /**
  * \brief Explicit initialization.
  */
  virtual bool init(IGraphicsSystem *graphicsSystem,
                    IInputProvider *inputProvider,
                    IResourceManager *resourceManager) = 0;

  /**
  * \brief Called on entering state.
  */
  virtual void onEnter() = 0;

  /**
  * \brief Updates state.
  * \return False if state has ended, true otherwise.
  */
  virtual bool update(float dtime) = 0;

  /**
  * \brief Called on exiting state.
  */
  virtual void onExit() = 0;

  /**
  * \brief Returns requested state transition.
  * TODO: Should this return the actual next state?
  */
  virtual const std::string &getNextState() const = 0;
};