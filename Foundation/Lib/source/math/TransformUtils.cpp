#include <cmath>

#include <glm/gtx/transform.hpp>

#include "foundation/math/TransformUtils.h"

glm::vec3 TransformUtils::rotate(const glm::vec3 &axis, float amount,
                                 const glm::vec3 &vector)
{
  glm::mat4 rotation = glm::rotate(glm::mat4(1), amount, axis);
  glm::vec4 vector4 = (rotation * glm::vec4(vector.x, vector.y, vector.z, 1));
  return glm::vec3(vector4.x, vector4.y, vector4.z);
}

glm::vec3 TransformUtils::rotate(const glm::vec3 &vector,
                                 const glm::vec3 &rotation)
{
  glm::vec3 result = rotate(glm::vec3(1.f, 0.f, 0.f), rotation.x, vector);
  result = rotate(glm::vec3(0.f, 1.f, 0.f), rotation.y, result);
  return rotate(glm::vec3(0.f, 0.f, 1.f), rotation.z, result);
}

glm::mat4 TransformUtils::createRotationMatrix(const glm::vec3 &up,
                                               const glm::vec3 &right,
                                               const glm::vec3 &forward)
{
  return glm::mat4(-right.x, -right.y, -right.z, 0.0f, up.x, up.y, up.z, 0.0f,
                   forward.x, forward.y, forward.z, 0.0f, 0.0f, 0.0f, 0.0f,
                   1.0f);
}

glm::vec3 TransformUtils::createRotation(const glm::mat4 &rotation)
{
  // Extract euler angles
  if (std::abs(rotation[0][0]) == 1.f)
  {
    return glm::vec3(std::atan2(rotation[0][2], rotation[2][3]), 0.f, 0.f);
  }
  else
  {
    return glm::vec3(std::atan2(-rotation[2][0], rotation[0][0]),
                     std::asin(rotation[1][0]),
                     std::atan2(-rotation[1][2], rotation[1][1]));
  }
}

glm::mat4 TransformUtils::createModelMatrix(const glm::vec3 &position,
                                            const glm::vec3 &up,
                                            const glm::vec3 &right,
                                            const glm::vec3 &forward,
                                            float scale)
{
  glm::mat4 rotationMatrix =
      glm::mat4(-right.x, -right.y, -right.z, 0.0f, up.x, up.y, up.z, 0.0f,
                forward.x, forward.y, forward.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  return glm::translate(position) * glm::scale(glm::mat4(1), glm::vec3(scale)) *
         rotationMatrix;
}
