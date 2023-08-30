#pragma once

#include <list>

#include "kern/graphics/collision/AABBox.h"

class Collidable;

class CollisionSystem
{
   public:
    /**
     * \brief Cleanup of resources.
     */
    ~CollisionSystem();

    /**
     * \brief Creates a new collidable and adds it to the specified group.
     *
     * A collidable entity has a bounding box and a transformation matrix.
     * Collidables in the same group are not tested for collision.
     * \param The untransformed bounding box for the entity
     */
    Collidable *add(const AABBox &box, unsigned int group);

    /**
     * \brief Remove collidable entity
     */
    void remove(Collidable *entity);

    /**
     * \brief Tests the entities for collision
     */
    void update();

    /**
     * \brief Creates and returns new collision group id
     */
    unsigned int getNewGroupId();

   private:
    /**
     * \brief Tests the entity for collision with all other entities not in the
     * group
     */
    void testCollision(Collidable *entity, unsigned int groupId);

    std::vector<std::list<Collidable *>>
        m_entities; /**< Stores entities by group id (vector index) in lists. */
};