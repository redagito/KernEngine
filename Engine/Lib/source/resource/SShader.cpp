#include "kern/resource/SShader.h"

SShader::SShader(ResourceId vertex, ResourceId tessCtrl, ResourceId tessEval, ResourceId geometry,
                 ResourceId fragment)
    : m_vertex(vertex),
      m_tessCtrl(tessCtrl),
      m_tessEval(tessEval),
      m_geometry(geometry),
      m_fragment(fragment)
{
    return;
}
