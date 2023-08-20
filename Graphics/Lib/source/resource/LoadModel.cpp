#include "graphics/resource/LoadModel.h"

#include <foundation/debug/Log.h>
#include <foundation/io/JsonDeserialize.h>
#include <foundation/io/JsonUtil.h>
#include <foundation/util/StringUtil.h>

bool load(const std::string &file, CResourceManager &manager, SModel &model)
{
    std::string extension = getFileExtension(file);
    std::string mesh;
    std::string material;
    if (extension == "json")
    {
        if (!loadModelFromJson(file, mesh, material))
        {
            return false;
        }
    }
    else
    {
        LOG_ERROR("Invalid or unknown model file format.");
        return false;
    }

    // Load mesh
    ResourceId meshId = manager.loadMesh(mesh);
    if (meshId == invalidResource)
    {
        LOG_ERROR("Failed to load mesh.");
        return false;
    }

    // Load material
    ResourceId materialId = manager.loadMaterial(material);
    if (meshId == invalidResource)
    {
        LOG_ERROR("Failed to load material.");
        return false;
    }

    // Set ids
    model.m_mesh = meshId;
    model.m_material = materialId;
    return true;
}

bool loadModelFromJson(const std::string &file, std::string &meshFile, std::string &materialFile)
{
    nlohmann::json root;
    if (!load(file, root))
    {
        return false;
    }

    if (!deserialize(root, "mesh", meshFile))
    {
        LOG_ERROR("Failed to load mesh node.");
        return false;
    }
    if (!deserialize(root, "material", materialFile))
    {
        LOG_ERROR("Failed to load material node.");
        return false;
    }
    return true;
}