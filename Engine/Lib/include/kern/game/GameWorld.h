#pragma once

#include <list>
#include <memory>

#include "kern/game/GameObject.h"

/**
 * \brief Stores game objects and manages object lifetime.
 */
class GameWorld
{
   public:
    /**
     * \brief Updates all game objects.
     */
    void update(float dtime);

    /**
     * \brief Adds game object.
     *
     * The object must be created on the heap and is owned by the game world.
     */
    void addObject(GameObject *object);

   private:
    std::list<std::unique_ptr<GameObject>> m_objects; /**< Game objects. */
};
