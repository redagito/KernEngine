#include "graphics/resource/core/SMaterial.h"

SMaterial::SMaterial() { return; }

SMaterial::SMaterial(ResourceId base, ResourceId normal, ResourceId specular,
                     ResourceId glow, ResourceId alpha)
    : m_base(base), m_normal(normal), m_specular(specular), m_glow(glow),
      m_alpha(alpha)
{
  return;
}