#pragma once

#include <glm/glm.hpp>

class Camera
{
   public:
    // Camera position in world space
    void setPosition(const glm::vec3& position);
    // Camera direction vector
    void setDirection(const glm::vec3& direction);
    // Camera direction from yaw and pitch
    void setDirection(const float yaw, const float pitch);
    // Camera direction from look at point
    void setLookAt(const glm::vec3& point);
    // Up vector
    void setUp(const glm::vec3& up);

    const glm::vec3& getPosition() const;
    const glm::vec3& getDirection() const;
    const glm::vec3& getUp() const;

    // Utility
    // Normalized right vector
    const glm::vec3 getRight() const;

    // View matrix
    const glm::mat4& getView() const;

   private:
    // Position in world space
    glm::vec3 position = glm::vec3{0.f, 0.f, 0.f};
    // Camera direction in world space
    glm::vec3 direction = glm::vec3{0.f, 0.f, -1.f};
    // Camera up vector
    glm::vec3 up = glm::vec3{0.f, 1.f, 0.f};

    // View matrix, recalculated on access
    mutable bool viewDirty = true;
    mutable glm::mat4 view;
};