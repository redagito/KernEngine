#include "graphics/animation/CRotationController.h"

#include "graphics/graphics/IScene.h"

CRotationController::CRotationController(SceneObjectId id,
                                         AnimationObjectType type,
                                         IScene &scene,
                                         const glm::vec3 &rotation)
    : m_objectId(id), m_type(type), m_scene(scene), m_rotation(rotation)
{
}

void CRotationController::update(float timeStep)
{
  if (m_type == AnimationObjectType::Model)
  {
    ResourceId mesh;
    ResourceId material;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    bool visible;

    // Retrieve object
    if (!m_scene.getObject(m_objectId, mesh, material, position, rotation,
                           scale, visible))
    {
      // Invalid id?
      return;
    }
    rotation = rotation * glm::quat(m_rotation * timeStep);
    m_scene.setObject(m_objectId, mesh, material, position, rotation, scale,
                      visible);
  }
  else if (m_type == AnimationObjectType::PointLight)
  {
    // Rotation point lights makes no sense
  }
  else
  {
    // Error
  }
  return;
}