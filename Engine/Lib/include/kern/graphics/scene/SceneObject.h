#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "kern/graphics/collision/BoundingSphere.h"
#include "kern/resource/ResourceId.h"

/**
 * \brief Scene object data struct.
 */
struct SceneObject
{
    SceneObject();
    SceneObject(ResourceId mesh, ResourceId material, const glm::vec3 &position,
                 const glm::quat &rotation, const glm::vec3 &scale, bool visible,
                 const BoundingSphere &sphere);

    SceneObject(ResourceId model, const glm::vec3 &position, const glm::quat &rotation,
                 const glm::vec3 &scale, bool visible, const BoundingSphere &sphere);

    void updateBoundingSphere();

    ResourceId m_mesh = InvalidResource;
    ResourceId m_material = InvalidResource;
    ResourceId m_model = InvalidResource;
    glm::vec3 m_position = glm::vec3(0.f);
    glm::quat m_rotation = glm::quat(0.f, 0.f, 0.f, 0.f);
    glm::vec3 m_scale = glm::vec3(1.f);
    bool m_visible = true;
    BoundingSphere boundingSphere; /**< Translated and scaled bounding sphere. */
};