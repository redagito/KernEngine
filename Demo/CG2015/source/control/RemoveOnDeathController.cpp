#include "control/RemoveOnDeathController.h"

#include "kern/game/GameObject.h"
#include "state/GamePlayState.h"

RemoveOnDeathController::RemoveOnDeathController(GamePlayState *state, const std::shared_ptr<SoundEmitter> &emitter)
    : m_object(nullptr), m_gameState(state), m_active(true), m_emitter(emitter)
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
        m_emitter->play();
    }
}

void RemoveOnDeathController::setActive(bool state) { m_active = state; }

void RemoveOnDeathController::receiveMessage(Message msg) {}