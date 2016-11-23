#pragma once

#include <glm/glm.hpp>

#include "graphics/ICamera.h"

/**
* \brief Simple static camera for shadow map pass.
*/
class CStaticCamera : public ICamera
{
public:
  CStaticCamera(const glm::mat4 &view, const glm::mat4 &projection,
                const glm::vec3 &position);

  const glm::mat4 &getView() const;
  const glm::mat4 &getProjection() const;
  const glm::vec3 &getPosition() const;

private:
  glm::mat4 m_view;
  glm::mat4 m_projection;
  glm::vec3 m_position;
};