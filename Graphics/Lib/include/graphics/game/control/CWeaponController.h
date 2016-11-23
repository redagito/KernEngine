#pragma once

#include "graphics/game/IController.h"
#include "graphics/game/Message.h"
#include "graphics/resource/IResourceManager.h"
#include "graphics/input/IInputProvider.h"
#include "graphics/game/CGameWorld.h"
#include "graphics/graphics/IScene.h"

// Collision
#include "graphics/collision/CCollisionSystem.h"
#include "graphics/collision/CCollidable.h"

/**
* \brief Controls weapons, creates projectiles.
*/
class CWeaponController : public IController
{
public:
  CWeaponController(IInputProvider *provider, CGameWorld *gameWorld,
                    IScene *scene, ResourceId mesh, ResourceId material,
                    IResourceManager *resourceManager,
                    CCollisionSystem *collisionSystem, int collisionGroup);
  ~CWeaponController();

  void attach(CGameObject *object);
  void detach();
  void update(float dtime);
  void setActive(bool state);
  void receiveMessage(Message msg);

private:
  CGameObject *m_object = nullptr; /**< Controlled game object. */
  bool m_active = true;            /**< Active state flag. */
  IInputProvider *m_inputProvider = nullptr;
  float m_weaponCooldown = 0.f;
  CGameWorld *m_gameWorld = nullptr;
  IResourceManager *m_resourceManager = nullptr;
  IScene *m_scene = nullptr;
  ResourceId m_mesh = invalidResource;
  ResourceId m_material = invalidResource;
  CCollisionSystem *m_collisionSystem = nullptr;

  unsigned int m_collisionGroup; /**< Player collision group. */
};