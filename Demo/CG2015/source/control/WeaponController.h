#pragma once

#include "kern/graphics/IScene.h"
#include "kern/graphics/input/IInputProvider.h"
#include "kern/resource/IResourceManager.h"

// Collision
#include "kern/graphics/collision/Collidable.h"
#include "kern/graphics/collision/CollisionSystem.h"

#include "kern/game/GameWorld.h"
#include "kern/game/IGameObjectController.h"
#include "kern/game/Message.h"

/**
 * \brief Controls weapons, creates projectiles.
 */
class WeaponController : public IGameObjectController
{
   public:
    WeaponController(IInputProvider *provider, GameWorld *gameWorld, IScene *scene,
                      ResourceId mesh, ResourceId material, IResourceManager *resourceManager,
                      CollisionSystem *collisionSystem, int collisionGroup);
    ~WeaponController();

    void attach(GameObject *object);
    void detach();
    void update(float dtime);
    void setActive(bool state);
    void receiveMessage(Message msg);

   private:
    GameObject *m_object = nullptr; /**< Controlled game object. */
    bool m_active = true;            /**< Active state flag. */
    IInputProvider *m_inputProvider = nullptr;
    float m_weaponCooldown = 0.f;
    GameWorld *m_gameWorld = nullptr;
    IResourceManager *m_resourceManager = nullptr;
    IScene *m_scene = nullptr;
    ResourceId m_mesh = InvalidResource;
    ResourceId m_material = InvalidResource;
    CollisionSystem *m_collisionSystem = nullptr;

    unsigned int m_collisionGroup; /**< Player collision group. */
};