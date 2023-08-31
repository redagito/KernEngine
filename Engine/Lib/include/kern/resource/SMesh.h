#pragma once

#include <vector>

#include "kern/resource/ResourceId.h"
#include "kern/resource/PrimitiveType.h"

/**
 * \brief Mesh data.
 */
struct SMesh
{
    SMesh();
    SMesh(std::vector<float> vertices, std::vector<unsigned int> indices,
          std::vector<float> normals, std::vector<float> uvs, PrimitiveType type);

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<float> m_normals;
    std::vector<float> m_uvs;
    PrimitiveType m_type;
};
