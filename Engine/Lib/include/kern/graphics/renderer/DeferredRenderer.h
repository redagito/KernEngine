#pragma once

#include <list>
#include <memory>

#include "kern/foundation/Transformer.h"

#include "kern/graphics/IRenderer.h"
#include "kern/graphics/renderer/FrameBuffer.h"
#include "kern/graphics/renderer/RenderRequest.h"
#include "kern/graphics/renderer/pass/ScreenQuadPass.h"

#include "kern/graphics/resource/ResourceConfig.h"

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

    void draw(const IScene &scene, const ICamera &camera, const IWindow &window,
              const IGraphicsResourceManager &manager);

    static DeferredRenderer *create(IResourceManager &manager);

   protected:
    /**
     * \brief Writes geometry data into g-buffer.
     */
    void geometryPass(const IScene &scene, const ICamera &camera, const IWindow &window,
                      const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Performs shadow map calculation.
     */
    void shadowMapPass(const IScene &scene, const ICamera &camera, const IWindow &window,
                       const IGraphicsResourceManager &manager);

    /**
     * \brief Performs shadow cube calculation.
     */
    void shadowCubePass(const IScene &scene, const ICamera &camera, const IWindow &window,
                        const IGraphicsResourceManager &manager);

    /**
     * \brief Writes light data into l-buffer.
     */
    void lightPass(const IScene &scene, const ICamera &camera, const IWindow &window,
                   const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Writes point light data to l-buffer.
     */
    void pointLightPass(const IScene &scene, const ICamera &camera, const IWindow &window,
                        const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Writes directional light data to l-buffer.
     */
    void directionalLightPass(const IScene &scene, const ICamera &camera, const IWindow &window,
                              const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Performs scene illumination and tone mapping.
     */
    void illuminationPass(const IScene &scene, const ICamera &camera, const IWindow &window,
                          const IGraphicsResourceManager &manager, ISceneQuery &query);

    /**
     * \brief Performs post processing of lit scene.
     */
    void postProcessPass(const ICamera &camera, const IWindow &window,
                         const IGraphicsResourceManager &manager,
                         const std::shared_ptr<Texture> &texture);

    void fogPass(const ICamera &camera, const IWindow &, const IGraphicsResourceManager &manager,
                 const std::shared_ptr<Texture> &texture);

    /**
     * \brief FXAA Pass.
     */
    void fxaaPass(const IWindow &window, const IGraphicsResourceManager &manager,
                  const std::shared_ptr<Texture> &texture);

    /**
     * \brief Vertical gauss blur pass.
     */
    void gaussBlurVerticalPass(const IWindow &window, const IGraphicsResourceManager &manager,
                               const std::shared_ptr<Texture> &texture);

    /**
     * \brief Horizontal gauss blur pass.
     */
    void gaussBlurHorizontalPass(const IWindow &window, const IGraphicsResourceManager &manager,
                                 const std::shared_ptr<Texture> &texture);

    void depthOfFieldPass(const ICamera &camera, const IWindow &window,
                          const IGraphicsResourceManager &manager,
                          const std::shared_ptr<Texture> &sceneTexture,
                          const std::shared_ptr<Texture> &blurTexture);

    /**
     * \brief Draws scene texture to main FBO.
     */
    void displayPass(const IWindow &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture);

    void passthroughPass(const IWindow &window, const IGraphicsResourceManager &manager,
                         const std::shared_ptr<Texture> &texture);

    void godRayPass1(const IWindow &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture);

    void godRayPass2(const IWindow &window, const IGraphicsResourceManager &manager,
                     const std::shared_ptr<Texture> &texture,
                     const std::shared_ptr<Texture> &godrayTexture);

    /**
     * \brief Depth visualization pass.
     *
     * For debugging.
     */
    void visualizeDepthPass(const ICamera &camera, const IWindow &window,
                            const IGraphicsResourceManager &manager);

    /**
     * \brief Vignette blur pass.
     */
    void vignetteBlurPass(const IWindow &window, const IGraphicsResourceManager &manager,
                          const std::shared_ptr<Texture> &texture);

    /**
     * \brief Bloom pass 1, writes blooom texture.
     */
    void bloomPass1(const IWindow &window, const IGraphicsResourceManager &manager,
                    const std::shared_ptr<Texture> &texture);

    /**
     * \brief Bloom pass 2, blends bloom texture on scene texture.
     */
    void bloomPass2(const IWindow &window, const IGraphicsResourceManager &manager,
                    const std::shared_ptr<Texture> &texture,
                    const std::shared_ptr<Texture> &bloomTexture);

    /**
     * \brief Lens flare pass, blends lens flare texture on scene texture.
     */
    void lensFlarePass(const IWindow &window, const IGraphicsResourceManager &manager,
                       const std::shared_ptr<Texture> &texture);

    void lensFlarePass2(const IWindow &window, const IGraphicsResourceManager &manager,
                        const std::shared_ptr<Texture> &texture);

    void lensFlarePass3(const IWindow &window, const IGraphicsResourceManager &manager,
                        const std::shared_ptr<Texture> &texture,
                        const std::shared_ptr<Texture> &lensTexture);

    /**
     * \brief Cel pass, blends Cel texture on scene texture.
     */
    void celPass(const IWindow &window, const IGraphicsResourceManager &manager,
                 const std::shared_ptr<Texture> &texture);

    /**
     * \brief Tone map pass for HDR to LDR conversion.
     *
     * This pass should be done at the end of the post porcessing pipeline.
     */
    void toneMapPass(const IWindow &window, const IGraphicsResourceManager &manager,
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
    ResourceId m_geometryPassShaderId = invalidResource;

    // Shadow map pass
    ResourceId m_shadowMapPassShaderId = invalidResource;
    FrameBuffer m_shadowMapBuffer;
    std::shared_ptr<Texture> m_shadowDepthTexture = nullptr;

    // Shadow cube pass
    ResourceId m_shadowCubePassShaderId = invalidResource;
    FrameBuffer m_shadowCubeBuffer;
    std::shared_ptr<Texture> m_shadowCubeDepthTexture = nullptr;
    std::shared_ptr<Texture> m_shadowCubeTexture = nullptr;

    // Light pass common resources
    // TODO Put into light pass class
    FrameBuffer m_lightPassFrameBuffer; /**< Stores lit scene and depth. */
    std::shared_ptr<Texture> m_lightPassTexture = nullptr;

    // Point light pass
    ResourceId m_pointLightPassShaderId = invalidResource;
    ResourceId m_pointLightSphereId = invalidResource;

    // Directional light pass
    ResourceId m_directionalLightPassShaderId = invalidResource;
    ResourceId m_directionalLightScreenQuadId = invalidResource;

    // Illumination pass
    ResourceId m_illuminationPassShaderId = invalidResource;
    ResourceId m_illuminationPassScreenQuadId = invalidResource;
    FrameBuffer m_illumationPassFrameBuffer;
    std::shared_ptr<Texture> m_illuminationPassTexture = nullptr;

    // Post processing pass
    ResourceId m_postProcessScreenQuadId = invalidResource;
    FrameBuffer m_postProcessPassFrameBuffer0;
    FrameBuffer m_postProcessPassFrameBuffer1;
    FrameBuffer m_postProcessPassFrameBuffer2;

    std::shared_ptr<Texture> m_postProcessPassTexture0 = nullptr;
    std::shared_ptr<Texture> m_postProcessPassTexture1 = nullptr;
    std::shared_ptr<Texture> m_postProcessPassTexture2 = nullptr;
    std::shared_ptr<Texture> m_postProcessPassOutputTexture = nullptr;

    // Gauss blur pass
    ResourceId m_gaussBlurVerticalShaderId = invalidResource;
    ResourceId m_gaussBlurHorizontalShaderId = invalidResource;

    // FXAA pass
    ResourceId m_fxaaPassShaderId = invalidResource;

    // Fog pass
    ResourceId m_fogPassShaderId = invalidResource;

    // Depth-of-field pass
    ResourceId m_depthOfFieldPassShaderId = invalidResource;

    // Godray pass
    ResourceId m_godRayPass1ShaderId = invalidResource;
    ResourceId m_godRayPass2ShaderId = invalidResource;

    // Display pass for final screen draw
    ResourceId m_displayPassShaderId = invalidResource;

    // Depth visualization pass
    ResourceId m_visualizeDepthPassShaderId = invalidResource;

    // Vignette blur pass
    ResourceId m_vignetteBlurPassShaderId = invalidResource;

    // Bloom pass
    ResourceId m_bloomPass1ShaderId = invalidResource;
    ResourceId m_bloomPass2ShaderId = invalidResource;

    // Lens flare Pass
    ResourceId m_lensFlarePassShaderId = invalidResource;
    ResourceId m_lensFlarePass2ShaderId = invalidResource;
    ResourceId m_lensFlarePass3ShaderId = invalidResource;

    // Cel Pass
    ResourceId m_celPassShaderId = invalidResource;

    // Tonemap pass
    ResourceId m_toneMapPassShaderId = invalidResource;

    // Fullscreen draw pass
    ScreenQuadPass m_screenQuadPass;

	// Render requests with custom shaders.
    std::list<RenderRequest> m_customShaderMeshes;
};
