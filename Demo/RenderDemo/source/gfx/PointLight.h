#pragma once

#include <glm/vec3.hpp>
#include "gfx/Shader.h"

class PointLight
{
   public:
    glm::vec3 position = glm::vec3(0.f);
    // Per color light strength
    glm::vec3 ambient = glm::vec3(0.f);
    glm::vec3 diffuse = glm::vec3(0.f);
    glm::vec3 specular = glm::vec3(0.f);

    // Attenuation factors with sane defaults
    float constant = 1.f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    void set(Shader& shader) const;
};