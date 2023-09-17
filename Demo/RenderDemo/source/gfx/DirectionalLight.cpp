#include "gfx/DirectionalLight.h"

void DirectionalLight::set(Shader& shader) const
{
    shader.setActive();
    shader.set("light.direction", direction);
    shader.set("light.ambient", ambient);
    shader.set("light.diffuse", diffuse);
    shader.set("light.specular", specular);
}