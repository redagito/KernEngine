#include "gfx/DirectionalLight.h"

void DirectionalLight::set(Shader& shader, const std::string& prefix) const
{
    shader.setActive();
    shader.set(prefix + ".direction", direction);
    shader.set(prefix + ".ambient", ambient);
    shader.set(prefix + ".diffuse", diffuse);
    shader.set(prefix + ".specular", specular);
}