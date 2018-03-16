#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "graphics/collision/CBoundingSphere.h"
#include "graphics/resource/ResourceConfig.h"

/**
 * \brief Scene object data struct.
 */
struct SSceneObject
{
    SSceneObject();
    SSceneObject(ResourceId mesh, ResourceId material, const glm::vec3 &position,
                 const glm::quat &rotation, const glm::vec3 &scale, bool visible,
                 const CBoundingSphere &sphere);

    SSceneObject(ResourceId model, const glm::vec3 &position, const glm::quat &rotation,
                 const glm::vec3 &scale, bool visible, const CBoundingSphere &sphere);

    void updateBoundingSphere();

    ResourceId m_mesh = invalidResource;
    ResourceId m_material = invalidResource;
    ResourceId m_model = invalidResource;
    glm::vec3 m_position = glm::vec3(0.f);
    glm::quat m_rotation = glm::quat(0.f, 0.f, 0.f, 0.f);
    glm::vec3 m_scale = glm::vec3(1.f);
    bool m_visible = true;
    CBoundingSphere boundingSphere; /**< Translated and scaled bounding sphere. */
};