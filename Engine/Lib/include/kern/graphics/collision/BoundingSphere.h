#pragma once

#include <vector>

#include <glm/glm.hpp>

class BoundingSphere
{
   public:
    BoundingSphere();
    BoundingSphere(const glm::vec3 &position, float radius);

    void setPosition(const glm::vec3 &position);
    void setRadius(float radius);

    const glm::vec3 &getPosition() const;
    float getRadius() const;

    static BoundingSphere create(const std::vector<float> &vertices);

   private:
    glm::vec3 m_position = glm::vec3(0.f);
    float m_radius = 0.f;
};

bool collides(const BoundingSphere &sphere1, const BoundingSphere &sphere2);