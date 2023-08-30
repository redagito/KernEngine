#include "control/HealthController.h"

#include <kern/game/GameObject.h>
#include <kern/graphics/collision/Collidable.h>

HealthController::HealthController(float health) : m_health(health), m_active(true) { return; }

HealthController::~HealthController() { return; }

void HealthController::attach(GameObject *object) { m_object = object; }

void HealthController::detach() { m_object = nullptr; }

void HealthController::update(float dtime)
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

void HealthController::setActive(bool state) { m_active = state; }

void HealthController::receiveMessage(Message msg) {}