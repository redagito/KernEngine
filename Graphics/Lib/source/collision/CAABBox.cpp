#include "graphics/collision/CAABBox.h"

#include <sstream>
#include <cmath>
#include <cassert>

// Returns mid point
const glm::vec3 &CAABBox::getMid() const { return m_middlePoint; }

// Returns halfwidth vector
const glm::vec3 &CAABBox::getHalfWidths() const { return m_halfWidth; }

// Sets middle
void CAABBox::setMid(const glm::vec3 &mid) { m_middlePoint = mid; }

// Sets half width
void CAABBox::setHalfWidths(const glm::vec3 &halfWidth)
{
  m_halfWidth = halfWidth;
}

// Returns as string
std::string CAABBox::toString() const
{
  std::stringstream ss("CAABBox:");
  ss << " mid.x =" << m_middlePoint.x;
  ss << " mid.y =" << m_middlePoint.y;
  ss << " mid.z =" << m_middlePoint.z;
  ss << " halfwidth.x =" << m_halfWidth.x;
  ss << " halfwidth.y =" << m_halfWidth.y;
  ss << " halfwidth.z =" << m_halfWidth.z;
  return ss.str();
}

CAABBox CAABBox::create(const std::vector<float> &vertices)
{
  assert(vertices.size() >= 3);
  assert(vertices.size() % 3 == 0);
  float xmin = vertices[0];
  float xmax = vertices[0];
  float ymin = vertices[1];
  float ymax = vertices[1];
  float zmin = vertices[2];
  float zmax = vertices[2];

  for (unsigned int i = 0; i < vertices.size(); i += 3)
  {
    // X coord
    if (vertices[i] < xmin)
    {
      xmin = vertices[i];
    }
    if (vertices[i] > xmax)
    {
      xmax = vertices[i];
    }
    // Y coord
    if (vertices[i + 1] < ymin)
    {
      ymin = vertices[i + 1];
    }
    if (vertices[i + 1] > ymax)
    {
      ymax = vertices[i + 1];
    }
    // Z coord
    if (vertices[i + 2] < zmin)
    {
      zmin = vertices[i + 2];
    }
    if (vertices[i + 2] > zmax)
    {
      zmax = vertices[i + 2];
    }
  }

  glm::vec3 mid((xmin + xmax) / 2.f, (ymin + ymax) / 2.f, (zmin + zmax) / 2.f);
  CAABBox box;
  box.setMid(mid);
  box.setHalfWidths(glm::vec3(xmax - box.getMid().x, ymax - box.getMid().y,
                              zmax - box.getMid().z));
  return box;
}

bool collides(const CAABBox &box1, const CAABBox &box2)
{
  glm::vec3 diff = box1.getMid() - box2.getMid();
  glm::vec3 widths = box1.getHalfWidths() + box2.getHalfWidths();
  if (std::abs(diff.x) > widths.x || std::abs(diff.y) > widths.y ||
      std::abs(diff.z) > widths.z)
  {
    return false;
  }
  return true;
}