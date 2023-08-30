#include "kern/graphics/resource/LoadMesh.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fmtlog/fmtlog.h>

#include <assimp/Importer.hpp>

#include "kern/foundation/StringUtil.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

bool load(const std::string &file, SMesh &mesh)
{
    logi("Loading mesh from file {}", file.c_str());
    std::string extension = getFileExtension(file);
    if (extension == "obj")
    {
        return loadMeshFromObj(file, mesh);
    }
    else
    {
        loge("Invalid or unknown mesh file format.");
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
        loge("Failed to load mesh from file %s: {}", file.c_str(), err.c_str());
        return false;
    }
    if (!err.empty())
    {
        loge("Error while loading mesh from file %s: {}", file.c_str(), err.c_str());
        return false;
    }
    if (!warn.empty())
    {
        logw("Warning while loading mesh from file %s: {}", file.c_str(), warn.c_str());
    }

    if (shapes.size() != 1)
    {
        loge("Loading of multiple meshes not supported: {}", file.c_str());
        return false;
    }

    auto shape = shapes.at(0);

    mesh.m_type = EPrimitiveType::Triangle;
    /*
    mesh.m_vertices = attrib.vertices;

    // Reorder normals and uvs to match vertex indices
    // TODO Doesnt work
    mesh.m_normals.resize(mesh.m_vertices.size(), -1.f);
    mesh.m_uvs.resize(mesh.m_vertices.size() / 3 * 2);
    for (auto index : shape.mesh.indices)
    {
        mesh.m_indices.push_back(index.vertex_index);
        //
        if (index.normal_index >= 0)
        {
            mesh.m_normals[index.vertex_index * 3] = attrib.normals.at(index.normal_index * 3);
            mesh.m_normals[index.vertex_index * 3 + 1] = attrib.normals.at(index.normal_index * 3 + 1);
            mesh.m_normals[index.vertex_index * 3 + 2] = attrib.normals.at(index.normal_index * 3 + 2);
        }

        if (index.texcoord_index >= 0)
        {
            mesh.m_uvs[index.vertex_index * 2] = attrib.texcoords.at(index.texcoord_index * 2);
            mesh.m_uvs[index.vertex_index * 2 + 1] = attrib.texcoords.at(index.texcoord_index * 2 + 1);
        }
    }
    */
    // Flatten and remove index buffer to non indexed mesh
    for (auto index : shape.mesh.indices)
    {
        // Vertex x/y/z
        mesh.m_vertices.push_back(attrib.vertices.at(index.vertex_index * 3));
        mesh.m_vertices.push_back(attrib.vertices.at(index.vertex_index * 3 + 1));
        mesh.m_vertices.push_back(attrib.vertices.at(index.vertex_index * 3 + 2));
        // Normal x/y/z
        mesh.m_normals.push_back(attrib.normals.at(index.normal_index * 3));
        mesh.m_normals.push_back(attrib.normals.at(index.normal_index * 3 + 1));
        mesh.m_normals.push_back(attrib.normals.at(index.normal_index * 3 + 2));
        // UVs
        if (index.texcoord_index >= 0)
        {
            mesh.m_uvs.push_back(attrib.texcoords.at(index.texcoord_index * 2));
            mesh.m_uvs.push_back(attrib.texcoords.at(index.texcoord_index * 2 + 1));
        }
        else
        {
            mesh.m_uvs.push_back(0.f);
            mesh.m_uvs.push_back(0.f);
        }
    }

    return true;
}
