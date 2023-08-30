#include "gfx/PhongColorMaterial.h"

#include <stdexcept>

std::unordered_map<std::string, PhongColorMaterial> PhongColorMaterial::s_materials;

PhongColorMaterial::PhongColorMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                                       float shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
}

void PhongColorMaterial::set(Shader& shader)
{
    shader.setActive();
    shader.set("material.ambient", ambient);
    shader.set("material.diffuse", diffuse);
    shader.set("material.specular", specular);
    shader.set("material.shininess", shininess);
}

void PhongColorMaterial::addMaterial(const std::string& name, const PhongColorMaterial& mat)
{
    s_materials[name] = mat;
}

const PhongColorMaterial& PhongColorMaterial::getMaterial(const std::string& name)
{
    auto it = s_materials.find(name);
    if (it != s_materials.end())
        return it->second;

    throw std::runtime_error("Failed to find material " + name);
}