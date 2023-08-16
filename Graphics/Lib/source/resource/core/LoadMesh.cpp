#include "graphics/resource/core/LoadMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <foundation/debug/Log.h>
#include <foundation/io/CObjModelLoader.h>
#include <foundation/util/StringUtil.h>

/**
 * Load with mesh loader
 */
static bool loadMeshFromObjFallback(const std::string &file, SMesh &mesh)
{
    CObjModelLoader loader;
    if (!loader.load(file))
    {
        return false;
    }

    mesh.m_type = EPrimitiveType::Triangle;
    mesh.m_normals = loader.getNormals();
    mesh.m_uvs = loader.getUV();
    mesh.m_vertices = loader.getVertices();
    return true;
}

bool load(const std::string &file, SMesh &mesh)
{
    LOG_INFO("Loading mesh from file %s", file.c_str());
    std::string extension = getFileExtension(file);
    if (extension == "obj")
    {
        return loadMeshFromObj(file, mesh);
    }
    else
    {
        LOG_ERROR("Invalid or unknown mesh file format.");
        return false;
    }
}

bool loadMeshFromObj(const std::string &file, SMesh &mesh)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str()))
    {
        LOG_ERROR("Failed to load mesh from file %s: %s", file.c_str(), err.c_str());
        return false;
    }
    if (!err.empty())
    {
        LOG_ERROR("Error while loading mesh from file %s: %s", file.c_str(), err.c_str());
        return false;
    }
    if (!warn.empty())
    {
        LOG_WARNING("Warning while loading mesh from file %s: %s", file.c_str(), warn.c_str());
    }

    if (shapes.size() != 1)
    {
        LOG_ERROR("Loading of multiple meshes not supported: %s", file.c_str());
        return false;
    }

    auto shape = shapes.at(0);

    mesh.m_vertices = attrib.vertices;
    mesh.m_type = EPrimitiveType::Triangle;

    // Reorder nomrals and uvs to match vertex indices
    mesh.m_normals.resize(mesh.m_vertices.size());
    mesh.m_uvs.resize(mesh.m_vertices.size());
    for (auto index : shape.mesh.indices)
    {
        mesh.m_indices.push_back(index.vertex_index);
        mesh.m_normals[index.vertex_index] = attrib.normals.at(index.normal_index);
        mesh.m_uvs[index.vertex_index] = attrib.texcoords.at(index.texcoord_index);
    }
    return true;
}
