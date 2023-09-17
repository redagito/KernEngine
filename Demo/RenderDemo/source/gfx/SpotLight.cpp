#include "gfx/SpotLight.h"

void SpotLight::set(Shader& shader, const std::string& prefix) const
{
    shader.setActive();
    shader.set(prefix + ".position", position);
    shader.set(prefix + ".direction", direction);
    shader.set(prefix + ".innerCutOff", std::cos(glm::radians(innerCutOff)));
    shader.set(prefix + ".outerCutOff", std::cos(glm::radians(outerCutOff)));

    shader.set(prefix + ".ambient", ambient);
    shader.set(prefix + ".diffuse", diffuse);
    shader.set(prefix + ".specular", specular);

    shader.set(prefix + ".constant", constant);
    shader.set(prefix + ".linear", linear);
    shader.set(prefix + ".quadratic", quadratic);
}