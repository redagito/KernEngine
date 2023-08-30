#pragma once

#include <glm/glm.hpp>

class TransformUtils
{
   public:
    static glm::vec3 rotate(const glm::vec3 &axis, float amount, const glm::vec3 &vector);

    static glm::vec3 rotate(const glm::vec3 &vector, const glm::vec3 &rotation);

    static glm::mat4 createRotationMatrix(const glm::vec3 &up, const glm::vec3 &right,
                                          const glm::vec3 &forward);

    /**
     * \brief Extracts euler angles from rotation matrix.
     */
    static glm::vec3 createRotation(const glm::mat4 &rotation);

    static glm::mat4 createModelMatrix(const glm::vec3 &position, const glm::vec3 &up,
                                       const glm::vec3 &right, const glm::vec3 &forward,
                                       float scale);
};