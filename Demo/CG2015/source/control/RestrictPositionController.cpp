#include "control/RestrictPositionController.h"

#include <glm/ext.hpp>

#include "kern/game/GameObject.h"

RestrictPositionController::RestrictPositionController(const glm::vec2 &minCoords,
                                                         const glm::vec2 &maxCoords)
    : m_minCoords(minCoords), m_maxCoords(maxCoords)
{
    return;
}

RestrictPositionController::~RestrictPositionController() { return; }

void RestrictPositionController::attach(GameObject *object) { m_object = object; }

void RestrictPositionController::detach() { m_object = nullptr; }

void RestrictPositionController::setActive(bool state) { m_active = state; }

void RestrictPositionController::update(float dtime)
{
    if (m_active && m_object != nullptr)
    {
        glm::vec3 position = m_object->getPosition();
        bool update = false;

        // Check x
        if (position.x < m_minCoords.x)
        {
            position.x = m_maxCoords.x;
            update = true;
        }
        else if (position.x > m_maxCoords.x)
        {
            position.x = m_minCoords.x;
            update = true;
        }

        // Check y
        if (position.z < m_minCoords.y)
        {
            position.z = m_maxCoords.y;
            update = true;
        }
        else if (position.z > m_maxCoords.y)
        {
            position.z = m_minCoords.y;
            update = true;
        }

        // Update object position
        if (update)
        {
            m_object->setPosition(position);
        }
    }
}

void RestrictPositionController::receiveMessage(Message msg)
{
    // Empty
}