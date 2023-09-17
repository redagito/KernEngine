#include "control/WeaponController.h"

#include <kern/game/GameObject.h>

#include <glm/glm.hpp>

#include "control/LinearMovementController.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

WeaponController::WeaponController(IInputProvider *provider, GameWorld *gameWorld, IScene *scene, ResourceId mesh,
                                   ResourceId material, IResourceManager *resourceManager,
                                   CollisionSystem *collisionSystem, int collisionGroup,
                                   std::shared_ptr<SoundEmitter> emitter)
    : m_collisionGroup(collisionGroup),
      m_collisionSystem(collisionSystem),
      m_resourceManager(resourceManager),
      m_inputProvider(provider),
      m_gameWorld(gameWorld),
      m_scene(scene),
      m_mesh(mesh),
      m_material(material),
      m_emitter(emitter)
{
    return;
}

WeaponController::~WeaponController() {}

void WeaponController::attach(GameObject *object) { m_object = object; }

void WeaponController::detach() { m_object = nullptr; }

void WeaponController::update(float dtime)
{
    if (m_active && m_object != nullptr)
    {
        m_weaponCooldown -= dtime;
        // Check for space key
        if (m_inputProvider->isKeyPressed(GLFW_KEY_SPACE) && m_weaponCooldown <= 0.f)
        {
            // Reset cooldown
            m_weaponCooldown = 0.3f;

            glm::vec3 direction = m_object->getForward();
            glm::vec3 position = m_object->getPosition();

            // Create game object
            GameObject *bullet = new GameObject;
            bullet->addController(std::make_shared<LinearMovementController>(direction, 50.f));
            bullet->setPosition(position + direction * 2.f);
            bullet->setRotation(m_object->getRotation());

            // Bullet colidable
            std::vector<float> bulletVertices;
            std::vector<unsigned int> bulletIndices;
            std::vector<float> bulletNormals;
            std::vector<float> bulletUvs;
            PrimitiveType bulletType;
            m_resourceManager->getMesh(m_mesh, bulletVertices, bulletIndices, bulletNormals, bulletUvs, bulletType);
            bullet->setCollidable(m_collisionSystem->add(AABBox::create(bulletVertices), m_collisionGroup));
            bullet->getCollidable()->setDamage(50.f);

            // Create scene proxy
            SceneObjectProxy *proxy =
                new SceneObjectProxy(m_scene, m_scene->createObject(m_mesh, m_material, glm::vec3(0.f),
                                                                    glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(1.f)));
            bullet->setSceneObject(proxy);

            // Add to world
            m_gameWorld->addObject(bullet);
            // Play sound
            m_emitter->play();
        }
    }
}

void WeaponController::setActive(bool state) { m_active = state; }

void WeaponController::receiveMessage(Message msg) {}