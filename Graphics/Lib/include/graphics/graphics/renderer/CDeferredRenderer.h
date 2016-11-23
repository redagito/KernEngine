#pragma once

#include <memory>
#include <list>

#include <foundation/math/CTransformer.h>

#include "graphics/graphics/IRenderer.h"
#include "graphics/graphics/renderer/CFrameBuffer.h"
#include "graphics/graphics/renderer/SRenderRequest.h"
#include "graphics/graphics/renderer/pass/CScreenQuadPass.h"

#include "graphics/resource/ResourceConfig.h"

class CShaderProgram;
class IResourceManager;
class ISceneQuery;

/**
* \brief Deferred renderer implementation.
*/
class CDeferredRenderer : public IRenderer
{
public:
  CDeferredRenderer();
  ~CDeferredRenderer();

  bool init(IResourceManager &manager);

  void draw(const IScene &scene, const ICamera &camera, const IWindow &window,
            const IGraphicsResourceManager &manager);

  static CDeferredRenderer *create(IResourceManager &manager);

protected:
  /**
  * \brief Writes geometry data into g-buffer.
  */
  void geometryPass(const IScene &scene, const ICamera &camera,
                    const IWindow &window,
                    const IGraphicsResourceManager &manager,
                    ISceneQuery &query);

  /**
  * \brief Performs shadow map calculation.
  */
  void shadowMapPass(const IScene &scene, const ICamera &camera,
                     const IWindow &window,
                     const IGraphicsResourceManager &manager);

  /**
   * \brief Performs shadow cube calculation.
   */
  void shadowCubePass(const IScene &scene, const ICamera &camera,
                      const IWindow &window,
                      const IGraphicsResourceManager &manager);

  /**
  * \brief Writes light data into l-buffer.
  */
  void lightPass(const IScene &scene, const ICamera &camera,
                 const IWindow &window, const IGraphicsResourceManager &manager,
                 ISceneQuery &query);

  /**
  * \brief Writes point light data to l-buffer.
  */
  void pointLightPass(const IScene &scene, const ICamera &camera,
                      const IWindow &window,
                      const IGraphicsResourceManager &manager,
                      ISceneQuery &query);

  /**
  * \brief Writes directional light data to l-buffer.
  */
  void directionalLightPass(const IScene &scene, const ICamera &camera,
                            const IWindow &window,
                            const IGraphicsResourceManager &manager,
                            ISceneQuery &query);

  /**
  * \brief Performs scene illumination and tone mapping.
  */
  void illuminationPass(const IScene &scene, const ICamera &camera,
                        const IWindow &window,
                        const IGraphicsResourceManager &manager,
                        ISceneQuery &query);

  /**
  * \brief Performs post processing of lit scene.
  */
  void postProcessPass(const ICamera &camera, const IWindow &window,
                       const IGraphicsResourceManager &manager,
                       const std::shared_ptr<CTexture> &texture);

  void fogPass(const ICamera &camera, const IWindow &,
               const IGraphicsResourceManager &manager,
               const std::shared_ptr<CTexture> &texture);

  /**
  * \brief FXAA Pass.
  */
  void fxaaPass(const IWindow &window, const IGraphicsResourceManager &manager,
                const std::shared_ptr<CTexture> &texture);

  /**
  * \brief Vertical gauss blur pass.
  */
  void gaussBlurVerticalPass(const IWindow &window,
                             const IGraphicsResourceManager &manager,
                             const std::shared_ptr<CTexture> &texture);

  /**
  * \brief Horizontal gauss blur pass.
  */
  void gaussBlurHorizontalPass(const IWindow &window,
                               const IGraphicsResourceManager &manager,
                               const std::shared_ptr<CTexture> &texture);

  void depthOfFieldPass(const ICamera &camera, const IWindow &window,
                        const IGraphicsResourceManager &manager,
                        const std::shared_ptr<CTexture> &sceneTexture,
                        const std::shared_ptr<CTexture> &blurTexture);

  /**
  * \brief Draws scene texture to main FBO.
  */
  void displayPass(const IWindow &window,
                   const IGraphicsResourceManager &manager,
                   const std::shared_ptr<CTexture> &texture);

  void passthroughPass(const IWindow &window,
                       const IGraphicsResourceManager &manager,
                       const std::shared_ptr<CTexture> &texture);

  void godRayPass1(const IWindow &window,
                   const IGraphicsResourceManager &manager,
                   const std::shared_ptr<CTexture> &texture);

  void godRayPass2(const IWindow &window,
                   const IGraphicsResourceManager &manager,
                   const std::shared_ptr<CTexture> &texture,
                   const std::shared_ptr<CTexture> &godrayTexture);

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
  void vignetteBlurPass(const IWindow &window,
                        const IGraphicsResourceManager &manager,
                        const std::shared_ptr<CTexture> &texture);

