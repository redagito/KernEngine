#pragma once

#include <string>

#include "kern/game/IGameObjectController.h"

class GamePlayState;

/**
 * \brief Checks the object state for death state and trigger explosion effect.
 * TODO This should actually react to an onDeath event.
 * TODO This actually can play any sound on death.
 */
class RemoveOnDeathController : public IGameObjectController
{
   public:
    /**
     * \brief Sets sound system and sound name.
     */
    RemoveOnDeathController(GamePlayState *state);

    /**
     * \brief Destructor for cleanup.
     */
    ~RemoveOnDeathController();

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
    GamePlayState *m_gameState = nullptr;
    bool m_active = false;
};
