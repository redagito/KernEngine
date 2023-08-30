#pragma once

#include <algorithm>
#include <list>
#include <memory>
#include <unordered_map>

#include "kern/graphics/IGraphicsResourceManager.h"
#include "kern/graphics/resource/IResourceListener.h"

#include "kern/graphics/resource/Material.h"
#include "kern/graphics/resource/Mesh.h"
#include "kern/graphics/resource/Model.h"
#include "kern/graphics/resource/ShaderProgram.h"
#include "kern/graphics/resource/Texture.h"
#include "kern/graphics/resource/TShaderObject.h"

class GraphicsResourceManager : public IGraphicsResourceManager, public IResourceListener
{
   public:
    /**
     * \brief Sets resource callbacks.
     */
    GraphicsResourceManager();

    /**
     * \brief Cleans up resource callbacks.
     */
    ~GraphicsResourceManager();

    /**
     * \brief On attach callback.
     * Implements the attach callback for the resource listener interface.
     * TODO Currently stubbed, renderer attaches itself to resource manager in
     * constructor.
     */
    void onAttach(IResourceManager *resourceManager);

    /**
     * \brief On detach callback.
     * Implements the detach callback for the resource listener interface.
     * TODO Currently stubbed.
     */
    void onDetach(IResourceManager *resourceManager);

    /**
     * \brief Notify callback.
     * Implements the notify callback for the resource listener interface.
     * The function resolves the resource type and forwards the call to specialized
     * functions.
     */
    void notify(EResourceType type, ResourceId, EListenerEvent event,
                IResourceManager *resourceManager);

    /**
     * \brief Maps id to internal mesh object.
     */
    Mesh *getMesh(ResourceId) const;

    /**
     * \brief Maps id to internal material object.
     */
    Material *getMaterial(ResourceId) const;

    /**
     * \brief Maps id to internal model object.
     */
    Model *getModel(ResourceId) const;

    /**
     * \brief Maps id to internal texture object.
     */
    Texture *getTexture(ResourceId) const;

    /**
     * \brief Maps id to internal shader program object.
     */
    ShaderProgram *getShaderProgram(ResourceId) const;

    /**
     * \brief Returns respective default texture.
     */
    Texture *getDefaultDiffuseTexture() const;
    Texture *getDefaultNormalTexture() const;
    Texture *getDefaultSpecularTexture() const;
    Texture *getDefaultGlowTexture() const;
    Texture *getDefaultAlphaTexture() const;

   protected:
    /**
     * \brief Maps id to internal vertex shader object.
     */
    TShaderObject<GL_VERTEX_SHADER> *getVertexShaderObject(ResourceId) const;

    /**
     * \brief Maps id to internal tessellation control shader object.
     */
    TShaderObject<GL_TESS_CONTROL_SHADER> *getTessControlShaderObject(ResourceId) const;

    /**
     * \brief Maps id to internal tessellation evaluation shader object.
     */
    TShaderObject<GL_TESS_EVALUATION_SHADER> *getTessEvalShaderObject(ResourceId) const;

    /**
     * \brief Maps id to internal geometry shader object.
     */
    TShaderObject<GL_GEOMETRY_SHADER> *getGeometryShaderObject(ResourceId) const;

    /**
     * \brief Maps id to internal fragment shader object.
     */
    TShaderObject<GL_FRAGMENT_SHADER> *getFragmentShaderObject(ResourceId) const;

   private:
    /**
     * \brief Initializes default textures for materials.
     */
    void initDefaultTextures();

    /**
     * \brief Loads vertex shader from resource manager.
     */
    bool loadVertexShader(ResourceId id, IResourceManager *resourceManager);

    /**
     * \brief Loads tessellation control shader from resource manager.
     */
    bool loadTessControlShader(ResourceId id, IResourceManager *resourceManager);

    /**
     * \brief Loads tessellation evaluation shader from resource manager.
     */
    bool loadTessEvalShader(ResourceId id, IResourceManager *resourceManager);

