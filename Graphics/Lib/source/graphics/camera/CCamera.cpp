#include "graphics/graphics/camera/CCamera.h"

void CCamera::setView(const glm::mat4 &view) { m_view = view; }

const glm::mat4 &CCamera::getView() const { return m_view; }

void CCamera::setProjection(const glm::mat4 &proj) { m_projection = proj; }

const glm::mat4 &CCamera::getProjection() const { return m_projection; }

void CCamera::setPosition(const glm::vec3 &position) { m_position = position; }

const glm::vec3 &CCamera::getPosition() const { return m_position; }