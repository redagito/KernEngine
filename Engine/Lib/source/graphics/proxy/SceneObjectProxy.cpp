#include "kern/graphics/proxy/SceneObjectProxy.h"

const glm::vec3 nullVec(0.f);

SceneObjectProxy::SceneObjectProxy()
{
    // Empty
}

SceneObjectProxy::SceneObjectProxy(IScene *scene, SceneObjectId id) { init(scene, id); }

bool SceneObjectProxy::init(IScene *scene, SceneObjectId id)
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

const glm::vec3 &SceneObjectProxy::getPosition() const
{
    getUpdate();
    return m_position;
}

const glm::quat &SceneObjectProxy::getRotation() const
{
    getUpdate();
    return m_rotation;
}

const glm::vec3 &SceneObjectProxy::getScale() const
{
    getUpdate();
    return m_scale;
}

void SceneObjectProxy::setPosition(const glm::vec3 &position)
{
    getUpdate();
    m_position = position;
    sendUpdate();
}

void SceneObjectProxy::setRotation(const glm::vec3 &rotation)
{
    getUpdate();
    m_rotation = glm::quat(rotation);
    sendUpdate();
}

void SceneObjectProxy::setRotation(const glm::quat &rotation)
{
    getUpdate();
    m_rotation = rotation;
    sendUpdate();
}

void SceneObjectProxy::setScale(const glm::vec3 &scale)
{
    getUpdate();
    m_scale = scale;
    sendUpdate();
}

void SceneObjectProxy::setVisibility(bool visible)
{
    getUpdate();
    m_visible = visible;
    sendUpdate();
}

void SceneObjectProxy::getUpdate() const
{
    if (!m_init)
    {
        return;
    }
    m_scene->getObject(m_objectId, m_mesh, m_material, m_position, m_rotation, m_scale, m_visible);
}

void SceneObjectProxy::sendUpdate()
{
    if (!m_init)
    {
        return;
    }
    m_scene->setObject(m_objectId, m_mesh, m_material, m_position, m_rotation, m_scale, m_visible);
}