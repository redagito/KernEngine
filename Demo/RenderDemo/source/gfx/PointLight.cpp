#include "gfx/PointLight.h"

void PointLight::set(Shader& shader) const
{
    shader.setActive();
    shader.set("light.position", position);
    shader.set("light.ambient", ambient);
    shader.set("light.diffuse", diffuse);
    shader.set("light.specular", specular);
    // Attenuation factors
    shader.set("light.constant", constant);
    shader.set("light.linear", linear);
    shader.set("light.quadratic", quadratic);
}