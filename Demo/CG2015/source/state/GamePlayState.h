#pragma once

#include "kern/graphics/animation/AnimationWorld.h"

#include "kern/graphics/IGraphicsSystem.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/camera/IControllableCamera.h"

#include "kern/graphics/resource/IResourceManager.h"

#include "kern/graphics/input/IInputProvider.h"

#include "kern/graphics/collision/AABBox.h"
#include "kern/graphics/collision/CollisionSystem.h"

#include "kern/game/AGameState.h"
#include "kern/game/GameObject.h"
#include "kern/game/GameWorld.h"

class GamePlayState : public AGameState
{
   public:
    GamePlayState();
    ~GamePlayState();

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
    CollisionSystem *m_collisionSystem = nullptr; /**< Collision system. */

    GameObject *m_player = nullptr;     /**< Player object. */
    GameObject *m_mothership = nullptr; /**< Mothership object. */
    GameObject *m_enemy = nullptr;      /**< Mothership object. */
    GameObject *m_ring = nullptr;       /**< Ring object. */
    GameObject *m_bossEnemy = nullptr;  /**< Mothership object. */

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
