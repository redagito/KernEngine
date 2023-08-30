#pragma once

#include <glm/glm.hpp>

#include "kern/game/IGameObjectController.h"
#include "kern/game/Message.h"

class AGameState;

/**
 * \brief Point to point movement.
 */
class SimpleWaypointController : public IGameObjectController
{
   public:
    SimpleWaypointController(const glm::vec3 &end, float speed, AGameState *gameState);

    /**
     * \brief Destructor for cleanup.
     */
    ~SimpleWaypointController();

    /**
     * \brief On attach to object.
     */
    void attach(GameObject *object);

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
    GameObject *m_object = nullptr; /**< Controlled game object. */
    bool m_active = true;            /**< Active state flag. */
    float m_idleTimer = 5.f;
};
