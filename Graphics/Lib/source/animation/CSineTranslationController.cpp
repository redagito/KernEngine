#include "graphics/animation/CSineTranslationController.h"

#include <cmath>

#include "graphics/IScene.h"

CSineTranslationController::CSineTranslationController(SceneObjectId id, AnimationObjectType type,
                                                       IScene &scene, const glm::vec3 &translation,
                                                       float timeScale)
    : m_objectId(id),
      m_type(type),
      m_scene(scene),
      m_translation(translation),
      m_timeScale(timeScale)
{
}

void CSineTranslationController::update(float timeStep)
{
    // Calc sine movement
    m_time += timeStep;
    glm::vec3 translation = m_translation * std::sin(m_time) * m_timeScale;
    if (m_type == AnimationObjectType::Model)
    {
        ResourceId mesh;
        ResourceId material;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        bool visible;

        // Retrieve object
        if (!m_scene.getObject(m_objectId, mesh, material, position, rotation, scale, visible))
        {
            // Invalid id?
            return;
        }
        position += translation;
        m_scene.setObject(m_objectId, mesh, material, position, rotation, scale, visible);
    }
    else if (m_type == AnimationObjectType::PointLight)
    {
        glm::vec3 position;
        float radius;
        glm::vec3 color;
        float intensity;
        bool castsShadow;
        // retrieve object
        if (!m_scene.getPointLight(m_objectId, position, radius, color, intensity, castsShadow))
        {
            // Invalid id
            return;
        }
        position += translation;
        m_scene.setPointLight(m_objectId, position, radius, color, intensity, castsShadow);
    }
    else
    {
        // Error
    }
    return;
}