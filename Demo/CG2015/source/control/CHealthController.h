#pragma once

#include "kern/game/IGameObjectController.h"

/**
 * \brief Stores health state and marks object for deletion on death.
 * TODO This should actually send an onDeath event to the object.
 */
class CHealthController : public IGameObjectController
{
   public:
    /**
     * \brief Sets health.
     */
    CHealthController(float health);

    /**
     * \brief Destructor for cleanup.
     */
    ~CHealthController();

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
     * \brief Active state.
     */
    void setActive(bool state);

    void receiveMessage(Message msg);

   private:
    GameObject *m_object = nullptr; /**< Controlled game object. */
    float m_health;                  /**< Health value. */
    bool m_active;                   /**< Cative flag. */
};