#include "control/CPlayerMovementController.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include "kern/foundation/TransformUtils.h"
#include "kern/graphics/input/IInputProvider.h"

#include "kern/game/GameObject.h"

CPlayerMovementController::CPlayerMovementController(IInputProvider *inputProvider, float speed)
    : m_inputProvider(inputProvider), m_speed(speed)
{
    return;
}

CPlayerMovementController::~CPlayerMovementController() { return; }

void CPlayerMovementController::attach(GameObject *object) { m_object = object; }

void CPlayerMovementController::detach()
{
    m_object = nullptr;
    m_rotationDegree = 0.f;
}

void CPlayerMovementController::setActive(bool state) { m_active = state; }

void CPlayerMovementController::update(float dtime)
{
    if (m_active && m_object != nullptr)
    {
        // const float pitchSpeed = 1.f;
        // const float rollSpeed = 1.f;
        const float yawSpeed = 2.f;

        glm::vec3 rotation(0.f);
        glm::vec3 position = m_object->getPosition();

        // Move up
        if (m_inputProvider->isKeyPressed(GLFW_KEY_W))
        {
            position.y += m_speed * dtime;
        }

        // Move down
        if (m_inputProvider->isKeyPressed(GLFW_KEY_S))
        {
            position.y -= m_speed * dtime;
        }

        // Rotate left
        if (m_inputProvider->isKeyPressed(GLFW_KEY_D))
        {
            rotation.y -= yawSpeed * dtime;
        }

        // Rotate right
        if (m_inputProvider->isKeyPressed(GLFW_KEY_A))
        {
            rotation.y += yawSpeed * dtime;
        }

        // Update rotation
        m_object->setRotation(m_object->getRotation() + rotation);

        // Calculate final position
        position += m_object->getForward() * dtime * m_speed;

        // Restrict y
        if (position.y < 20.f)
        {
            position.y = 20.f;
        }
        else if (position.y > 50.f)
        {
            position.y = 50.f;
        }

        // Update translation
        m_object->setPosition(position);
    }
}

void CPlayerMovementController::receiveMessage(Message msg) {}
