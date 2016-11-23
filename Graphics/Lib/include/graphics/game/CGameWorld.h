#pragma once

#include <list>
#include <memory>

#include "graphics/game/CGameObject.h"

/**
* \brief Stores game objects and manages object lifetime.
*/
class CGameWorld
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
  void addObject(CGameObject *object);

private:
  std::list<std::unique_ptr<CGameObject>> m_objects; /**< Game objects. */
};
