#pragma once

#include <glm/glm.hpp>

#include "graphics/game/IController.h"
#include "graphics/game/Message.h"

/**
* \brief Controls object to move a linear path.
*/
class CLinearMovementController : public IController
{
public:
  CLinearMovementController(const glm::vec3 &direction, float speed);
  ~CLinearMovementController();

  void attach(CGameObject *object);
  void detach();
  void update(float dtime);
  void setActive(bool state);
  void receiveMessage(Message msg);

private:
  CGameObject *m_object = nullptr; /**< Controlled game object. */
  bool m_active = true;            /**< Active state flag. */

  glm::vec3 m_direction = glm::vec3(0.f);
  float m_speed = 0.f;
};