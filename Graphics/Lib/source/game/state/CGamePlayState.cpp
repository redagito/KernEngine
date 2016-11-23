#include "graphics/game/state/CGamePlayState.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "graphics/graphics/IGraphicsSystem.h"
#include "graphics/graphics/IScene.h"

// GameObject
#include "graphics/game/CGameObject.h"

// Controller
#include "graphics/game/control/CPlayerMovementController.h"
#include "graphics/game/control/CCameraController.h"
#include "graphics/game/control/CRestrictPositionController.h"
#include "graphics/game/control/CWeaponController.h"
#include "graphics/game/control/CSimpleWaypointController.h"
#include "graphics/game/control/CLinearMovementController.h"
#include "graphics/game/control/CHealthController.h"
#include "graphics/game/control/CRemoveOnDeathController.h"

#include "graphics/graphics/camera/CFirstPersonCamera.h"

#include "graphics/io/CSceneLoader.h"
#include "graphics/animation/CAnimationWorld.h"

// Collision
#include "graphics/collision/CCollisionSystem.h"
#include "graphics/collision/CCollidable.h"

const std::string exitStr = "lose";
const std::string exitStrW = "win";

CGamePlayState::CGamePlayState()
    : m_enemyCount(6), m_enemyTime(4.f), m_enemyXPosition(-75.f)
{
}

CGamePlayState::~CGamePlayState()
{
  // Empty
}

