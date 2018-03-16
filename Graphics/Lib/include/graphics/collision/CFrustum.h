#pragma once

#include <glm/glm.hpp>

#include "graphics/collision/CBoundingSphere.h"
#include "graphics/collision/CPlane.h"

/**
 * \brief Represents a frustum for view frustum culling.
 *
 * Based on http://www.lighthouse3d.com/tutorials/view-frustum-culling.
 */
class CFrustum
{
   public:
    CFrustum();
    ~CFrustum();

    /**
     * \brief Calculates frustum planes from camera parameters.
     */
    void setFromCameraParameters(float viewAngle, float screenRatio, float nearZ, float farZ,
                                 glm::vec3 &position, glm::vec3 &look, glm::vec3 &up);

    /**
     * \brief Extracts frustum planes from model-view-projection matrix.
     *
     * Clipspace approach based on
     *http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/
     */
    void setFromViewProjectionClipSpaceApproach(const glm::mat4 &view, const glm::mat4 &proj);

    /**
     * \brief Calculates frustum planes from inverse view projection matrix.
     */
    void setFromInverseViewProjection(const glm::mat4 &invViewProj);

    /**
     * \brief Calculates frustum from view and projection matrix.
     *
     * Wrappes the setFromInverseViewProjection function by calculating inverse
     *matrices and calling the function.
     */
    void setFromViewProjection(const glm::mat4 &view, const glm::mat4 &proj);

    /**
     * \brief Checks if a point is in the Frustum.
     */
    bool isInside(glm::vec3 &point) const;

    /**
     * \brief Checks if the sphere is outside of the Frustum.
     * \brief Returns true if the sphere either is completely inside or intersects
     * the frustum and false otherwise.
     */
    bool isInsideOrIntersects(const CBoundingSphere &sphere) const;

   private:
    enum PlanePosition
    {
        Top = 0,
        Bottom,
        Left,
        Right,
        Near,
        Far,

        NumPlanes
    };

    CPlane m_planes[NumPlanes]; /**< Frustum planes. */
};