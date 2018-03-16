#include "graphics/resource/core/LoadMesh.h"

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
    // Wavefront OBJ file format loaded with tinyobj
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // Load as obj, always triangulate
    std::string err;
    if (!tinyobj::LoadObj(shapes, materials, err, file.c_str()))
    {
        LOG_ERROR("%s.", err.c_str());
        return false;
    }
    if (shapes.size() > 1)
    {
        LOG_WARNING(
            "Multple meshes in obj files not supported. The file %s "
            "contains %i meshes.",
            file.c_str(), shapes.size());
        for (const auto &shape : shapes)
        {
            LOG_DEBUG("Name: %s, Indices: %i", shape.name.c_str(), shape.mesh.indices.size());
        }
        LOG_WARNING("Only the first mesh will be loaded.");
    }
    if (shapes.empty())
    {
        LOG_ERROR("No mesh data loaded from file %s.", file.c_str());
        return false;
    }

    mesh.m_type = EPrimitiveType::Triangle;
    mesh.m_indices = shapes.at(0).mesh.indices;
    mesh.m_vertices = shapes.at(0).mesh.positions;
    mesh.m_normals = shapes.at(0).mesh.normals;
    mesh.m_uvs = shapes.at(0).mesh.texcoords;

    return true;
}
