#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "gfx/Shader.h"

class PhongColorMaterial
{
   public:
    glm::vec3 ambient = glm::vec3(0.f);
    glm::vec3 diffuse = glm::vec3(0.f);
    glm::vec3 specular = glm::vec3(0.f);
    float shininess = 0.f;

    PhongColorMaterial() = default;
    PhongColorMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess);

    void set(Shader& shader);

    // Static material library
    static void addMaterial(const std::string& name, const PhongColorMaterial& mat);
    static const PhongColorMaterial& getMaterial(const std::string& name);

   private:
    static std::unordered_map<std::string, PhongColorMaterial> s_materials;
};