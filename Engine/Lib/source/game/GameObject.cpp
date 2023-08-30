#include "kern/game/GameObject.h"

#include "kern/graphics/collision/Collidable.h"

#include <cassert>

#include "kern/foundation/TransformUtils.h"
#include "kern/game/IGameObjectController.h"

// Constructor initializes drawable and matrices
GameObject::GameObject() : m_collidable(nullptr)  // Collidable
{
    return;
}

GameObject::~GameObject() { return; }

void GameObject::setRotation(const glm::vec3 &rotation)
{
    m_rotation = rotation;
    // Update forward vector
    m_forward = TransformUtils::rotate(glm::vec3(0.f, 0.f, 1.f), m_rotation);
    m_transformationChanged = true;
}

void GameObject::setPosition(const glm::vec3 &position)
{
    m_position = position;
    m_transformationChanged = true;
}

void GameObject::setScale(const glm::vec3 &scale)
{
    m_scale = scale;
    m_transformationChanged = true;
}

const glm::vec3 &GameObject::getRotation() const { return m_rotation; }

void GameObject::addController(const std::shared_ptr<IGameObjectController> &controller)
{
    assert(controller != nullptr);
    controller->attach(this);
    m_controllers.push_back(controller);
}

void GameObject::setSceneObject(SceneObjectProxy *proxy)
{
    assert(proxy != nullptr);
    m_sceneObject.reset(proxy);
}

const glm::vec3 &GameObject::getPosition() const { return m_position; }

const glm::vec3 &GameObject::getScale() const { return m_scale; }

const glm::vec3 &GameObject::getForward() const { return m_forward; }

void GameObject::update(float dtime)
{
    // Update object controllers
    for (auto &controller : m_controllers)
    {
        controller->update(dtime);
    }

    // Update attached objects if tranformation of the game object has changed
    if (m_transformationChanged)
    {
        // Update scene object
        if (m_sceneObject != nullptr)
        {
            // TODO Slow?
            m_sceneObject->setPosition(getPosition());
            m_sceneObject->setRotation(getRotation());
            m_sceneObject->setScale(getScale());
        }
        // Update collidable
        if (hasCollidable())
        {
            m_collidable->setTranslation(getPosition());
            m_collidable->setScale(getScale());
        }
    }
}

void GameObject::markDeleted()
{
    m_deleteRequested = true;
    if (m_sceneObject != nullptr)
    {
        m_sceneObject->setVisibility(false);
    }
    if (hasCollidable())
    {
        m_collidable->markDeleted();
    }
}

bool GameObject::isDeleteRequested() const { return m_deleteRequested; }

void GameObject::sendMessage(Message message)
{
    for (auto &controller : m_controllers)
    {
        controller->receiveMessage(message);
    }
}

void GameObject::setCollidable(Collidable *entity)
{
    assert(entity != nullptr);
    if (m_collidable != entity)
    {
        m_collidable = entity;
        // Update
        m_collidable->setScale(getScale());
        m_collidable->setTranslation(getPosition());
    }
}

Collidable *GameObject::getCollidable() const
{
    assert(m_collidable != nullptr);
    return m_collidable;
}

bool GameObject::hasCollidable() const { return m_collidable != nullptr; }

bool GameObject::isDead() const { return m_dead; }

void GameObject::setDead() { m_dead = true; }