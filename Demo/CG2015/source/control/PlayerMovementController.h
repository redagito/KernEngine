#pragma once

#include <glm/glm.hpp>

#include "kern/game/IGameObjectController.h"
#include "kern/game/Message.h"
#include "kern/graphics/input/IInputProvider.h"

/**
 * \brief Player side movement.
 */
class PlayerMovementController : public IGameObjectController
{
   public:
    PlayerMovementController(IInputProvider *inputProvider, float speed);

    /**
     * \brief Destructor for cleanup.
     */
    ~PlayerMovementController();

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
    IInputProvider *m_inputProvider = nullptr;
    GameObject *m_object = nullptr; /**< Controlled game object. */
    float m_speed = 0.f;             /**< Side movement speed. */
    float m_rotationDegree = 0.f;    /**< Current rotation in degree. */
    bool m_active = true;            /**< Active state flag. */
};