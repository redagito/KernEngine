#pragma once

#include <glm/glm.hpp>

#include "kern/graphics/animation/AnimationObjectType.h"
#include "kern/graphics/animation/IAnimationController.h"
#include "kern/graphics/SceneConfig.h"

class IScene;

// Rotates the controlled object
class RotationController : public IAnimationController
{
   public:
    RotationController(SceneObjectId id, AnimationObjectType type, IScene &scene,
                        const glm::vec3 &rotation);

    void update(float timeStep);

   private:
    SceneObjectId m_objectId = -1;
    AnimationObjectType m_type;
    IScene &m_scene;
    glm::vec3 m_rotation;
};