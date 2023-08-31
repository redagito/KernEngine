#include "kern/graphics/collision/CollisionSystem.h"

#include <stdexcept>

#include <fmtlog/fmtlog.h>

#include "kern/graphics/collision/Collidable.h"

unsigned int CollisionSystem::getNewGroupId()
{
    // Add new collision group
    m_entities.push_back(std::list<Collidable *>());
    // Return the group id
    return (unsigned int)m_entities.size() - 1;
}

// Cleanup
CollisionSystem::~CollisionSystem()
{
    for (size_t i = 0; i < m_entities.size(); ++i)
    {
        for (auto iter = m_entities[i].begin(); iter != m_entities[i].end(); ++iter)
        {
            delete (*iter);
        }
    }
}

Collidable *CollisionSystem::add(const AABBox &box, unsigned int groupId)
{
    if (groupId >= m_entities.size())
    {
        loge("Group id {} does not exist", groupId);
        throw std::runtime_error("CollisionSystem.add: Invalid goup id");
    }
    // Create new entity and add to the collision group
    Collidable *entity = new Collidable(groupId, box);
    m_entities[groupId].push_back(entity);
    return entity;
}

// Removes collidable entity
void CollisionSystem::remove(Collidable *entity)
{
    m_entities[entity->getGroupId()].remove(entity);
    delete entity;
}

// Test all entities for collision
void CollisionSystem::update()
{
    for (unsigned int i = 0; i < m_entities.size(); ++i)
    {
        auto iter = m_entities[i].begin();
        while (iter != m_entities[i].end())
        {
            // Delete if requested
            if ((*iter)->deleteRequested())
            {
                Collidable *temp = (*iter);
                iter = m_entities[i].erase(iter);
                delete temp;
            }
            else
            {
                if ((*iter)->isCollidable())
                {
                    testCollision((*iter), i);
                }
                ++iter;
            }
        }
    }
}

// Tests collidable entity against all entities not in the group
void CollisionSystem::testCollision(Collidable *entity, unsigned int group)
{
    // Everything gets tested twice, make this faster
    for (unsigned int i = 0; i < m_entities.size(); ++i)
    {
        if (i != group)
        {
            auto iter = m_entities[i].begin();
            while (iter != m_entities[i].end())
            {
                // Delete if requested
                if ((*iter)->deleteRequested())
                {
                    Collidable *temp = (*iter);
                    iter = m_entities[i].erase(iter);
                    delete temp;
                }
                else
                {
                    if ((*iter)->isCollidable())
                    {
                        if (collides(entity->getAABBox(), (*iter)->getAABBox()))
                        {
                            // Since every collision is tested both ways, damage only happens
                            // from first entity to second entity
                            (*iter)->receiveDamage(entity->getDamage());
                        }
                    }
                    ++iter;
                }
            }
        }
    }
}