#pragma once

#include <glm/glm.hpp>

#include "Animation.h"
#include "IAnimationController.h"
#include "graphics/SceneConfig.h"

class IScene;

class CSineTranslationController : public IAnimationController
{
public:
  CSineTranslationController(SceneObjectId id, AnimationObjectType type,
                             IScene &scene, const glm::vec3 &translation,
                             float timeScale);

  void update(float timeStep);

private:
  SceneObjectId m_objectId = -1;
  AnimationObjectType m_type;
  IScene &m_scene;
  glm::vec3 m_translation;
  float m_timeScale = 1.f;
  float m_time = 0.f;
};