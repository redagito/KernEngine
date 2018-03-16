#include "graphics/resource/core/SModel.h"

SModel::SModel() {}

SModel::SModel(ResourceId mesh, ResourceId material) : m_mesh(mesh), m_material(material)
{
    return;
}