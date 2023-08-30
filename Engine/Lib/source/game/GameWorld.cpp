#include "kern/game/GameWorld.h"

#include <cassert>

void GameWorld::update(float dtime)
{
    for (auto iter = m_objects.begin(); iter != m_objects.end();)
    {
        // Update single game object
        (*iter)->update(dtime);
        if ((*iter)->isDeleteRequested())
        {
            // Delete object
            iter = m_objects.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void GameWorld::addObject(GameObject *object)
{
    assert(object != nullptr);
    m_objects.push_back(std::unique_ptr<GameObject>(object));
}
