#include "graphics/resource/CResourceManager.h"

#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>

#include <foundation/debug/Log.h>
#include <foundation/io/CIniFile.h>
#include <foundation/io/CObjModelLoader.h>
#include <foundation/io/CShaderPreprocessor.h>
#include <foundation/io/JsonDeserialize.h>
#include <foundation/io/JsonUtil.h>

#include "graphics/resource/IResourceListener.h"
#include "graphics/resource/LoadImage.h"
#include "graphics/resource/LoadMaterial.h"
#include "graphics/resource/LoadMesh.h"
#include "graphics/resource/LoadModel.h"
#include "graphics/resource/LoadShader.h"

CResourceManager::CResourceManager() { return; }

CResourceManager::~CResourceManager()
{
    for (auto listener : m_resourceListeners)
    {
        listener->onDetach(this);
    }
}

ResourceId CResourceManager::createMesh(const std::vector<float> &vertices,
                                        const std::vector<unsigned int> &indices,
                                        const std::vector<float> &normals,
                                        const std::vector<float> &uvs, EPrimitiveType type)
{
    // Create mesh id
    ResourceId id = m_nextMeshId;
    ++m_nextMeshId;

    // Add mesh
    m_meshes[id] = SMesh(vertices, indices, normals, uvs, type);

    // Notify listener with create event
    notifyResourceListeners(EResourceType::Mesh, id, EListenerEvent::Create);
    return id;
}

ResourceId CResourceManager::loadMesh(const std::string &file)
{
    auto entry = m_meshFiles.find(file);
    if (entry != m_meshFiles.end())
    {
        return entry->second;
    }

    // Load mesh
    SMesh mesh;
    if (!load(file, mesh))
    {
        LOG_ERROR("Failed to load mesh from file %s.", file.c_str());
        return invalidResource;
    }

    // Create mesh resource
    ResourceId meshId =
        createMesh(mesh.m_vertices, mesh.m_indices, mesh.m_normals, mesh.m_uvs, mesh.m_type);
    if (meshId == invalidResource)
    {
        LOG_ERROR("Failed to create mesh resource id from file %s.", file.c_str());
        return invalidResource;
    }
    m_meshFiles[file] = meshId;
    return meshId;
}

bool CResourceManager::getMesh(ResourceId id, std::vector<float> &vertices,
                               std::vector<unsigned int> &indices, std::vector<float> &normals,
                               std::vector<float> &uvs, EPrimitiveType &type) const
{
    // Retrieve from map
    auto iter = m_meshes.find(id);
    if (iter == m_meshes.end())
    {
        return false;
    }
    // Copy data
    vertices = iter->second.m_vertices;
    indices = iter->second.m_indices;
    normals = iter->second.m_normals;
    uvs = iter->second.m_uvs;
    type = iter->second.m_type;
    return true;
}

ResourceId CResourceManager::createImage(const std::vector<unsigned char> &imageData,
                                         unsigned int width, unsigned int height,
                                         EColorFormat format)
{
    // Create image
    ResourceId id = m_nextImageId;
    ++m_nextImageId;

    // TODO Sanity check if mesh id exists?
    // Add mesh
    m_images[id] = SImage(imageData, width, height, format);

    // Notify listener with create event
    notifyResourceListeners(EResourceType::Image, id, EListenerEvent::Create);
    return id;
}

ResourceId CResourceManager::loadImage(const std::string &file, EColorFormat format)
{
    auto entry = m_imageFiles.find(file);
    if (entry != m_imageFiles.end())
    {
        return entry->second;
    }

    // Load image
    SImage image;
    if (!load(file, format, image))
    {
        LOG_ERROR("Failed to load image from file %s.", file.c_str());
        return invalidResource;
    }

    // Create managed resource
    ResourceId imageId = createImage(image.m_data, image.m_width, image.m_height, image.m_format);
    if (imageId == invalidResource)
    {
        LOG_ERROR("Failed to create image resource id from file %s.", file.c_str());
        return invalidResource;
    }
    m_imageFiles[file] = imageId;
    return imageId;
}

