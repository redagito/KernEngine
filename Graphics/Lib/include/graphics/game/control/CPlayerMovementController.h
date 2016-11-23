#pragma once

#include <glm/glm.hpp>

#include "graphics/game/IController.h"
#include "graphics/game/Message.h"
#include "graphics/input/IInputProvider.h"

/**
* \brief Player side movement.
*/
class CPlayerMovementController : public IController
{
public:
  CPlayerMovementController(IInputProvider *inputProvider, float speed);

  /**
  * \brief Destructor for cleanup.
  */
  ~CPlayerMovementController();

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
  IInputProvider *m_inputProvider = nullptr;
  CGameObject *m_object = nullptr; /**< Controlled game object. */
  float m_speed = 0.f;             /**< Side movement speed. */
  float m_rotationDegree = 0.f;    /**< Current rotation in degree. */
  bool m_active = true;            /**< Active state flag. */
};