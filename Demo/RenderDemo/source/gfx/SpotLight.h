#pragma once

#include <glm/glm.hpp>

#include "gfx/Shader.h"

class SpotLight
{
   public:
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, 1.f);

    // Attenuation factors with sane defaults
    float constant = 1.f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    // Light
    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(1.f);

    // Cutoff angles in degrees
    float innerCutOff = 12.5f;
    float outerCutOff = 15.f;

    void set(Shader& shader, const std::string& prefix) const;
};