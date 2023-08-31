#include "kern/game/GameSystem.h"

#include <cassert>

#include <fmtlog/fmtlog.h>

GameSystem::~GameSystem() { logd("Game system destroyed."); }

GameSystem::GameSystem() : m_activeState(nullptr) { logd("Game system created."); }

// Updates game objects
bool GameSystem::update(float dtime)
{
    // Exit if state not set
    if (m_activeState == nullptr)
    {
        loge("No active state is set.");
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

bool GameSystem::init(const std::string &initialState, IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
                      IResourceManager *resourceManager, SoundSystem *soundSystem)
{
    logd("Initializing game system.");
    logd("Game system initial state set to {}.", initialState.c_str());

    // Initialize game states
    for (auto &entry : m_gameStates)
    {
        if (!entry.second->init(graphicsSystem, inputProvider, resourceManager, soundSystem))
        {
            loge("Failed to initialize game state {}.", entry.first.c_str());
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

bool GameSystem::addState(const std::string &name, IGameState *state)
{
    // Check for duplicates
    if (m_gameStates.count(name) != 0)
    {
        // State with name already exists
        logw("Overwriting game state {}.", name.c_str());
    }

    // Add state
    m_gameStates[name] = std::unique_ptr<IGameState>(state);
    logd("Added game state {} to game system.", name.c_str());
    return true;
}

void GameSystem::removeState(const std::string &name)
{
    if (m_gameStates.count(name) == 0)
    {
        logw("Could not remove state {}, the state does not exist.", name.c_str());
        return;
    }
    m_gameStates.erase(name);
    logd("Removed game state {} from game system.", name.c_str());
}
