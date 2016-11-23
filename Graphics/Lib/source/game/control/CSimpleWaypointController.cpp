#include "CSimpleWaypointController.h"

#include "game/CGameObject.h"
#include "math/TransformUtils.h"

#include "game/state/AGameState.h"

#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

CSimpleWaypointController::CSimpleWaypointController(const glm::vec3 &end,
                                                     float speed,
                                                     AGameState *state)
    : m_end(end), m_speed(speed), m_gameState(state)
{
  return;
}

CSimpleWaypointController::~CSimpleWaypointController() { return; }

void CSimpleWaypointController::attach(CGameObject *object)
{
  m_object = object;
}

void CSimpleWaypointController::detach() { m_object = nullptr; }

void CSimpleWaypointController::setActive(bool state) { m_active = state; }

void CSimpleWaypointController::update(float dtime)
{
  glm::vec3 position = m_object->getPosition();
  if (m_active && m_object != nullptr)
  {
    // Reached end
    if (position == m_end)
    {
      // Countdown idle time
      m_idleTimer -= dtime;
    }
    if (m_idleTimer <= 0.f)
    {
      // Idle time over, trigger lose condition
      m_gameState->triggerStateTransition("lose");
    }

    // Normalized direction vector
    glm::vec3 direction = glm::normalize(m_end - position);
    // Current distance to target
    float distance = glm::distance(position, m_end);
    // Next step over distance?
    if (m_speed * dtime > distance)
    {
      // Reach end
      position = m_end;
    }
    else
    {
      // Add step
      position += direction * m_speed * dtime;
    }

    m_object->setPosition(position);
  }
}

void CSimpleWaypointController::receiveMessage(Message msg) {}