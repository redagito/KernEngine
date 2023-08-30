#pragma once

#include <glm/glm.hpp>

#include "kern/graphics/IScene.h"

class SceneObjectProxy
{
   public:
    SceneObjectProxy();
    SceneObjectProxy(IScene *scene, SceneObjectId id);

    bool init(IScene *scene, SceneObjectId id);

    const glm::vec3 &getPosition() const;
    const glm::quat &getRotation() const;
    const glm::vec3 &getScale() const;

    void setPosition(const glm::vec3 &position);
    void setRotation(const glm::vec3 &rotation);
    void setRotation(const glm::quat &rotation);
    void setScale(const glm::vec3 &scale);

    /**
     * \brief Sets visibility of the object.
     *
     * Set to false to make scene object invisible.
     */
    void setVisibility(bool visible);

   private:
    void getUpdate() const;
    void sendUpdate();

    mutable ResourceId m_mesh = invalidResource;
    mutable ResourceId m_material = invalidResource;
    mutable glm::vec3 m_position;
    mutable glm::quat m_rotation;
    mutable glm::vec3 m_scale;
    mutable bool m_visible = true;

    bool m_init = false;
    IScene *m_scene = nullptr;
    SceneObjectId m_objectId = invalidObject;
};