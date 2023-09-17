#include "gfx/PointLight.h"

void PointLight::set(Shader& shader, const std::string& prefix) const
{
    shader.setActive();
    shader.set(prefix + ".position", position);
    shader.set(prefix + ".ambient", ambient);
    shader.set(prefix + ".diffuse", diffuse);
    shader.set(prefix + ".specular", specular);
    // Attenuation factors
    shader.set(prefix + ".constant", constant);
    shader.set(prefix + ".linear", linear);
    shader.set(prefix + ".quadratic", quadratic);
}