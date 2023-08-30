#pragma once

#include <glm/glm.hpp>

/**
 * \brief Quadric interpolation for 3 samples.
 */
float quadricInterpolation(const glm::vec2 &sample0, const glm::vec2 &sample1,
                           const glm::vec2 &sample2, float x);