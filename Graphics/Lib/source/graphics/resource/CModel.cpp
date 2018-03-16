#include "graphics/graphics/resource/CModel.h"

CModel::CModel(const CMesh *mesh, const CMaterial *material) : m_mesh(mesh), m_material(material)
{
    return;
}

bool CModel::init(const CMesh *mesh, const CMaterial *material)
{
    if (mesh == nullptr || material == nullptr)
    {
        return false;
    }
    m_mesh = mesh;
    m_material = material;
    return true;
}

bool CModel::hasMesh() const { return m_mesh != nullptr; }

bool CModel::hasMaterial() const { return m_material != nullptr; }

const CMesh *CModel::getMesh() const { return m_mesh; }

const CMaterial *CModel::getMaterial() const { return m_material; }