bool CResourceManager::getImage(ResourceId id, std::vector<unsigned char> &data,
                                unsigned int &width, unsigned int &height,
                                EColorFormat &format) const
{
    // Retrieve from map
    auto iter = m_images.find(id);
    if (iter == m_images.end())
    {
        return false;
    }
    // Copy data
    data = iter->second.m_data;
    width = iter->second.m_width;
    height = iter->second.m_height;
    format = iter->second.m_format;
    return true;
}

ResourceId CResourceManager::createMaterial(ResourceId base, ResourceId normal, ResourceId specular,
                                            ResourceId glow, ResourceId alpha)
{
    // Create material
    ResourceId id = m_nextMaterialId;
    ++m_nextMaterialId;

    // Add material
    m_materials[id] = SMaterial(base, normal, specular, glow, alpha);

    // Notify listener with create event
    notifyResourceListeners(EResourceType::Material, id, EListenerEvent::Create);
    return id;
}

ResourceId CResourceManager::loadMaterial(const std::string &file)
{
    auto entry = m_materialFiles.find(file);
    if (entry != m_materialFiles.end())
    {
        return entry->second;
    }

    LOG_DEBUG("Loading material from file %s.", file.c_str());
    SMaterial material;
    if (!load(file, *this, material))
    {
        LOG_ERROR("Failed to load material from file %s.", file.c_str());
        return invalidResource;
    }

    ResourceId materialId = createMaterial(material.m_base, material.m_normal, material.m_specular,
                                           material.m_glow, material.m_alpha);
    if (materialId == invalidResource)
    {
        LOG_ERROR("Failed to create material resource id for material file %s.", file.c_str());
        return invalidResource;
    }
    m_materialFiles[file] = materialId;
    return materialId;
}

bool CResourceManager::getMaterial(ResourceId id, ResourceId &base, ResourceId &normal,
                                   ResourceId &specular, ResourceId &glow, ResourceId &alpha) const
{
    // Retrieve from map
    auto iter = m_materials.find(id);
    if (iter == m_materials.end())
    {
        return false;
    }
    // Copy data
    base = iter->second.m_base;
    normal = iter->second.m_normal;
    specular = iter->second.m_specular;
    glow = iter->second.m_glow;
    alpha = iter->second.m_alpha;
    return true;
}

ResourceId CResourceManager::createModel(ResourceId mesh, ResourceId material)
{
    // Create model
    ResourceId id = m_nextModelId;
    ++m_nextModelId;

    // Add material
    m_models[id] = SModel(mesh, material);

    // Notify listener with create event
    notifyResourceListeners(EResourceType::Model, id, EListenerEvent::Create);
    return id;
}

ResourceId CResourceManager::loadModel(const std::string &file)
{
    auto entry = m_modelFiles.find(file);
    if (entry != m_modelFiles.end())
    {
        return entry->second;
    }

    LOG_DEBUG("Loading model from file %s.", file.c_str());
    SModel model;
    if (!load(file, *this, model))
    {
        LOG_ERROR("Failed to load model from file %s.", file.c_str());
        return invalidResource;
    }

    ResourceId modelId = createModel(model.m_mesh, model.m_material);
    if (modelId == invalidResource)
    {
        LOG_ERROR("Failed to create model resource id for model file %s.", file.c_str());
        return invalidResource;
    }
    m_modelFiles[file] = modelId;
    return modelId;
}

bool CResourceManager::getModel(ResourceId id, ResourceId &mesh, ResourceId &material)
{
    // Retrieve from map
    auto iter = m_models.find(id);
    if (iter == m_models.end())
    {
        return false;
    }
    // Copy data
    mesh = iter->second.m_mesh;
    material = iter->second.m_material;
    return true;
}

ResourceId CResourceManager::createString(const std::string &text)
{
    // Create string
    ResourceId id = m_nextStringId;
    ++m_nextStringId;

    // Add string
    m_strings[id] = text;

    // Notify listener with create event
    notifyResourceListeners(EResourceType::String, id, EListenerEvent::Create);
    return id;
}

ResourceId CResourceManager::loadString(const std::string &file) { return loadString(file, false); }

