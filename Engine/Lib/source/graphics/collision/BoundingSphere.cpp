#include "kern/graphics/collision/BoundingSphere.h"

#include <cmath>

BoundingSphere::BoundingSphere()
{
    // Empty
}

BoundingSphere::BoundingSphere(const glm::vec3 &position, float radius)
    : m_position(position), m_radius(radius)
{
    // Empty
}

void BoundingSphere::setPosition(const glm::vec3 &position) { m_position = position; }

void BoundingSphere::setRadius(float radius) { m_radius = radius; }

const glm::vec3 &BoundingSphere::getPosition() const { return m_position; }

float BoundingSphere::getRadius() const { return m_radius; }

BoundingSphere BoundingSphere::create(const std::vector<float> &vertices)
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

    BoundingSphere sphere;
    // Default position is mid
    sphere.setPosition(glm::vec3(0.f));
    // Set to calculated radius
    sphere.setRadius(radius);
    return sphere;
}

bool collides(const BoundingSphere &sphere1, const BoundingSphere &sphere2)
{
    float distance = glm::distance(sphere1.getPosition(), sphere2.getPosition());
    return distance < (sphere1.getRadius() + sphere2.getRadius());
}