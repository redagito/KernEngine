#include "graphics/game/CGameWorld.h"

#include <cassert>

void CGameWorld::update(float dtime)
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

void CGameWorld::addObject(CGameObject *object)
{
  assert(object != nullptr);
  m_objects.push_back(std::unique_ptr<CGameObject>(object));
}
