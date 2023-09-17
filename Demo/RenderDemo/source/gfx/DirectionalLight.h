#pragma once

#include <glm/glm.hpp>

#include "gfx/Shader.h"

class DirectionalLight
{
   public:
    // Ligth direction, default towards Z
    glm::vec3 direction = glm::vec3(0.f, 0.f, 1.f);
    // Colors
    glm::vec3 ambient = glm::vec3(1.f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);

    void set(Shader& shader, const std::string& prefix) const;
};