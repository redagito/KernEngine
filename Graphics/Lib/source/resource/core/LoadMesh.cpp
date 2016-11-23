#include "graphics/resource/core/LoadMesh.h"

#include <tiny_obj_loader.h>

#include <foundation/util/StringUtil.h>
#include <foundation/io/CObjModelLoader.h>
#include <foundation/debug/Log.h>

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
    return loadMeshFromObjFallback(file, mesh);
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
  tinyobj::attrib_t attrib;

  // Load as obj
  std::string err;
  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str()))
  {
    LOG_ERROR("%s.", err.c_str());
    return false;
  }
  if (shapes.size() > 1)
  {
    LOG_WARNING("Multple meshes in obj files not supported. The file %s "
                "contains %i meshes.",
                file.c_str(), shapes.size());
    for (const auto &shape : shapes)
    {
      LOG_DEBUG("Name: %s, Indices: %i",
                shape.name.c_str(), shape.mesh.indices.size());
    }
    return false;
  }
  if (shapes.empty())
  {
    LOG_ERROR("No mesh data loaded from file %s.", file.c_str());
    return false;
  }
  mesh.m_type = EPrimitiveType::Triangle;

  // Currently not working, needs rebuilding index buffer
  return false;
}
