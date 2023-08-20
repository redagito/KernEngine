#pragma once

#include "graphics/animation/CAnimationWorld.h"

#include "graphics/IGraphicsSystem.h"
#include "graphics/IScene.h"
#include "graphics/camera/IControllableCamera.h"

#include "graphics/resource/IResourceManager.h"

#include "graphics/input/IInputProvider.h"

#include "graphics/collision/CAABBox.h"
#include "graphics/collision/CCollisionSystem.h"

#include "application/game/AGameState.h"
#include "application/game/CGameObject.h"
#include "application/game/CGameWorld.h"

class CGamePlayState : public AGameState
{
   public:
    CGamePlayState();
    ~CGamePlayState();

    bool init(IGraphicsSystem *graphicsSystem, IInputProvider *inputProvider,
              IResourceManager *resourceManager);
    void onEnter();
    bool update(float dtime);
    void onExit();
    const std::string &getNextState() const;
    void winCount();

   private:
    IGraphicsSystem *m_graphicsSystem = nullptr;
    IScene *m_scene = nullptr;
    IResourceManager *m_resourceManager = nullptr;
    IInputProvider *m_inputProvider = nullptr;
    std::shared_ptr<IControllableCamera> m_camera = nullptr;
    CCollisionSystem *m_collisionSystem = nullptr; /**< Collision system. */

    CGameObject *m_player = nullptr;     /**< Player object. */
    CGameObject *m_mothership = nullptr; /**< Mothership object. */
    CGameObject *m_enemy = nullptr;      /**< Mothership object. */
    CGameObject *m_ring = nullptr;       /**< Ring object. */
    CGameObject *m_bossEnemy = nullptr;  /**< Mothership object. */

    int m_enemyCount = 0; /**< Enemy spawn counter. */
    int m_enemyId = 0;
    float m_enemyTime = 0.0;        /**< Enemy spawn time. */
    float m_enemyXPosition = 0.0;   // Hack
    unsigned int m_playerGroup = 0; /**< Player collision group. */
    unsigned int m_enemyGroup = 0;  /**< Enemy collision group. */

    ResourceId enemyShip = invalidResource;
    ResourceId enemyShipMaterial = invalidResource;
    ResourceId bossShip = invalidResource;
    ResourceId bossShipMaterial = invalidResource;
    ResourceId bossRing = invalidResource;
    ResourceId bossRingMaterial = invalidResource;

    int m_winCounter = 0;
};
