#pragma once

#include <glm/glm.hpp>

#include "kern/game/IGameObjectController.h"
#include "kern/game/Message.h"

/**
 * \brief Controls object to move a linear path.
 */
class CLinearMovementController : public IGameObjectController
{
   public:
    CLinearMovementController(const glm::vec3 &direction, float speed);
    ~CLinearMovementController();

    void attach(GameObject *object);
    void detach();
    void update(float dtime);
    void setActive(bool state);
    void receiveMessage(Message msg);

   private:
    GameObject *m_object = nullptr; /**< Controlled game object. */
    bool m_active = true;            /**< Active state flag. */

    glm::vec3 m_direction = glm::vec3(0.f);
    float m_speed = 0.f;
};