#pragma once

#include <glm/glm.hpp>

#include "kern/graphics/animation/Animation.h"
#include "kern/graphics/animation/IAnimationController.h"
#include "kern/graphics/SceneConfig.h"

class IScene;

class MovementController : public IAnimationController
{
   public:
    MovementController(SceneObjectId id, AnimationObjectType type, IScene &scene,
                        const glm::vec3 &direction);

    void update(float timeStep);

   private:
    SceneObjectId m_objectId = -1;
    AnimationObjectType m_type;
    IScene &m_scene;

    glm::vec3 m_direction;
    float m_endTime;

    float m_currentTimePoint = 0.0f;
};