#include "kern/game/AGameState.h"

const std::string errStr("Error");

AGameState::~AGameState()
{
    // Empty
}

GameWorld &AGameState::getGameWorld() { return m_gameWorld; }

const GameWorld &AGameState::getGameWorld() const { return m_gameWorld; }

bool AGameState::init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
                      IResourceManager *resourceManager)
{
    return true;
}

void AGameState::onEnter() { return; }

bool AGameState::update(float dtime)
{
    getGameWorld().update(dtime);
    return !isStateTransitionTriggered();
}

void AGameState::onExit() { return; }

const std::string &AGameState::getNextState() const { return m_nextStateId; }

void AGameState::triggerStateTransition(const std::string &nextState)
{
    m_stateTransitionTriggered = true;
    m_nextStateId = nextState;
}

bool AGameState::isStateTransitionTriggered() const { return m_stateTransitionTriggered; }