#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "kern/resource/IResourceLoader.h"
#include "kern/resource/IResourceManager.h"
#include "kern/resource/Image.h"
#include "kern/resource/ResourceEvent.h"
#include "kern/resource/ResourceType.h"
#include "kern/resource/SMaterial.h"
#include "kern/resource/SMesh.h"
#include "kern/resource/SModel.h"
#include "kern/resource/SShader.h"

/**
 * \brief Resource manager implementation.
 */
class ResourceManager : public IResourceManager
{
   public:
    ResourceManager() = default;
    ~ResourceManager();

    bool loadFromFile(const std::string &file) override;

    ResourceId createMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                          const std::vector<float> &normals, const std::vector<float> &uvs,
                          PrimitiveType type) override;

    ResourceId loadMesh(const std::string &file) override;

    bool getMesh(ResourceId id, std::vector<float> &vertices, std::vector<unsigned int> &indices,
                 std::vector<float> &normals, std::vector<float> &uvs, PrimitiveType &type) const override;

    ResourceId createImage(const std::vector<unsigned char> &imageData, unsigned int width, unsigned int height,
                           ColorFormat format) override;

    ResourceId loadImage(const std::string &file, ColorFormat format) override;

    bool getImage(ResourceId id, std::vector<unsigned char> &data, unsigned int &width, unsigned int &height,
                  ColorFormat &format) const override;

    ResourceId createMaterial(ResourceId base, ResourceId normal, ResourceId specular, ResourceId glow,
                              ResourceId alpha) override;

    ResourceId loadMaterial(const std::string &file) override;

    bool getMaterial(ResourceId id, ResourceId &base, ResourceId &normal, ResourceId &specular, ResourceId &glow,
                     ResourceId &alpha) const override;

    ResourceId createModel(ResourceId mesh, ResourceId material) override;

    ResourceId loadModel(const std::string &file) override;

    bool getModel(ResourceId id, ResourceId &mesh, ResourceId &material) override;

    ResourceId createString(const std::string &text) override;

    ResourceId loadString(const std::string &file) override;

    bool getString(ResourceId id, std::string &text) const override;

    ResourceId createShader(ResourceId vertex, ResourceId tessCtrl, ResourceId tessEval, ResourceId geometry,
                            ResourceId fragment) override;

    ResourceId loadShader(const std::string &file) override;

    bool getShader(ResourceId id, ResourceId &vertex, ResourceId &tessCtrl, ResourceId &tessEval, ResourceId &geometry,
                   ResourceId &fragment) const override;

    void addResourceListener(IResourceListener *listener);
    void removeResourceListener(IResourceListener *listener);

    // Adds a factory function for crearing a resource loader object for a specific file extension
    void addResourceLoaderCreator(const std::string &extension, const std::function<std::unique_ptr<IResourceLoader>()>& creator);

   protected:
    void notifyResourceListeners(ResourceType type, ResourceId id, ResourceEvent event);

   private:
    ResourceId m_nextMeshId = 0;     /**< Next free mesh id. */
    ResourceId m_nextImageId = 0;    /**< Next free image id. */
    ResourceId m_nextMaterialId = 0; /**< Next free material id. */
    ResourceId m_nextModelId = 0;    /**< Next free model id. */
    ResourceId m_nextStringId = 0;   /**< Next free string id. */
    ResourceId m_nextShaderId = 0;   /**< Next free shader id. */

    // TODO Change to vector?
    std::unordered_map<ResourceId, SMesh> m_meshes;        /**< Loaded meshes. */
    std::unordered_map<ResourceId, Image> m_images;        /**< Loaded images. */
    std::unordered_map<ResourceId, SMaterial> m_materials; /**< Loaded materials. */
    std::unordered_map<ResourceId, SModel> m_models;       /**< Loaded models. */
    std::unordered_map<ResourceId, std::string> m_strings; /**< Loaded strings. */
    std::unordered_map<ResourceId, SShader> m_shaders;     /**< Loaded shaders. */

    std::unordered_map<std::string, ResourceId> m_meshFiles;     /**< Maps mesh file to mesh resource id. */
    std::unordered_map<std::string, ResourceId> m_imageFiles;    /**< Maps image file to image resource id. */
    std::unordered_map<std::string, ResourceId> m_materialFiles; /**< Maps material file to material resource id. */
    std::unordered_map<std::string, ResourceId> m_modelFiles;    /**< Maps model file to model resource id. */
    std::unordered_map<std::string, ResourceId> m_textFiles;     /**< Maps text file to string resource id. */
    std::unordered_map<std::string, ResourceId> m_shaderFiles;   /**< Maps shader program file to shader resource id. */

    std::list<IResourceListener *> m_resourceListeners; /**< Registered listeners. */
    // Resource loader creation functions
    std::unordered_map<std::string, std::function<std::unique_ptr<IResourceLoader>(void)>> m_resourceLoaderCreators;
};