bool CResourceManager::getString(ResourceId id, std::string &text) const
{
    // Retrieve from map
    auto iter = m_strings.find(id);
    if (iter == m_strings.end())
    {
        return false;
    }
    // Copy data
    text = iter->second;
    return true;
}

ResourceId CResourceManager::createShader(ResourceId vertex, ResourceId tessCtrl,
                                          ResourceId tessEval, ResourceId geometry,
                                          ResourceId fragment)
{
    // Needs at least valid vertex and fragment shader
    if (vertex == invalidResource || fragment == invalidResource)
    {
        KERN_ERROR(
            "Failed to create shader, resource id for vertex or fragment shader is invalid.");
        return invalidResource;
    }

    // Create shader
    ResourceId id = m_nextShaderId;
    ++m_nextShaderId;

    // Add shader
    m_shaders[id] = SShader(vertex, tessCtrl, tessEval, geometry, fragment);

    // Notify listener with create event
    notifyResourceListeners(EResourceType::Shader, id, EListenerEvent::Create);
    return id;
}

bool CResourceManager::getShader(ResourceId id, ResourceId &vertex, ResourceId &tessCtrl,
                                 ResourceId &tessEval, ResourceId &geometry,
                                 ResourceId &fragment) const
{
    // Retrieve from map
    auto iter = m_shaders.find(id);
    if (iter == m_shaders.end())
    {
        return false;
    }
    // Copy data
    vertex = iter->second.m_vertex;
    tessCtrl = iter->second.m_tessCtrl;
    tessEval = iter->second.m_tessEval;
    geometry = iter->second.m_geometry;
    fragment = iter->second.m_fragment;
    return true;
}

ResourceId CResourceManager::loadShader(const std::string &file)
{
    // Check if shader exists
    auto entry = m_shaderFiles.find(file);
    if (entry != m_shaderFiles.end())
    {
        return entry->second;
    }

    LOG_DEBUG("Loading shader from file %s.", file.c_str());
    SShader shader;
    if (!load(file, *this, shader))
    {
        LOG_ERROR("Failed to load shader from file %s.", file.c_str());
        return invalidResource;
    }

    ResourceId shaderId = invalidResource;
    shaderId = createShader(shader.m_vertex, shader.m_tessCtrl, shader.m_tessEval,
                            shader.m_geometry, shader.m_fragment);
    if (shaderId == invalidResource)
    {
        LOG_ERROR("Failed to create reasource id for shader file %s.", file.c_str());
        return invalidResource;
    }

    m_shaderFiles[file] = shaderId;
    return shaderId;
}

void CResourceManager::addResourceListener(IResourceListener *listener)
{
    m_resourceListeners.push_back(listener);
    listener->onAttach(this);
}

void CResourceManager::removeResourceListener(IResourceListener *listener)
{
    m_resourceListeners.remove(listener);
    listener->onDetach(this);
}

void CResourceManager::notifyResourceListeners(EResourceType type, ResourceId id,
                                               EListenerEvent event)
{
    for (const auto &listener : m_resourceListeners)
    {
        listener->notify(type, id, event, this);
    }
}

ResourceId CResourceManager::loadString(const std::string &file, bool preprocess)
{
    auto iter = m_textFiles.find(file);
    if (iter != m_textFiles.end())
    {
        return iter->second;
    }

    LOG_DEBUG("Loading text from file %s.", file.c_str());
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        LOG_ERROR("Failed to open the text file %s.", file.c_str());
        return -1;
    }

    // Load file
    std::string text((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    ResourceId stringId = -1;

    if (preprocess)
    {
        // Preprocessing of include statements for shader source files
        CShaderPreprocessor preprocessor;
        preprocessor.setIncludePath("data/shader/include/");
        if (!preprocessor.preprocess(text, text))
        {
            LOG_ERROR("Failed to preprocess the text file %s.", file.c_str());
            return -1;
        }
    }

    // Create new string entry
    stringId = createString(text);
    if (stringId == -1)
    {
        LOG_ERROR("Failed to create string id for text file %s.", file.c_str());
        return -1;
    }

    m_textFiles[file] = stringId;
    return stringId;
}