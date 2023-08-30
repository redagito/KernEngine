#include "kern/graphics/camera/Camera.h"

void Camera::setView(const glm::mat4 &view) { m_view = view; }

const glm::mat4 &Camera::getView() const { return m_view; }

void Camera::setProjection(const glm::mat4 &proj) { m_projection = proj; }

const glm::mat4 &Camera::getProjection() const { return m_projection; }

void Camera::setPosition(const glm::vec3 &position) { m_position = position; }

const glm::vec3 &Camera::getPosition() const { return m_position; }