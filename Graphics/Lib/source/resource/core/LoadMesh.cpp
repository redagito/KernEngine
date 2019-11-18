#include "graphics/resource/core/LoadMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	Assimp::Importer importer;
	auto scene = importer.ReadFile(file, aiProcess_Triangulate);
	

    mesh.m_type = EPrimitiveType::Triangle;
	//mesh.m_vertices = shapes.at(0).mesh.positions;
	//mesh.m_indices = shapes.at(0).mesh.indices;
    //mesh.m_normals = shapes.at(0).mesh.normals;
    //mesh.m_uvs = shapes.at(0).mesh.texcoords;

    return true;
}
