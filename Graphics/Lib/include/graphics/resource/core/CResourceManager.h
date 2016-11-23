#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <list>
#include <string>

#include "resource/IResourceManager.h"

#include "SImage.h"
#include "SMaterial.h"
#include "SMesh.h"
#include "SModel.h"
#include "SShader.h"

/**
* \brief Resource manager implementation.
*/
class CResourceManager : public IResourceManager
{
public:
  CResourceManager();
  ~CResourceManager();

  ResourceId createMesh(const std::vector<float> &vertices,
                        const std::vector<unsigned int> &indices,
                        const std::vector<float> &normals,
                        const std::vector<float> &uvs, EPrimitiveType type);

  ResourceId loadMesh(const std::string &file);

  bool getMesh(ResourceId id, std::vector<float> &vertices,
               std::vector<unsigned int> &indices, std::vector<float> &normals,
               std::vector<float> &uvs, EPrimitiveType &type) const;

  ResourceId createImage(const std::vector<unsigned char> &imageData,
                         unsigned int width, unsigned int height,
                         EColorFormat format);

  ResourceId loadImage(const std::string &file, EColorFormat format);

  bool getImage(ResourceId id, std::vector<unsigned char> &data,
                unsigned int &width, unsigned int &height,
                EColorFormat &format) const;

  ResourceId createMaterial(ResourceId base, ResourceId normal,
                            ResourceId specular, ResourceId glow,
                            ResourceId alpha);

  ResourceId loadMaterial(const std::string &file);

  bool getMaterial(ResourceId id, ResourceId &base, ResourceId &normal,
                   ResourceId &specular, ResourceId &glow,
                   ResourceId &alpha) const;

  ResourceId createModel(ResourceId mesh, ResourceId material);

  ResourceId loadModel(const std::string &file);

  bool getModel(ResourceId id, ResourceId &mesh, ResourceId &material);

  ResourceId createString(const std::string &text);

  ResourceId loadString(const std::string &file);
  ResourceId loadString(const std::string &file, bool doPreprocessing);

  bool getString(ResourceId id, std::string &text) const;

  ResourceId createShader(ResourceId vertex, ResourceId tessCtrl,
                          ResourceId tessEval, ResourceId geometry,
                          ResourceId fragment);

  ResourceId loadShader(const std::string &file);

  bool getShader(ResourceId id, ResourceId &vertex, ResourceId &tessCtrl,
                 ResourceId &tessEval, ResourceId &geometry,
                 ResourceId &fragment) const;

  void addResourceListener(IResourceListener *listener);
  void removeResourceListener(IResourceListener *listener);

protected:
  void notifyResourceListeners(EResourceType type, ResourceId id,
                               EListenerEvent event);

private:
  ResourceId m_nextMeshId = 0;     /**< Next free mesh id. */
  ResourceId m_nextImageId = 0;    /**< Next free image id. */
  ResourceId m_nextMaterialId = 0; /**< Next free material id. */
  ResourceId m_nextModelId = 0;    /**< Next free model id. */
  ResourceId m_nextStringId = 0;   /**< Next free string id. */
  ResourceId m_nextShaderId = 0;   /**< Next free shader id. */

  // TODO Change to vector?
  std::unordered_map<ResourceId, SMesh> m_meshes;  /**< Loaded meshes. */
  std::unordered_map<ResourceId, SImage> m_images; /**< Loaded images. */
  std::unordered_map<ResourceId, SMaterial>
      m_materials;                                 /**< Loaded materials. */
  std::unordered_map<ResourceId, SModel> m_models; /**< Loaded models. */
  std::unordered_map<ResourceId, std::string> m_strings; /**< Loaded strings. */
  std::unordered_map<ResourceId, SShader> m_shaders;     /**< Loaded shaders. */

  std::unordered_map<std::string, ResourceId>
      m_meshFiles; /**< Maps mesh file to mesh resource id. */
  std::unordered_map<std::string, ResourceId>
      m_imageFiles; /**< Maps image file to image resource id. */
  std::unordered_map<std::string, ResourceId>
      m_materialFiles; /**< Maps material file to material resource id. */
  std::unordered_map<std::string, ResourceId>
      m_modelFiles; /**< Maps model file to model resource id. */
  std::unordered_map<std::string, ResourceId>
      m_textFiles; /**< Maps text file to string resource id. */
  std::unordered_map<std::string, ResourceId>
      m_shaderFiles; /**< Maps shader program file to shader resource id. */

  std::list<IResourceListener *>
      m_resourceListeners; /**< Registered listeners. */
};
