#pragma once

#include <list>

#include "graphics/collision/CAABBox.h"

class CCollidable;

class CCollisionSystem
{
   public:
    /**
     * \brief Cleanup of resources.
     */
    ~CCollisionSystem();

    /**
     * \brief Creates a new collidable and adds it to the specified group.
     *
     * A collidable entity has a bounding box and a transformation matrix.
     * Collidables in the same group are not tested for collision.
     * \param The untransformed bounding box for the entity
     */
    CCollidable *add(const CAABBox &box, unsigned int group);

    /**
     * \brief Remove collidable entity
     */
    void remove(CCollidable *entity);

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
    void testCollision(CCollidable *entity, unsigned int groupId);

    std::vector<std::list<CCollidable *>>
        m_entities; /**< Stores entities by group id (vector index) in lists. */
};