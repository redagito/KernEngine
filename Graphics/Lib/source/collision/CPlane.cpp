#include "graphics/collision/CPlane.h"

CPlane::CPlane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
{
  set3Points(v1, v2, v3);
}

CPlane::CPlane() {}

CPlane::~CPlane() {}

void CPlane::set3Points(const glm::vec3 &v1, const glm::vec3 &v2,
                        const glm::vec3 &v3)
{
  m_normal = glm::normalize(glm::cross(v1 - v2, v3 - v2));
  m_d = -(glm::dot(m_normal, v2));
}

void CPlane::setNormalAndPoint(const glm::vec3 &normal, const glm::vec3 &point)
{
  m_normal = glm::normalize(normal);
  m_d = -(glm::dot(m_normal, point));
}

void CPlane::setCoefficients(const glm::vec4 &v)
{
  setCoefficients(v.x, v.y, v.z, v.w);
}

void CPlane::setCoefficients(float a, float b, float c, float d)
{
  // Based on
  // http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-implementation-details/
  // Set the normal vector
  m_normal = glm::vec3(a, b, c);
  // Compute the length of the vector
  float length = glm::length(m_normal);
  // Normalize the vector
  m_normal = glm::normalize(m_normal);
  // d is the signed offset to the origin
  // Divide d by length to get regular euclidean distance
  m_d = d / length;
}

float CPlane::distance(const glm::vec3 &p) const
{
  return m_d + glm::dot(m_normal, p);
}