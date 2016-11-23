#pragma once

#include <vector>

#include <glm/glm.hpp>

class CBoundingSphere
{
public:
  CBoundingSphere();
  CBoundingSphere(const glm::vec3 &position, float radius);

  void setPosition(const glm::vec3 &position);
  void setRadius(float radius);

  const glm::vec3 &getPosition() const;
  float getRadius() const;

  static CBoundingSphere create(const std::vector<float> &vertices);

private:
  glm::vec3 m_position = glm::vec3(0.f);
  float m_radius = 0.f;
};

bool collides(const CBoundingSphere &sphere1, const CBoundingSphere &sphere2);