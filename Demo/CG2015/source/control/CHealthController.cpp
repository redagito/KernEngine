#include "control/CHealthController.h"

#include "kern/game/GameObject.h"
#include "kern/graphics/collision/Collidable.h"

CHealthController::CHealthController(float health) : m_health(health), m_active(true) { return; }

CHealthController::~CHealthController() { return; }

void CHealthController::attach(GameObject *object) { m_object = object; }

void CHealthController::detach() { m_object = nullptr; }

void CHealthController::update(float dtime)
{
    if (m_active && m_object != nullptr && m_object->hasCollidable())
    {
        // Set health
        m_health -= m_object->getCollidable()->getDamageReceived();
        if (m_health <= 0.f)
        {
            // Set object into death state
            m_object->setDead();
        }
    }
}

void CHealthController::setActive(bool state) { m_active = state; }

void CHealthController::receiveMessage(Message msg) {}