    /**
     * \brief Loads geometry shader from resource manager.
     */
    bool loadGeometryShader(ResourceId id, IResourceManager *resourceManager);

    /**
     * \brief Loads fragment shader from resource manager.
     */
    bool loadFragmentShader(ResourceId id, IResourceManager *resourceManager);

    /**
     * \brief Handles resource events for image resources.
     */
    void handleImageEvent(ResourceId, EListenerEvent event, IResourceManager *resourceManager);

    /**
     * \brief Handles resource events for mesh resources.
     */
    void handleMeshEvent(ResourceId, EListenerEvent event, IResourceManager *resourceManager);

    /**
     * \brief Handles resource events for material resources.
     */
    void handleMaterialEvent(ResourceId, EListenerEvent event, IResourceManager *resourceManager);

    /**
     * \brief Handles resource events for model resource.
     */
    void handleModelEvent(ResourceId, EListenerEvent event, IResourceManager *resourceManager);

    /**
     * \brief Handles resource events for shader resources.
     */
    void handleShaderEvent(ResourceId, EListenerEvent event, IResourceManager *resourceManager);

    /**
     * \brief Handles resource events for string resources.
     * Stubbed as the renderer does not need string events.
     * TODO Consider hot reloading of shader source code and on-the-fly
     * recompiling of shader objects/programs
     */
    void handleStringEvent(ResourceId, EListenerEvent event, IResourceManager *resourceManager);

    std::unordered_map<ResourceId, std::unique_ptr<Mesh>>
        m_meshes; /**< Maps mesh id from resource manager to GPU side mesh. */

    std::unordered_map<ResourceId, std::unique_ptr<Texture>> m_textures; /**< Maps image id
                                                                             from resource
                                                                             manager to GPU
                                                                             side texture. */

    std::unordered_map<ResourceId, std::unique_ptr<Material>>
        m_materials; /**< Maps material id from resource manager to cached
                        material. */

    std::unordered_map<ResourceId, std::unique_ptr<Model>>
        m_models; /**< Maps model id from resource manager to cached model. */

    std::unordered_map<ResourceId, std::unique_ptr<TShaderObject<GL_VERTEX_SHADER>>>
        m_vertexShader; /**< Maps string resource ids to compiled vertex shader
                           objects. */

    std::unordered_map<ResourceId, std::unique_ptr<TShaderObject<GL_TESS_CONTROL_SHADER>>>
        m_tessConstrolShader; /**< Maps string resource ids to compiled
                                 tessellation control
                                                  shader objects. */

    std::unordered_map<ResourceId, std::unique_ptr<TShaderObject<GL_TESS_EVALUATION_SHADER>>>
        m_tessEvalShader; /**< Maps string resource ids to compiled tessellation
                             evaluation shader
                                          objects. */

    std::unordered_map<ResourceId, std::unique_ptr<TShaderObject<GL_GEOMETRY_SHADER>>>
        m_geometryShader; /**< Maps string resource ids to compiled geometry
                             shader objects. */

    std::unordered_map<ResourceId, std::unique_ptr<TShaderObject<GL_FRAGMENT_SHADER>>>
        m_fragmentShader; /**< Maps string resource ids to compiled fragment
                             shader objects. */

    std::unordered_map<ResourceId, std::unique_ptr<ShaderProgram>>
        m_shaderPrograms; /**< Maps resource ids to linked shader programs. */

    std::unique_ptr<Texture> m_defaultDiffuseTexture = nullptr;  /**< Default diffuse texture. */
    std::unique_ptr<Texture> m_defaultNormalTexture = nullptr;   /**< Default normal texture. */
    std::unique_ptr<Texture> m_defaultSpecularTexture = nullptr; /**< Default specular texture. */
    std::unique_ptr<Texture> m_defaultGlowTexture = nullptr;     /**< Default glow texture. */
    std::unique_ptr<Texture> m_defaultAlphaTexture = nullptr;    /**< Default alpha texture. */

    std::list<IResourceManager *> m_registeredManagers; /**< Resource managers,
                                                           this listener is
                                                           attached to. */
};
