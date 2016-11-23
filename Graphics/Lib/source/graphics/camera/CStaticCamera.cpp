#include "graphics/graphics/camera/CStaticCamera.h"

CStaticCamera::CStaticCamera(const glm::mat4 &view, const glm::mat4 &projection,
                             const glm::vec3 &position)
    : m_view(view), m_projection(projection), m_position(position)
{
  return;
}

const glm::mat4 &CStaticCamera::getView() const { return m_view; }

const glm::mat4 &CStaticCamera::getProjection() const { return m_projection; }

const glm::vec3 &CStaticCamera::getPosition() const { return m_position; }
