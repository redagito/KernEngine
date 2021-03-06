#pragma once

#include <glm/glm.hpp>

#include "graphics/animation/Animation.h"
#include "graphics/animation/IAnimationController.h"
#include "graphics/graphics/SceneConfig.h"

class IScene;

class CRotationController : public IAnimationController
{
   public:
    CRotationController(SceneObjectId id, AnimationObjectType type, IScene &scene,
                        const glm::vec3 &rotation);

    void update(float timeStep);

   private:
    SceneObjectId m_objectId = -1;
    AnimationObjectType m_type;
    IScene &m_scene;
    glm::vec3 m_rotation;
};