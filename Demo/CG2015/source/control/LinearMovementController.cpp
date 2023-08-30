#include "control/LinearMovementController.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "kern/game/GameObject.h"

LinearMovementController::LinearMovementController(const glm::vec3 &direction, float speed)
    : m_direction(direction), m_speed(speed)
{
    return;
}

LinearMovementController::~LinearMovementController() {}

void LinearMovementController::attach(GameObject *object) { m_object = object; }

void LinearMovementController::detach() { m_object = nullptr; }

void LinearMovementController::update(float dtime)
{
    if (m_active && m_object != nullptr)
    {
        m_object->setPosition(m_object->getPosition() + m_direction * m_speed * dtime);
    }
}

void LinearMovementController::setActive(bool state) { m_active = state; }

void LinearMovementController::receiveMessage(Message msg) {}