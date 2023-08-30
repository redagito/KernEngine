#pragma once

#include <glm/glm.hpp>

#include "kern/graphics/ICamera.h"

class Camera : public ICamera
{
   public:
    void setView(const glm::mat4 &view);
    const glm::mat4 &getView() const;

    void setProjection(const glm::mat4 &proj);
    const glm::mat4 &getProjection() const;

    void setPosition(const glm::vec3 &position);
    const glm::vec3 &getPosition() const;

   private:
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::vec3 m_position;
};