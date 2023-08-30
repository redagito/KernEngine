#pragma once

#include <glm/glm.hpp>

/**
 * \brief Represents a 3d plane.
 */
class Plane
{
   public:
    Plane();
    Plane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
    ~Plane();

    // Ordering of points defines normal vector
    // Clockwise/Counterclockwise ordering rule is used to calculate normal vector
    void set3Points(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

    void setNormalAndPoint(const glm::vec3 &normal, const glm::vec3 &point);
    void setCoefficients(const glm::vec4 &v);
    void setCoefficients(float a, float b, float c, float d);

    /**
     * \brief Calculates the shortest, signed distance from the plane to the point.
     */
    float distance(const glm::vec3 &point) const;

   private:
    glm::vec3 m_normal = glm::vec3(0.f, 1.f, 0.f); /**< Stores the plane normal. */
    float m_d = 0.f; /**< Represents the regular euclidean distance from the origin. */
};