bool CGamePlayState::init(IGraphicsSystem *graphicsSystem,
                          IInputProvider *inputProvider,
                          IResourceManager *resourceManager)
{
  m_collisionSystem = new CCollisionSystem();
  m_graphicsSystem = graphicsSystem;
  m_inputProvider = inputProvider;
  m_resourceManager = resourceManager;
  m_scene = m_graphicsSystem->createScene();

  // Collision group ids for static entities, enemy objects and the player model
  m_playerGroup = m_collisionSystem->getNewGroupId();
  m_enemyGroup = m_collisionSystem->getNewGroupId();

  CAnimationWorld animWorld;
  CSceneLoader loader(*resourceManager);
  loader.load("data/world/game_1.json", *m_scene, animWorld);

  m_camera = std::make_shared<CFirstPersonCamera>(
      glm::vec3(90.f, 70.f, 90.f), glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(0.f, 1.f, 0.f), 45.f, 4.f / 3.f, 0.01f, 1000.f);

  // Disable god ray
  m_camera->getFeatureInfoForWrite().godRayActive = false;

  // Create player
  m_player = new CGameObject();

  m_player->addController(
      std::make_shared<CPlayerMovementController>(inputProvider, 20.f));
  m_player->addController(std::make_shared<CCameraController>(m_camera.get()));
  m_player->addController(std::make_shared<CRestrictPositionController>(
      glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));

  // Load bullet
  ResourceId bulletMesh = m_resourceManager->loadMesh("data/mesh/bullet.obj");
  if (bulletMesh == invalidResource)
  {
    return false;
  }
  ResourceId bulletMaterial =
      m_resourceManager->loadMaterial("data/material/white.json");
  if (bulletMaterial == invalidResource)
  {
    return false;
  }

  m_player->addController(std::make_shared<CWeaponController>(
      m_inputProvider, &getGameWorld(), m_scene, bulletMesh, bulletMaterial,
      m_resourceManager, m_collisionSystem, m_playerGroup));
  m_player->setPosition(glm::vec3(0.f, 20.f, -100.f));
  m_player->setRotation(glm::vec3(0.f));
  m_player->setScale(glm::vec3(0.5f));

  // Get model resources
  ResourceId playerShip = m_resourceManager->loadMesh("data/mesh/ship_2.obj");
  if (playerShip == invalidResource)
  {
    return false;
  }

  // Player collidable added to player collision group
  std::vector<float> playerVertices;
  std::vector<unsigned int> playerIndices;
  std::vector<float> playerNormals;
  std::vector<float> playerUvs;
  EPrimitiveType playerType;
  m_resourceManager->getMesh(playerShip, playerVertices, playerIndices,
                             playerNormals, playerUvs, playerType);
  m_player->setCollidable(
      m_collisionSystem->add(CAABBox::create(playerVertices), m_playerGroup));
  m_player->getCollidable()->setDamage(50.f);

  ResourceId playerShipMaterial =
      m_resourceManager->loadMaterial("data/material/line_metal.json");
  if (playerShipMaterial == invalidResource)
  {
    return false;
  }
  // Create scene object
  CSceneObjectProxy *playerSceneObject = new CSceneObjectProxy(
      m_scene,
      m_scene->createObject(playerShip, playerShipMaterial, glm::vec3(0.f),
                            glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(1.f)));
  m_player->setSceneObject(playerSceneObject);

  // Add player
  getGameWorld().addObject(m_player);

  // Create mothership
  m_mothership = new CGameObject();
  m_mothership->setPosition(glm::vec3(0.f, 68.f, 700.f));
  m_mothership->setRotation(glm::vec3(0.f));
  m_mothership->setScale(glm::vec3(60.f));
  m_mothership->addController(std::make_shared<CSimpleWaypointController>(
      glm::vec3(0.f, 68.f, 0.f), 5.f, this));

  // Get model resources
  ResourceId motherShip =
      m_resourceManager->loadMesh("data/mesh/mothership.obj");
  if (motherShip == invalidResource)
  {
    return false;
  }
  ResourceId motherShipMaterial =
      m_resourceManager->loadMaterial("data/material/mothership.json");
  if (motherShipMaterial == invalidResource)
  {
    return false;
  }
  // Create scene object
  CSceneObjectProxy *motherSceneObject = new CSceneObjectProxy(
      m_scene, m_scene->createObject(
                   motherShip, motherShipMaterial, glm::vec3(0.f, 68.f, 700.f),
                   glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(60.f)));
  m_mothership->setSceneObject(motherSceneObject);

  // Add player
  getGameWorld().addObject(m_mothership);

  // Create pyramide
  CGameObject *m_pyramide = new CGameObject();
  m_pyramide->setPosition(glm::vec3(0.f, 2.f, 0.f));
  m_pyramide->setRotation(glm::vec3(0.f, -90.f, 0.f));
  m_pyramide->setScale(glm::vec3(3.f));

  // Get model resources
  ResourceId pyramide = m_resourceManager->loadMesh("data/mesh/piramyde.obj");
  if (pyramide == invalidResource)
  {
    return false;
  }

  ResourceId pyramideMaterial =
      m_resourceManager->loadMaterial("data/material/sand.json");
  if (pyramideMaterial == invalidResource)
  {
    return false;
  }
  // Create scene object
  // glm::vec3(0.f, -90.f, 0.f)
  CSceneObjectProxy *pyramideSceneObject = new CSceneObjectProxy(
      m_scene, m_scene->createObject(
                   pyramide, pyramideMaterial, glm::vec3(0.f, 2.f, 0.f),
                   glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(3.f)));
  m_pyramide->setSceneObject(pyramideSceneObject);

  // Add player
  getGameWorld().addObject(m_pyramide);

  // LOAD ENEMY RESOURCE
  // Get model resources
  enemyShip = m_resourceManager->loadMesh("data/mesh/enemy.obj");
  if (enemyShip == invalidResource)
  {
    return false;
  }

  enemyShipMaterial =
      m_resourceManager->loadMaterial("data/material/enemy.json");
  if (enemyShipMaterial == invalidResource)
  {
    return false;
  }

  // Load boss Enemy resources
  // Add enemy with rotating ring
  m_bossEnemy = new CGameObject();
  m_bossEnemy->setPosition(glm::vec3(0.f, 20.f, -50.f));
  m_bossEnemy->setRotation(glm::vec3(0.f));
  m_bossEnemy->setScale(glm::vec3(0.6f));
  m_bossEnemy->addController(std::make_shared<CRestrictPositionController>(
      glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
  m_bossEnemy->addController(std::make_shared<CLinearMovementController>(
      m_bossEnemy->getForward(), 17.f));
  m_bossEnemy->addController(std::make_shared<CHealthController>(300.f));
  m_bossEnemy->addController(std::make_shared<CRemoveOnDeathController>(this));

  bossShip = m_resourceManager->loadMesh("data/mesh/ship_2.obj");
  if (bossShip == invalidResource)
  {
    return false;
  }

  bossShipMaterial =
      m_resourceManager->loadMaterial("data/material/metallic_galvanized.json");
  if (enemyShipMaterial == invalidResource)
  {
    return false;
  }

  // Create scene object
  CSceneObjectProxy *bossSceneObject = new CSceneObjectProxy(
      m_scene, m_scene->createObject(
                   bossShip, bossShipMaterial, glm::vec3(0.f, 20.f, -50.f),
                   glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(0.5f)));
  m_bossEnemy->setSceneObject(bossSceneObject);

  getGameWorld().addObject(m_bossEnemy);

  m_ring = new CGameObject();
  m_ring->setPosition(glm::vec3(0.f, 20.f, -50.f));
  m_ring->setRotation(glm::vec3(0.f));
  m_ring->setScale(glm::vec3(0.5f));
  m_ring->addController(std::make_shared<CRestrictPositionController>(
      glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
  m_ring->addController(
      std::make_shared<CLinearMovementController>(m_ring->getForward(), 17.f));

  bossRing = m_resourceManager->loadMesh("data/mesh/ring_animation.obj");
  if (bossRing == invalidResource)
  {
    return false;
  }

  bossRingMaterial =
      m_resourceManager->loadMaterial("data/material/metallic_galvanized.json");
  if (bossRingMaterial == invalidResource)
  {
    return false;
  }

  // Create scene object
  CSceneObjectProxy *bossRingObject = new CSceneObjectProxy(
      m_scene, m_scene->createObject(
                   bossRing, bossRingMaterial, glm::vec3(0.f, 20.f, -50.f),
                   glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(0.5f)));
  m_ring->setSceneObject(bossRingObject);

  getGameWorld().addObject(m_ring);

  return true;
}

void CGamePlayState::onEnter()
{
  m_graphicsSystem->setActiveCamera(m_camera.get());
  m_graphicsSystem->setActiveScene(m_scene);
}

bool CGamePlayState::update(float dtime)
{

  m_enemyTime -= dtime;
  if (m_enemyCount > 0.f && m_enemyTime <= 0.f)
  {
    m_enemyTime = 2.f;
    m_enemyId++;
    // Create new enemy
    CGameObject *enemy = new CGameObject();
    if (m_enemyId == 1)
    {
      enemy->setPosition(glm::vec3(-50.f, 30.f, 50.f));
      enemy->setRotation(glm::vec3(0.f));
      enemy->setScale(glm::vec3(0.5f));
      enemy->addController(std::make_shared<CRestrictPositionController>(
          glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
      enemy->addController(std::make_shared<CLinearMovementController>(
          enemy->getForward(), 15.f));
      enemy->addController(std::make_shared<CHealthController>(100.f));
      enemy->addController(std::make_shared<CRemoveOnDeathController>(this));
    }

    if (m_enemyId == 2)
    {
      enemy->setPosition(glm::vec3(0.f, 45.f, -50.f));
      enemy->setRotation(glm::vec3(0.f));
      enemy->setScale(glm::vec3(0.5f));
      enemy->addController(std::make_shared<CRestrictPositionController>(
          glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
      enemy->addController(std::make_shared<CLinearMovementController>(
          enemy->getForward(), 15.f));
      enemy->addController(std::make_shared<CHealthController>(100.f));
      enemy->addController(std::make_shared<CRemoveOnDeathController>(this));
    }

    if (m_enemyId == 3)
    {
      enemy->setPosition(glm::vec3(50.f, 20.f, 0.f));
      enemy->setRotation(glm::vec3(0.f));
      enemy->setScale(glm::vec3(0.5f));
      enemy->addController(std::make_shared<CRestrictPositionController>(
          glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
      enemy->addController(std::make_shared<CLinearMovementController>(
          enemy->getForward(), 15.f));
      enemy->addController(std::make_shared<CHealthController>(100.f));
      enemy->addController(std::make_shared<CRemoveOnDeathController>(this));
    }

    if (m_enemyId == 4)
    {
      enemy->setPosition(glm::vec3(50.f, 25.f, 0.f));
      enemy->setRotation(glm::vec3(0.f, 92.6, 0.f));
      enemy->setScale(glm::vec3(0.5f));
      enemy->addController(std::make_shared<CRestrictPositionController>(
          glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
      enemy->addController(std::make_shared<CSimpleWaypointController>(
          glm::vec3(-101.f, 25.f, 0.f), 15.f, this));
      enemy->addController(std::make_shared<CHealthController>(100.f));
      enemy->addController(std::make_shared<CRemoveOnDeathController>(this));
    }

    if (m_enemyId == 5)
    {
      enemy->setPosition(glm::vec3(0.f, 40.f, -50.f));
      enemy->setRotation(glm::vec3(0.f, 92.6, 0.f));
      enemy->setScale(glm::vec3(0.5f));
      enemy->addController(std::make_shared<CRestrictPositionController>(
          glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
      enemy->addController(std::make_shared<CSimpleWaypointController>(
          glm::vec3(-101.f, 40.f, -50.f), 15.f, this));
      enemy->addController(std::make_shared<CHealthController>(100.f));
      enemy->addController(std::make_shared<CRemoveOnDeathController>(this));
    }

    if (m_enemyId == 6)
    {
      enemy->setPosition(glm::vec3(-50.f, 50.f, 50.f));
      enemy->setRotation(glm::vec3(0.f, 92.6, 0.f));
      enemy->setScale(glm::vec3(0.5f));
      enemy->addController(std::make_shared<CRestrictPositionController>(
          glm::vec2(-100.f, -100.f), glm::vec2(100.f, 100.f)));
      enemy->addController(std::make_shared<CSimpleWaypointController>(
          glm::vec3(-101.f, 50.f, 50.f), 15.f, this));
      enemy->addController(std::make_shared<CHealthController>(100.f));
      enemy->addController(std::make_shared<CRemoveOnDeathController>(this));
    }

    // Player collidable added to player collision group
    std::vector<float> enemyVertices;
    std::vector<unsigned int> enemyIndices;
    std::vector<float> enemyNormals;
    std::vector<float> enemyUvs;
    EPrimitiveType enemyType;
    m_resourceManager->getMesh(enemyShip, enemyVertices, enemyIndices,
                               enemyNormals, enemyUvs, enemyType);
    enemy->setCollidable(
        m_collisionSystem->add(CAABBox::create(enemyVertices), m_enemyGroup));

    // Create scene object
    CSceneObjectProxy *enemySceneObject = new CSceneObjectProxy(
        m_scene,
        m_scene->createObject(enemyShip, enemyShipMaterial,
                              glm::vec3(m_enemyXPosition, 25.f, 0.f),
                              glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(0.5f)));
    enemy->setSceneObject(enemySceneObject);

    // Add enemy
    getGameWorld().addObject(enemy);

    m_enemyCount -= 1;
    m_enemyXPosition += 25.f;
  }

  m_ring->setRotation(glm::vec3(m_ring->getRotation().x,
                                m_ring->getRotation().y,
                                m_ring->getRotation().z + dtime));

  if (m_inputProvider->isKeyPressed(GLFW_KEY_P))
  {
    return false;
  }
  if (m_inputProvider->isKeyPressed(GLFW_KEY_O))
  {
    m_winCounter = 6;
    return false;
  }
  if (m_winCounter == 6)
  {
    return false;
  }

  if (m_inputProvider->isKeyPressed(GLFW_KEY_K))
  {
    m_camera->getFeatureInfoForWrite().toonActive = true;
  }
  if (m_inputProvider->isKeyPressed(GLFW_KEY_L))
  {
    m_camera->getFeatureInfoForWrite().toonActive = false;
  }
  // Update collision system
  m_collisionSystem->update();
  // Update gameworld
  return AGameState::update(dtime);
}

void CGamePlayState::onExit()
{
  // Cleanup?
}

const std::string &CGamePlayState::getNextState() const
{
  if (m_winCounter == 6)
  {
    return exitStrW;
  }
  else
  {
    return exitStr;
  }
}

void CGamePlayState::winCount() { m_winCounter++; }
