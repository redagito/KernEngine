#include "graphics/collision/CBoundingSphere.h"

#include <cmath>

CBoundingSphere::CBoundingSphere()
{
    // Empty
}

CBoundingSphere::CBoundingSphere(const glm::vec3 &position, float radius)
    : m_position(position), m_radius(radius)
{
    // Empty
}

void CBoundingSphere::setPosition(const glm::vec3 &position) { m_position = position; }

void CBoundingSphere::setRadius(float radius) { m_radius = radius; }

const glm::vec3 &CBoundingSphere::getPosition() const { return m_position; }

float CBoundingSphere::getRadius() const { return m_radius; }

CBoundingSphere CBoundingSphere::create(const std::vector<float> &vertices)
{
    // Simple algorithm: Assume 0/0/0 is mid point of the mesh.
    // Radius is distance from mid point to furthest vertex
    float radius = 0.f;

    // Retrieve furthest vertex
    for (unsigned int i = 0; i < vertices.size(); i += 3)
    {
        float distance =
            std::sqrt(vertices.at(i) * vertices.at(i) + vertices.at(i + 1) * vertices.at(i + 1) +
                      vertices.at(i + 2) * vertices.at(i + 2));
        if (distance > radius)
        {
            radius = distance;
        }
    }

    CBoundingSphere sphere;
    // Default position is mid
    sphere.setPosition(glm::vec3(0.f));
    // Set to calculated radius
    sphere.setRadius(radius);
    return sphere;
}

bool collides(const CBoundingSphere &sphere1, const CBoundingSphere &sphere2)
{
    float distance = glm::distance(sphere1.getPosition(), sphere2.getPosition());
    return distance < (sphere1.getRadius() + sphere2.getRadius());
}