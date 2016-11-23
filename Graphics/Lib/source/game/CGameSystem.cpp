#include "graphics/game/CGameSystem.h"

#include <cassert>

#include <foundation/debug/Log.h>

CGameSystem::~CGameSystem() { LOG_DEBUG("Game system destroyed."); }

CGameSystem::CGameSystem() : m_activeState(nullptr)
{
  LOG_DEBUG("Game system created.");
}

// Updates game objects
bool CGameSystem::update(float dtime)
{
  // Exit if state not set
  if (m_activeState == nullptr)
  {
    LOG_ERROR("No active state is set.");
    return false;
  }

  // Update current state
  if (!m_activeState->update(dtime))
  {
    // State ended
    m_activeState->onExit();
    // Check state exists
    if (m_gameStates.count(m_activeState->getNextState()) == 0)
    {
      // State transition error
      return false;
    }
    // Set next state
    m_activeState = m_gameStates.at(m_activeState->getNextState()).get();
    m_activeState->onEnter();
  }
  // Update success
  return true;
}

bool CGameSystem::init(const std::string &initialState,
                       IGraphicsSystem *graphicsSystem,
                       IInputProvider *inputProvider,
                       IResourceManager *resourceManager)
{
  LOG_DEBUG("Initializing game system.");
  LOG_DEBUG("Game system initial state set to %s.", initialState.c_str());

  // Initialize game states
  for (auto &entry : m_gameStates)
  {
    if (!entry.second->init(graphicsSystem, inputProvider, resourceManager))
    {
      LOG_ERROR("Failed to initialize game state %s.", entry.first.c_str());
      return false;
    }
  }

  // Set initial state active
  assert(m_gameStates.count(initialState) != 0);
  m_activeState = m_gameStates.at(initialState).get();

  // On enter state trigger
  m_activeState->onEnter();
  return true;
}

bool CGameSystem::addState(const std::string &name, IGameState *state)
{
  // Check for duplicates
  if (m_gameStates.count(name) != 0)
  {
    // State with name already exists
    LOG_WARNING("Overwriting game state %s.", name.c_str());
  }

  // Add state
  m_gameStates[name] = std::unique_ptr<IGameState>(state);
  LOG_DEBUG("Added game state %s to game system.", name.c_str());
  return true;
}

void CGameSystem::removeState(const std::string &name)
{
  if (m_gameStates.count(name) == 0)
  {
    LOG_WARNING("Could not remove state %s, the state does not exist.",
                name.c_str());
    return;
  }
  m_gameStates.erase(name);
  LOG_DEBUG("Removed game state %s from game system.", name.c_str());
}
