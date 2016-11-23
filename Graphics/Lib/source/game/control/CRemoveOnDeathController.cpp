#include "graphics/game/control/CRemoveOnDeathController.h"

#include "graphics/game/CGameObject.h"
#include "graphics/game/state/CGamePlayState.h"

CRemoveOnDeathController::CRemoveOnDeathController(CGamePlayState *state)
    : m_object(nullptr), m_gameState(state), m_active(true)
{
  return;
}

CRemoveOnDeathController::~CRemoveOnDeathController() { return; }

void CRemoveOnDeathController::attach(CGameObject *object)
{
  m_object = object;
}

void CRemoveOnDeathController::detach() { m_object = nullptr; }

void CRemoveOnDeathController::update(float dtime)
{
  if (m_object != nullptr && m_active && m_object->isDead())
  {
    // Mark for deletion
    m_gameState->winCount();
    m_object->markDeleted();
  }
}

void CRemoveOnDeathController::setActive(bool state) { m_active = state; }

void CRemoveOnDeathController::receiveMessage(Message msg) {}