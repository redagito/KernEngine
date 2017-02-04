#pragma once

#include "graphics/graphics/IScene.h"
#include "graphics/input/IInputProvider.h"
#include "graphics/resource/IResourceManager.h"

// Collision
#include "graphics/collision/CCollidable.h"
#include "graphics/collision/CCollisionSystem.h"

#include "application/game/CGameWorld.h"
#include "application/game/IController.h"
#include "application/game/Message.h"

/**
 * \brief Controls weapons, creates projectiles.
 */
class CWeaponController : public IController
{
   public:
    CWeaponController(IInputProvider *provider, CGameWorld *gameWorld, IScene *scene,
                      ResourceId mesh, ResourceId material, IResourceManager *resourceManager,
                      CCollisionSystem *collisionSystem, int collisionGroup);
    ~CWeaponController();

    void attach(CGameObject *object);
    void detach();
    void update(float dtime);
    void setActive(bool state);
    void receiveMessage(Message msg);

   private:
	unsigned int m_collisionGroup = 0; /**< Object collision group. */
	CCollisionSystem *m_collisionSystem = nullptr;
	IResourceManager *m_resourceManager = nullptr;
	IInputProvider *m_inputProvider = nullptr;
	CGameWorld *m_gameWorld = nullptr;
	IScene *m_scene = nullptr;
	ResourceId m_mesh = invalidResource;
	ResourceId m_material = invalidResource;

    CGameObject *m_object = nullptr; /**< Controlled game object. */
	bool m_active = true;            /**< Active state flag. */
	// Time between shots
	float m_weaponCooldown = 0.f;
	CAABBox m_bulletAABB;
};