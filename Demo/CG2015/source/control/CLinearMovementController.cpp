#include "control/CLinearMovementController.h"

#include <GLFW/glfw3.h>

#include "application/game/CGameObject.h"

CLinearMovementController::CLinearMovementController(const glm::vec3 &direction, float speed)
    : m_direction(direction), m_speed(speed)
{
    return;
}

CLinearMovementController::~CLinearMovementController() {}

void CLinearMovementController::attach(CGameObject *object) { m_object = object; }

void CLinearMovementController::detach() { m_object = nullptr; }

void CLinearMovementController::update(float dtime)
{
    if (m_active && m_object != nullptr)
    {
        m_object->setPosition(m_object->getPosition() + m_direction * m_speed * dtime);
    }
}

void CLinearMovementController::setActive(bool state) { m_active = state; }

void CLinearMovementController::receiveMessage(Message msg) {}