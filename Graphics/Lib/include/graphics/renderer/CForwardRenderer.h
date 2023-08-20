#pragma once

#include <list>

#include "graphics/resource/ResourceConfig.h"

#include "graphics/IRenderer.h"
#include "graphics/renderer/SRenderRequest.h"

class CShaderProgram;
class IResourceManager;

/**
 * \brief Forward renderer.
 */
class CForwardRenderer : public IRenderer
{
   public:
    /**
     * \brief Default constructor.
     */
    CForwardRenderer();

    /**
     * \brief Internal resource cleanup.
     */
    ~CForwardRenderer();

    /**
     * \brief Draws whole scene.
     * Implements the draw call as specified by the renderer interface.
     * This function implements the highlevel drawing logic for the scene,
     * selects rendering targets and manages render passes.
     */
    void draw(const IScene &scene, const ICamera &camera, const IWindow &window,
              const IGraphicsResourceManager &manager);

    /**
     * \brief Performs resource aqucisition and initialization.
     */
    bool init(IResourceManager &manager);

    /**
     * \brief Creates and initializes forward renderer.
     */
    static CForwardRenderer *create(IResourceManager &manager);

   protected:
    /**
     * \brief Draws mesh to current rendering target.
     * Selects the rendering method (indexed, direct, etc.) based on the available
     * mesh data
     * and performs actual draw call.
     */
    void draw(CMesh *mesh, const glm::mat4 &translation, const glm::mat4 &rotation,
              const glm::mat4 &scale, CMaterial *material, const IGraphicsResourceManager &manager);

   private:
    bool initShaders(IResourceManager &manager);

    glm::mat4 m_currentView = glm::mat4(1.f);       /**< Stores the current view matrix. */
    glm::mat4 m_currentProjection = glm::mat4(1.f); /**< Stores the current projection matrix. */

    std::list<SRenderRequest> m_customShaderMeshes; /**< Render requests with custom shaders. */
    ResourceId m_forwardShaderId;                   /**< Forward shader resource id. */
    CShaderProgram *m_forwardShader = nullptr;      /**< Currently active shader object. */
};
