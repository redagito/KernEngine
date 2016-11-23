#include "graphics/sampler/Interpolation.h"

#include <cassert>

float quadricInterpolation(const glm::vec2 &sample0, const glm::vec2 &sample1,
                           const glm::vec2 &sample2, float x)
{
  assert(sample0.x - sample1.x != 0.f);
  assert(sample0.x - sample2.x != 0.f);
  assert(sample1.x - sample2.x != 0.f);

  float x0 = sample0.x;
  float x1 = sample1.x;
  float x2 = sample2.x;
  float y0 = sample0.y;
  float y1 = sample1.y;
  float y2 = sample2.y;

  // Quadric interpolation constants
  float a = (x0 * (y2 - y1) + x1 * (y0 - y2) + x2 * (y1 - y0)) /
            ((x0 - x1) * (x0 - x2) * (x1 - x2));
  float b = (y1 - y0 + a * x0 * x0 - a * x1 * x1) / (x1 - x0);
  float c = y0 - a * x0 * x0 - b * x0;

  // Caclulate value
  return a * x * x + b * x + c;
}
