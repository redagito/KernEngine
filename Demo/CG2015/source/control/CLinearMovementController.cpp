#include "control/CLinearMovementController.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "kern/game/GameObject.h"

CLinearMovementController::CLinearMovementController(const glm::vec3 &direction, float speed)
    : m_direction(direction), m_speed(speed)
{
    return;
}

CLinearMovementController::~CLinearMovementController() {}

void CLinearMovementController::attach(GameObject *object) { m_object = object; }

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