#include "kern/graphics/camera/FreeCamera.h"

#include <glm/ext.hpp>

#include "kern/foundation/TransformUtils.h"

FreeCamera::FreeCamera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up,
                         float fieldOfView, float aspectRatio, float zNear, float zFar)
{
    lookAt(position, target, up);
    setProjection(fieldOfView, aspectRatio, zNear, zFar);
}

const glm::vec3 &FreeCamera::getPosition() const { return m_position; }

void FreeCamera::setPosition(const glm::vec3 &position)
{
    m_position = position;
    updateView();
}

void FreeCamera::move(const glm::vec3 &direction)
{
    m_position += direction;
    updateView();
}

void FreeCamera::moveForward(float amount)
{
    m_position += m_forward * amount;
    updateView();
}

void FreeCamera::moveRight(float amount)
{
    m_position += m_right * amount;
    updateView();
}

void FreeCamera::moveUp(float amount)
{
    m_position += m_up * amount;
    updateView();
}

void FreeCamera::rotate(const glm::vec3 &axis, float amount)
{
    m_up = TransformUtils::rotate(axis, amount, m_up);
    m_right = TransformUtils::rotate(axis, amount, m_right);
    m_forward = TransformUtils::rotate(axis, amount, m_forward);
    updateView();
}

void FreeCamera::pitch(float amount)
{
    m_up = TransformUtils::rotate(m_right, amount, m_up);
    m_forward = TransformUtils::rotate(m_right, amount, m_forward);
    updateView();
}

void FreeCamera::roll(float amount)
{
    m_up = TransformUtils::rotate(m_forward, amount, m_up);
    m_right = TransformUtils::rotate(m_forward, amount, m_right);
    updateView();
}

void FreeCamera::yaw(float amount)
{
    m_right = TransformUtils::rotate(m_up, amount, m_right);
    m_forward = TransformUtils::rotate(m_up, amount, m_forward);
    updateView();
}

void FreeCamera::lookAt(const glm::vec3 &target, const glm::vec3 &up)
{
    m_forward = glm::normalize(target - m_position);
    m_right = -glm::normalize(glm::cross(m_forward, up));
    m_up = glm::cross(m_forward, m_right);
    updateView();
}

const glm::mat4 &FreeCamera::getView() const { return m_view; }

const glm::mat4 &FreeCamera::getProjection() const { return m_proj; }

void FreeCamera::lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
    m_position = eye;
    lookAt(target, up);
}

void FreeCamera::setProjection(float fieldOfView, float aspectRatio, float zNear, float zFar)
{
    m_fieldOfView = fieldOfView;
    m_aspectRatio = aspectRatio;
    m_zNear = zNear;
    m_zFar = zFar;
    m_proj = glm::perspective(fieldOfView, aspectRatio, zNear, zFar);
}

void FreeCamera::updateView() { m_view = glm::lookAt(m_position, m_position + m_forward, m_up); }