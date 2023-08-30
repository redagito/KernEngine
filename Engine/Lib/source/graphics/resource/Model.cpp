#include "kern/graphics/resource/Model.h"

Model::Model(const Mesh *mesh, const Material *material) : m_mesh(mesh), m_material(material)
{
    return;
}

bool Model::init(const Mesh *mesh, const Material *material)
{
    if (mesh == nullptr || material == nullptr)
    {
        return false;
    }
    m_mesh = mesh;
    m_material = material;
    return true;
}

bool Model::hasMesh() const { return m_mesh != nullptr; }

bool Model::hasMaterial() const { return m_material != nullptr; }

const Mesh *Model::getMesh() const { return m_mesh; }

const Material *Model::getMaterial() const { return m_material; }