#include "gfx/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::setPosition(const glm::vec3& position)
{
    this->position = position;
    viewDirty = true;
}

void Camera::setDirection(const glm::vec3& direction)
{
    this->direction = direction;
    viewDirty = true;
}

void Camera::setDirection(const float yaw, const float pitch)
{
    glm::vec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    setDirection(direction);
}

void Camera::setLookAt(const glm::vec3& point) 
{ 
    setDirection(point - getPosition()); 
}

void Camera::setUp(const glm::vec3& up)
{
    this->up = up;
    viewDirty = true;
}

const glm::vec3& Camera::getPosition() const { return position; }

const glm::vec3& Camera::getDirection() const { return direction; }

const glm::vec3& Camera::getUp() const { return up; }

const glm::vec3 Camera::getRight() const { return glm::normalize(glm::cross(direction, up)); }

const glm::mat4& Camera::getView() const
{
    if (viewDirty)
    {
        viewDirty = false;
        view = glm::lookAt(position, position + direction, up);
    }
    return view;
}