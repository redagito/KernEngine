#pragma once

#include <glm/glm.hpp>

#include "game/IController.h"
#include "game/Message.h"
#include "input/IInputProvider.h"

/**
* \brief Restricts object position in space.
*
* Restricts x and z coordinates to a plane and reenters an object from the
* other side.
*/
class CRestrictPositionController : public IController
{
public:
  CRestrictPositionController(const glm::vec2 &minCoords,
                              const glm::vec2 &maxCoords);

  /**
  * \brief Destructor for cleanup.
  */
  ~CRestrictPositionController();

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
  CGameObject *m_object = nullptr; /**< Controlled game object. */
  glm::vec2 m_minCoords;
  glm::vec2 m_maxCoords;
  bool m_active = true; /**< Active state flag. */
};