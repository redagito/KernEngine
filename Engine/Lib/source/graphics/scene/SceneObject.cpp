#include "kern/graphics/scene/SceneObject.h"

#include <algorithm>

SceneObject::SceneObject() { return; }

SceneObject::SceneObject(ResourceId mesh, ResourceId material, const glm::vec3 &position,
                           const glm::quat &rotation, const glm::vec3 &scale, bool visible,
                           const BoundingSphere &sphere)
    : m_mesh(mesh),
      m_material(material),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale),
      m_visible(visible),
      boundingSphere(sphere)
{
    updateBoundingSphere();
    return;
}

SceneObject::SceneObject(ResourceId model, const glm::vec3 &position, const glm::quat &rotation,
                           const glm::vec3 &scale, bool visible, const BoundingSphere &sphere)
    : m_model(model),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale),
      m_visible(visible),
      boundingSphere(sphere)
{
    updateBoundingSphere();
    return;
}

void SceneObject::updateBoundingSphere()
{
    // Translate
    boundingSphere.setPosition(m_position);
    // Scale sphere by highest scaling factor
    boundingSphere.setRadius(boundingSphere.getRadius() *
                             std::max(std::max(m_scale.x, m_scale.y), m_scale.z));
}