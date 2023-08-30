#pragma once

#include <glm/glm.hpp>

#include "kern/game/IGameObjectController.h"
#include "kern/game/Message.h"

#include "kern/graphics/input/IInputProvider.h"

/**
 * \brief Restricts object position in space.
 *
 * Restricts x and z coordinates to a plane and reenters an object from the
 * other side.
 */
class CRestrictPositionController : public IGameObjectController
{
   public:
    CRestrictPositionController(const glm::vec2 &minCoords, const glm::vec2 &maxCoords);

    /**
     * \brief Destructor for cleanup.
     */
    ~CRestrictPositionController();

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
    GameObject *m_object = nullptr; /**< Controlled game object. */
    glm::vec2 m_minCoords;
    glm::vec2 m_maxCoords;
    bool m_active = true; /**< Active state flag. */
};