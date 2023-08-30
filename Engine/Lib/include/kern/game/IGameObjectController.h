#pragma once

#include "kern/game/Message.h"

class GameObject; /**< Game object forward declare. */

/**
 * \brief Controller class interface.
 * Controllers are attached to game objects and implement specific
 * game logic.
 */
class IGameObjectController
{
   public:
    /**
     * \brief Virtual destructor.
     */
    virtual ~IGameObjectController();

    /**
     * \brief Called on attaching to game object.
     */
    virtual void attach(GameObject *object) = 0;

    /**
     * \brief Called on detaching from game object.
     */
    virtual void detach() = 0;

    /**
     * \brief Updates controller.
     */
    virtual void update(float dtime) = 0;

    /**
     * \brief Sets active state.
     * Can enable/disable controller update.
     */
    virtual void setActive(bool state) = 0;

    /**
     * \brief Receives message from game object.
     */
    virtual void receiveMessage(Message message) = 0;
};