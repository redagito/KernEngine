#include "gfx/SpotLight.h"

void SpotLight::set(Shader& shader) const
{
    shader.setActive();
    shader.set("light.position", position);
    shader.set("light.direction", direction);
    shader.set("light.innerCutOff", std::cos(glm::radians(innerCutOff)));
    shader.set("light.outerCutOff", std::cos(glm::radians(outerCutOff)));

    shader.set("light.ambient", ambient);
    shader.set("light.diffuse", diffuse);
    shader.set("light.specular", specular);

    shader.set("light.constant", constant);
    shader.set("light.linear", linear);
    shader.set("light.quadratic", quadratic);
}