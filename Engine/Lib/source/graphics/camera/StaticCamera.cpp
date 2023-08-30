#include "kern/graphics/camera/StaticCamera.h"

StaticCamera::StaticCamera(const glm::mat4 &view, const glm::mat4 &projection,
                             const glm::vec3 &position)
    : m_view(view), m_projection(projection), m_position(position)
{
    return;
}

const glm::mat4 &StaticCamera::getView() const { return m_view; }

const glm::mat4 &StaticCamera::getProjection() const { return m_projection; }

const glm::vec3 &StaticCamera::getPosition() const { return m_position; }