  /**
  * \brief Bloom pass 1, writes blooom texture.
  */
  void bloomPass1(const IWindow &window,
                  const IGraphicsResourceManager &manager,
                  const std::shared_ptr<CTexture> &texture);

  /**
  * \brief Bloom pass 2, blends bloom texture on scene texture.
  */
  void bloomPass2(const IWindow &window,
                  const IGraphicsResourceManager &manager,
                  const std::shared_ptr<CTexture> &texture,
                  const std::shared_ptr<CTexture> &bloomTexture);

  /**
  * \brief Lens flare pass, blends lens flare texture on scene texture.
  */
  void lensFlarePass(const IWindow &window,
                     const IGraphicsResourceManager &manager,
                     const std::shared_ptr<CTexture> &texture);

  void lensFlarePass2(const IWindow &window,
                      const IGraphicsResourceManager &manager,
                      const std::shared_ptr<CTexture> &texture);

  void lensFlarePass3(const IWindow &window,
                      const IGraphicsResourceManager &manager,
                      const std::shared_ptr<CTexture> &texture,
                      const std::shared_ptr<CTexture> &lensTexture);

  /**
  * \brief Cel pass, blends Cel texture on scene texture.
  */
  void celPass(const IWindow &window, const IGraphicsResourceManager &manager,
               const std::shared_ptr<CTexture> &texture);

  /**
  * \brief Tone map pass for HDR to LDR conversion.
  *
  * This pass should be done at the end of the post porcessing pipeline.
  */
  void toneMapPass(const IWindow &window,
                   const IGraphicsResourceManager &manager,
                   const std::shared_ptr<CTexture> &texture);

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

  void draw(CMesh *mesh, const glm::mat4 &translation,
            const glm::mat4 &rotation, const glm::mat4 &scale,
            CMaterial *material, const IGraphicsResourceManager &manager,
            CShaderProgram *shader);

private:
  CTransformer m_transformer; /**< Stores current transformation matrices. */

  // Geometry pass
  // TODO Put into geometry pass class
  CFrameBuffer m_geometryBuffer; /**< GBuffer. */
  std::shared_ptr<CTexture> m_depthTexture =
      nullptr; /**< Depth texture attachment. */
  std::shared_ptr<CTexture> m_diffuseGlowTexture =
      nullptr; /**< Diffuse texture with glow as alpha. */
  std::shared_ptr<CTexture>
      m_normalSpecularTexture; /**< Normal texture with specularity as alpha. */
  ResourceId m_geometryPassShaderId = invalidResource;

  // Shadow map pass
  ResourceId m_shadowMapPassShaderId = invalidResource;
  CFrameBuffer m_shadowMapBuffer;
  std::shared_ptr<CTexture> m_shadowDepthTexture = nullptr;

  // Shadow cube pass
  ResourceId m_shadowCubePassShaderId = invalidResource;
  CFrameBuffer m_shadowCubeBuffer;
  std::shared_ptr<CTexture> m_shadowCubeDepthTexture = nullptr;
  std::shared_ptr<CTexture> m_shadowCubeTexture = nullptr;

  // Light pass common resources
  // TODO Put into light pass class
  CFrameBuffer m_lightPassFrameBuffer; /**< Stores lit scene and depth. */
  std::shared_ptr<CTexture> m_lightPassTexture = nullptr;

  // Point light pass
  ResourceId m_pointLightPassShaderId = invalidResource;
  ResourceId m_pointLightSphereId = invalidResource;

  // Directional light pass
  ResourceId m_directionalLightPassShaderId = invalidResource;
  ResourceId m_directionalLightScreenQuadId = invalidResource;

  // Illumination pass
  ResourceId m_illuminationPassShaderId = invalidResource;
  ResourceId m_illuminationPassScreenQuadId = invalidResource;
  CFrameBuffer m_illumationPassFrameBuffer;
  std::shared_ptr<CTexture> m_illuminationPassTexture = nullptr;

  // Post processing pass
  ResourceId m_postProcessScreenQuadId = invalidResource;
  CFrameBuffer m_postProcessPassFrameBuffer0;
  CFrameBuffer m_postProcessPassFrameBuffer1;
  CFrameBuffer m_postProcessPassFrameBuffer2;

  std::shared_ptr<CTexture> m_postProcessPassTexture0 = nullptr;
  std::shared_ptr<CTexture> m_postProcessPassTexture1 = nullptr;
  std::shared_ptr<CTexture> m_postProcessPassTexture2 = nullptr;
  std::shared_ptr<CTexture> m_postProcessPassOutputTexture = nullptr;

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
  CScreenQuadPass m_screenQuadPass;
  std::list<SRenderRequest>
      m_customShaderMeshes; /**< Render requests with custom shaders. */
};
