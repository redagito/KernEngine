#include "graphics/game/CGameObject.h"

#include <cassert>

#include <foundation/math/TransformUtils.h>

#include "graphics/collision/CCollidable.h"
#include "graphics/game/IController.h"


// Constructor initializes drawable and matrices
CGameObject::CGameObject() : m_collidable(nullptr) // Collidable
{
  return;
}

CGameObject::~CGameObject() { return; }

void CGameObject::setRotation(const glm::vec3 &rotation)
{
  m_rotation = rotation;
  // Update forward vector
  m_forward = TransformUtils::rotate(glm::vec3(0.f, 0.f, 1.f), m_rotation);
  m_transformationChanged = true;
}

void CGameObject::setPosition(const glm::vec3 &position)
{
  m_position = position;
  m_transformationChanged = true;
}

void CGameObject::setScale(const glm::vec3 &scale)
{
  m_scale = scale;
  m_transformationChanged = true;
}

const glm::vec3 &CGameObject::getRotation() const { return m_rotation; }

void CGameObject::addController(const std::shared_ptr<IController> &controller)
{
  assert(controller != nullptr);
  controller->attach(this);
  m_controllers.push_back(controller);
}

void CGameObject::setSceneObject(CSceneObjectProxy *proxy)
{
  assert(proxy != nullptr);
  m_sceneObject.reset(proxy);
}

const glm::vec3 &CGameObject::getPosition() const { return m_position; }

const glm::vec3 &CGameObject::getScale() const { return m_scale; }

const glm::vec3 &CGameObject::getForward() const { return m_forward; }

void CGameObject::update(float dtime)
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

void CGameObject::markDeleted()
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

bool CGameObject::isDeleteRequested() const { return m_deleteRequested; }

void CGameObject::sendMessage(Message message)
{
  for (auto &controller : m_controllers)
  {
    controller->receiveMessage(message);
  }
}

void CGameObject::setCollidable(CCollidable *entity)
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

CCollidable *CGameObject::getCollidable() const
{
  assert(m_collidable != nullptr);
  return m_collidable;
}

bool CGameObject::hasCollidable() const { return m_collidable != nullptr; }

bool CGameObject::isDead() const { return m_dead; }

void CGameObject::setDead() { m_dead = true; }