#pragma once

#include <glm/glm.hpp>

// Represens a sound listener
// Only one listener is active
class SoundListener
{
   public:
    void setPosition(const glm::vec3& position);
    void setVelocity(const glm::vec3& velocity);

    const glm::vec3& getPosition() const;
    const glm::vec3& getVelocity() const;

   private:
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 velocity = glm::vec3(0.f);
};