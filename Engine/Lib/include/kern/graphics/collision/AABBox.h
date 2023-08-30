#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

/**
 * \brief Axis aligned bounding box
 */
class AABBox
{
   public:
    /**
     * \brief Returns middle of the box
     */
    const glm::vec3 &getMid() const;

    /**
     * \brief Returns the halfwidths
     */
    const glm::vec3 &getHalfWidths() const;

    /**
     * \brief Sets the mid point
     */
    void setMid(const glm::vec3 &middle);

    /**
     * \brief Sets halfwidth vector
     */
    void setHalfWidths(const glm::vec3 &halfWidths);

    /**
     * \brief Returns string representation of the bounding box.
     *
     * Used for debugging.
     */
    std::string toString() const;

    /**
     * \brief Creates axis aligned bounding box from vertices.
     */
    static AABBox create(const std::vector<float> &vertices);

   private:
    glm::vec3 m_middlePoint = glm::vec3(0.f); /**< Middle of the box. */
    glm::vec3 m_halfWidth = glm::vec3(0.f);   /**< Halfwidths of the box. */
};

/**
 * \brief Collision check.
 */
bool collides(const AABBox &b1, const AABBox &b2);