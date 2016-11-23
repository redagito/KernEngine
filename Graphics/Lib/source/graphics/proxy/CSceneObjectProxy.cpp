#include "CSceneObjectProxy.h"

const glm::vec3 nullVec(0.f);

CSceneObjectProxy::CSceneObjectProxy()
{
  // Empty
}

CSceneObjectProxy::CSceneObjectProxy(IScene *scene, SceneObjectId id)
{
  init(scene, id);
}

bool CSceneObjectProxy::init(IScene *scene, SceneObjectId id)
{
  if (scene != nullptr && id != invalidObject)
  {
    m_scene = scene;
    m_objectId = id;
    m_init = true;
    return true;
  }
  return false;
}

const glm::vec3 &CSceneObjectProxy::getPosition() const
{
  getUpdate();
  return m_position;
}

const glm::quat &CSceneObjectProxy::getRotation() const
{
  getUpdate();
  return m_rotation;
}

const glm::vec3 &CSceneObjectProxy::getScale() const
{
  getUpdate();
  return m_scale;
}

void CSceneObjectProxy::setPosition(const glm::vec3 &position)
{
  getUpdate();
  m_position = position;
  sendUpdate();
}

void CSceneObjectProxy::setRotation(const glm::vec3 &rotation)
{
  getUpdate();
  m_rotation = glm::quat(rotation);
  sendUpdate();
}

void CSceneObjectProxy::setRotation(const glm::quat &rotation)
{
  getUpdate();
  m_rotation = rotation;
  sendUpdate();
}

void CSceneObjectProxy::setScale(const glm::vec3 &scale)
{
  getUpdate();
  m_scale = scale;
  sendUpdate();
}

void CSceneObjectProxy::setVisibility(bool visible)
{
  getUpdate();
  m_visible = visible;
  sendUpdate();
}

void CSceneObjectProxy::getUpdate() const
{
  if (!m_init)
  {
    return;
  }
  m_scene->getObject(m_objectId, m_mesh, m_material, m_position, m_rotation,
                     m_scale, m_visible);
}

void CSceneObjectProxy::sendUpdate()
{
  if (!m_init)
  {
    return;
  }
  m_scene->setObject(m_objectId, m_mesh, m_material, m_position, m_rotation,
                     m_scale, m_visible);
}