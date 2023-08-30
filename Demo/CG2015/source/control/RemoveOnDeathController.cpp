#include "control/RemoveOnDeathController.h"

#include "kern/game/GameObject.h"
#include "state/GamePlayState.h"

RemoveOnDeathController::RemoveOnDeathController(GamePlayState *state)
    : m_object(nullptr), m_gameState(state), m_active(true)
{
    return;
}

RemoveOnDeathController::~RemoveOnDeathController() { return; }

void RemoveOnDeathController::attach(GameObject *object) { m_object = object; }

void RemoveOnDeathController::detach() { m_object = nullptr; }

void RemoveOnDeathController::update(float dtime)
{
    if (m_object != nullptr && m_active && m_object->isDead())
    {
        // Mark for deletion
        m_gameState->winCount();
        m_object->markDeleted();
    }
}

void RemoveOnDeathController::setActive(bool state) { m_active = state; }

void RemoveOnDeathController::receiveMessage(Message msg) {}