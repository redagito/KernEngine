#pragma once

#include <string>

#include "kern/game/IGameObjectController.h"

class CGamePlayState;

/**
 * \brief Checks the object state for death state and trigger explosion effect.
 * TODO This should actually react to an onDeath event.
 * TODO This actually can play any sound on death.
 */
class CRemoveOnDeathController : public IGameObjectController
{
   public:
    /**
     * \brief Sets sound system and sound name.
     */
    CRemoveOnDeathController(CGamePlayState *state);

    /**
     * \brief Destructor for cleanup.
     */
    ~CRemoveOnDeathController();

    /**
     * \brief On attach to object.
     */
    void attach(GameObject *object);

    /**
     * \brief On detaching from game object.
     */
    void detach();

    /**
     * \brief Updates controller.
     */
    void update(float dtime);

    void setActive(bool active);

    void receiveMessage(Message msg);

   private:
    GameObject *m_object = nullptr; /**< Controlled game object. */
    CGamePlayState *m_gameState = nullptr;
    bool m_active = false;
};
