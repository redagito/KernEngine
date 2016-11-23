#pragma once

#include <glm/glm.hpp>

#include "application/game/IController.h"
#include "application/game/Message.h"

class AGameState;

/**
* \brief Point to point movement.
*/
class CSimpleWaypointController : public IController
{
public:
  CSimpleWaypointController(const glm::vec3 &end, float speed,
                            AGameState *gameState);

  /**
  * \brief Destructor for cleanup.
  */
  ~CSimpleWaypointController();

  /**
  * \brief On attach to object.
  */
  void attach(CGameObject *object);

  /**
  * \brief On dettaching from game object.
  */
  void detach();

  /**
  * \brief Updates controller.
  */
  void update(float dtime);

  /**
  * \brief Sets active state.
  */
  void setActive(bool state);

  /**
  * \brief Racive state from other objects
  */
  void receiveMessage(Message msg);

private:
  glm::vec3 m_end;
  float m_speed = 0.f; /**< Side movement speed. */
  AGameState *m_gameState = nullptr;
  CGameObject *m_object = nullptr; /**< Controlled game object. */
  bool m_active = true;            /**< Active state flag. */
  float m_idleTimer = 5.f;
};
