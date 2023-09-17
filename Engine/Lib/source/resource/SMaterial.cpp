#include "kern/resource/SMaterial.h"

SMaterial::SMaterial(ResourceId base, ResourceId normal, ResourceId specular, ResourceId glow,
                     ResourceId alpha)
    : m_base(base), m_normal(normal), m_specular(specular), m_glow(glow), m_alpha(alpha)
{
    return;
}