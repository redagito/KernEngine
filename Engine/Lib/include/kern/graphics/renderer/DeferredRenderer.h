#pragma once

#include <list>
#include <memory>

#include "kern/foundation/Transformer.h"

#include "kern/graphics/IRenderer.h"
#include "kern/graphics/renderer/FrameBuffer.h"
#include "kern/graphics/renderer/RenderRequest.h"
#include "kern/graphics/renderer/pass/ScreenQuadPass.h"

#include "kern/resource/ResourceId.h"

class ShaderProgram;
class IResourceManager;
class ISceneQuery;

/**
 * \brief Deferred renderer implementation.
 */
class DeferredRenderer : public IRenderer
{
   public:
    DeferredRenderer();
    ~DeferredRenderer();

    bool init(IResourceManager &manager);

    void draw(const IScene &scene, const ICamera &camera, const Window &window,
              const IGraphicsResourceManager &manager);

    static DeferredRenderer *create(IResourceManager &manager);

   protected:
    /**
     * \brief Writes geometry data into g-buffer.
     */
    void geometryPass(const IScene &scene, const ICamera &camera, const Window &window,
                      const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Performs shadow map calculation.
     */
    void shadowMapPass(const IScene &scene, const ICamera &camera, const Window &window,
                       const IGraphicsResourceManager &manager);

    /**
     * \brief Performs shadow cube calculation.
     */
    void shadowCubePass(const IScene &scene, const ICamera &camera, const Window &window,
                        const IGraphicsResourceManager &manager);

    /**
     * \brief Writes light data into l-buffer.
     */
    void lightPass(const IScene &scene, const ICamera &camera, const Window &window,
                   const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Writes point light data to l-buffer.
     */
    void pointLightPass(const IScene &scene, const ICamera &camera, const Window &window,
                        const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Writes directional light data to l-buffer.
     */
    void directionalLightPass(const IScene &scene, const ICamera &camera, const Window &window,
                              const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Performs scene illumination and tone mapping.
     */
    void illuminationPass(const IScene &scene, const ICamera &camera, const Window &window,
                          const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Performs post processing of lit scene.
     */
    void postProcessPass(const ICamera &camera, const Window &window,
                         const IGraphicsResourceManager &manager,
                         const std::shared_ptr<Texture> &texture);

    void fogPass(const ICamera &camera, const Window &, const IGraphicsResourceManager &manager,
                 const std::shared_ptr<Texture> &texture);

    /**
     * \brief FXAA Pass.
     */
    void fxaaPass(const Window &window, const IGraphicsResourceManager &manager,
                  const std::shared_ptr<Texture> &texture);

    /**
     * \brief Vertical gauss blur pass.
     */
    void gaussBlurVerticalPass(const Window &window, const IGraphicsResourceManager &manager,
                               const std::shared_ptr<Texture> &texture);

    /**
     * \brief Horizontal gauss blur pass.
     */
    void gaussBlurHorizontalPass(const Window &window, const IGraphicsResourceManager &manager,
                                 const std::shared_ptr<Texture> &texture);

    void depthOfFieldPass(const ICamera &camera, const Window &window,
                          const IGraphicsResourceManager &manager,
                          const std::shared_ptr<Texture> &sceneTexture,
                          const std::shared_ptr<Texture> &blurTexture);

    /**
     * \brief Draws scene texture to main FBO.
     */
    void displayPass(const Window &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture);

    void passthroughPass(const Window &window, const IGraphicsResourceManager &manager,
                         const std::shared_ptr<Texture> &texture);

    void godRayPass1(const Window &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture);

    void godRayPass2(const Window &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture,
                     const std::shared_ptr<Texture> &godrayTexture);

    /**
     * \brief Depth visualization pass.
     *
     * For debugging.
     */
    void visualizeDepthPass(const ICamera &camera, const Window &window,
                            const IGraphicsResourceManager &manager);

    /**
     * \brief Vignette blur pass.
     */
    void vignetteBlurPass(const Window &window, const IGraphicsResourceManager &manager,
                          const std::shared_ptr<Texture> &texture);

    /**
     * \brief Bloom pass 1, writes blooom texture.
     */
    void bloomPass1(const Window &window, const IGraphicsResourceManager &manager,
                    const std::shared_ptr<Texture> &texture);

    /**
     * \brief Bloom pass 2, blends bloom texture on scene texture.
     */
    void bloomPass2(const Window &window, const IGraphicsResourceManager &manager,
                    const std::shared_ptr<Texture> &texture,
                    const std::shared_ptr<Texture> &bloomTexture);

    /**
     * \brief Lens flare pass, blends lens flare texture on scene texture.
     */
    void lensFlarePass(const Window &window, const IGraphicsResourceManager &manager,
                       const std::shared_ptr<Texture> &texture);

    void lensFlarePass2(const Window &window, const IGraphicsResourceManager &manager,
                        const std::shared_ptr<Texture> &texture);

    void lensFlarePass3(const Window &window, const IGraphicsResourceManager &manager,
                        const std::shared_ptr<Texture> &texture,
                        const std::shared_ptr<Texture> &lensTexture);

    /**
     * \brief Cel pass, blends Cel texture on scene texture.
     */
    void celPass(const Window &window, const IGraphicsResourceManager &manager,
                 const std::shared_ptr<Texture> &texture);

    /**
     * \brief Tone map pass for HDR to LDR conversion.
     *
     * This pass should be done at the end of the post porcessing pipeline.
     */
    void toneMapPass(const Window &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture);

    /**
     * \brief Initializes resources for geometry pass.
     */
    bool initGeometryPass(IResourceManager &manager);

    /**
     * \brief Initializes resources for point light pass.
     */
    bool initPointLightPass(IResourceManager &manager);

    /**
     * \brief Initializes resources for directional light pass.
     */
    bool initDirectionalLightPass(IResourceManager &manager);

    /**
     * \brief Initializes resources for illumination pass.
     */
    bool initIlluminationPass(IResourceManager &manager);

    /**
     * \brief Initializes resources for shadow map pass
     */
    bool initShadowMapPass(IResourceManager &manager);

    /**
     * \brief Initializes resources for shadow map pass
     */
    bool initShadowCubePass(IResourceManager &manager);

    /**
     * \brief Initializes post processing pass.
     */
    bool initPostProcessPass(IResourceManager &manager);

    /**
     * \brief Initializes post process pass for depth of field.
     *
     * Called by initPostProcesPass.
     */
    bool initDepthOfFieldPass(IResourceManager &manager);

    /**
     * \brief Initializes vertical gauss blur pass.
     */
    bool initGaussBlurVerticalPass(IResourceManager &manager);

    /**
     * \brief Initializes horizontal gauss blur pass.
     */
    bool initGaussBlurHorizontalPass(IResourceManager &manager);

    /**
     * \brief Initializes FXAA pass for post processing.
     */
    bool initFxaaPass(IResourceManager &manager);

    /**
     * \brief Initializes fog pass for post processing.
     */
    bool initFogPass(IResourceManager &manager);

    bool initGodRayPass1(IResourceManager &manager);
    bool initGodRayPass2(IResourceManager &manager);

    /**
     * \brief Display pass draws final image to screen.
     */
    bool initDisplayPass(IResourceManager &manager);

    bool initVisualizeDepthPass(IResourceManager &manager);

    /**
     * \brief Initializes distortion pass.
     */
    bool initVignetteBlurPass(IResourceManager &manager);

    /**
     * \brief Bloom technique.
     */
    bool initBloomPass1(IResourceManager &manager);
    bool initBloomPass2(IResourceManager &manager);

    /**
     * \brief Lens flare technique.
     */
    bool initLensFlarePass(IResourceManager &manager);

    /**
     * \brief Lens flare technique.
     */
    bool initLensFlarePass2(IResourceManager &manager);

    /**
     * \brief Lens flare technique.
     */
    bool initLensFlarePass3(IResourceManager &manager);

    /**
     * \brief Lens flare technique.
     */
    bool initCelPass(IResourceManager &manager);

    /**
     * \brief Tone mapping pass.
     */
    bool initToneMapPass(IResourceManager &manager);

    void draw(Mesh *mesh, const glm::mat4 &translation, const glm::mat4 &rotation,
              const glm::mat4 &scale, Material *material, const IGraphicsResourceManager &manager,
              ShaderProgram *shader);

   private:
    Transformer m_transformer; /**< Stores current transformation matrices. */

    // Geometry pass
    // TODO Put into geometry pass class
    FrameBuffer m_geometryBuffer;                      /**< GBuffer. */
    std::shared_ptr<Texture> m_depthTexture = nullptr; /**< Depth texture attachment. */
    std::shared_ptr<Texture> m_diffuseGlowTexture =
        nullptr; /**< Diffuse texture with glow as alpha. */
    std::shared_ptr<Texture>
        m_normalSpecularTexture; /**< Normal texture with specularity as alpha. */
    ResourceId m_geometryPassShaderId = InvalidResource;

    // Shadow map pass
    ResourceId m_shadowMapPassShaderId = InvalidResource;
    FrameBuffer m_shadowMapBuffer;
    std::shared_ptr<Texture> m_shadowDepthTexture = nullptr;

    // Shadow cube pass
    ResourceId m_shadowCubePassShaderId = InvalidResource;
    FrameBuffer m_shadowCubeBuffer;
    std::shared_ptr<Texture> m_shadowCubeDepthTexture = nullptr;
    std::shared_ptr<Texture> m_shadowCubeTexture = nullptr;

    // Light pass common resources
    // TODO Put into light pass class
    FrameBuffer m_lightPassFrameBuffer; /**< Stores lit scene and depth. */
    std::shared_ptr<Texture> m_lightPassTexture = nullptr;

    // Point light pass
    ResourceId m_pointLightPassShaderId = InvalidResource;
    ResourceId m_pointLightSphereId = InvalidResource;

    // Directional light pass
    ResourceId m_directionalLightPassShaderId = InvalidResource;
    ResourceId m_directionalLightScreenQuadId = InvalidResource;

    // Illumination pass
    ResourceId m_illuminationPassShaderId = InvalidResource;
    ResourceId m_illuminationPassScreenQuadId = InvalidResource;
    FrameBuffer m_illumationPassFrameBuffer;
    std::shared_ptr<Texture> m_illuminationPassTexture = nullptr;

    // Post processing pass
    ResourceId m_postProcessScreenQuadId = InvalidResource;
    FrameBuffer m_postProcessPassFrameBuffer0;
    FrameBuffer m_postProcessPassFrameBuffer1;
    FrameBuffer m_postProcessPassFrameBuffer2;

    std::shared_ptr<Texture> m_postProcessPassTexture0 = nullptr;
    std::shared_ptr<Texture> m_postProcessPassTexture1 = nullptr;
    std::shared_ptr<Texture> m_postProcessPassTexture2 = nullptr;
    std::shared_ptr<Texture> m_postProcessPassOutputTexture = nullptr;

    // Gauss blur pass
    ResourceId m_gaussBlurVerticalShaderId = InvalidResource;
    ResourceId m_gaussBlurHorizontalShaderId = InvalidResource;

    // FXAA pass
    ResourceId m_fxaaPassShaderId = InvalidResource;

    // Fog pass
    ResourceId m_fogPassShaderId = InvalidResource;

    // Depth-of-field pass
    ResourceId m_depthOfFieldPassShaderId = InvalidResource;

    // Godray pass
    ResourceId m_godRayPass1ShaderId = InvalidResource;
    ResourceId m_godRayPass2ShaderId = InvalidResource;

    // Display pass for final screen draw
    ResourceId m_displayPassShaderId = InvalidResource;

    // Depth visualization pass
    ResourceId m_visualizeDepthPassShaderId = InvalidResource;

    // Vignette blur pass
    ResourceId m_vignetteBlurPassShaderId = InvalidResource;

    // Bloom pass
    ResourceId m_bloomPass1ShaderId = InvalidResource;
    ResourceId m_bloomPass2ShaderId = InvalidResource;

    // Lens flare Pass
    ResourceId m_lensFlarePassShaderId = InvalidResource;
    ResourceId m_lensFlarePass2ShaderId = InvalidResource;
    ResourceId m_lensFlarePass3ShaderId = InvalidResource;

    // Cel Pass
    ResourceId m_celPassShaderId = InvalidResource;

    // Tonemap pass
    ResourceId m_toneMapPassShaderId = InvalidResource;

    // Fullscreen draw pass
    ScreenQuadPass m_screenQuadPass;

	// Render requests with custom shaders.
    std::list<RenderRequest> m_customShaderMeshes;
};
