#include "kern/graphics/renderer/DeferredRenderer.h"

#include <fmtlog/fmtlog.h>

#include <cassert>
#include <glm/ext.hpp>
#include <string>

#include "kern/graphics/ICamera.h"
#include "kern/graphics/IGraphicsResourceManager.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/Window.h"
#include "kern/graphics/camera/StaticCamera.h"
#include "kern/graphics/renderer/Draw.h"
#include "kern/graphics/renderer/RenderBuffer.h"
#include "kern/graphics/renderer/RendererCoreConfig.h"
#include "kern/graphics/resource/Material.h"
#include "kern/graphics/resource/Mesh.h"
#include "kern/graphics/resource/ShaderProgram.h"
#include "kern/graphics/resource/Texture.h"
#include "kern/graphics/scene/SceneQuery.h"
#include "kern/resource/IResourceManager.h"

DeferredRenderer::DeferredRenderer() { return; }

DeferredRenderer::~DeferredRenderer() { return; }

bool DeferredRenderer::init(IResourceManager &manager)
{
    logi("Initializing deferred renderer.");

    // Init geometry pass resources
    if (!initGeometryPass(manager))
    {
        loge("Failed to intialize geometry pass.");
        return false;
    }

    // Shadow mapping
    // TODO Should be done in directional light pass
    if (!initShadowMapPass(manager))
    {
        loge("Failed to intialize shadow map pass.");
        return false;
    }

    if (!initShadowCubePass(manager))
    {
        loge("Failed to intialize shadow map pass.");
        return false;
    }

    // Init light pass resources
    if (!initPointLightPass(manager))
    {
        loge("Failed to initialize point light pass.");
        return false;
    }

    // Init directional light pass
    if (!initDirectionalLightPass(manager))
    {
        loge("Failed to initialize directional light pass.");
        return false;
    }

    // Init illumination pass
    if (!initIlluminationPass(manager))
    {
        loge("Failed to initialize illumination pass.");
        return false;
    }

    // Post process
    // TODO Dedicated post processor?
    if (!initPostProcessPass(manager))
    {
        loge("Failed to initialize post processing pass.");
        return false;
    }

    // Display pass
    if (!initDisplayPass(manager))
    {
        loge("Failed to initialize display pass.");
        return false;
    }

    if (!m_screenQuadPass.init(manager))
    {
        loge("Failed to initialize screen quad pass.");
        return false;
    }

    if (!initVisualizeDepthPass(manager))
    {
        loge("Failed to initialize depth visualization pass.");
        return false;
    }

    return true;
}

void DeferredRenderer::draw(const IScene &scene, const ICamera &camera, const Window &window,
                            const IGraphicsResourceManager &manager)
{
    // Draw init
    window.setActive();

    // Query visible scene objects and lights
    SceneQuery query;
    scene.getVisibleObjects(camera, query);

    // Geometry pass fills gbuffer
    geometryPass(scene, camera, window, manager, query);

    // Light pass fills lbuffer
    lightPass(scene, camera, window, manager, query);

    // Illumination pass renders lit scene from lbuffer and gbuffer
    illuminationPass(scene, camera, window, manager, query);

    // Post processing pass
    postProcessPass(camera, window, manager, m_illuminationPassTexture);

    FrameBuffer::setDefaultActive();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    // Select rendering mode
    if (camera.getFeatureInfo().renderMode == RenderMode::Color)
    {
        displayPass(window, manager, m_diffuseGlowTexture);
    }
    else if (camera.getFeatureInfo().renderMode == RenderMode::Depth)
    {
        visualizeDepthPass(camera, window, manager);
    }
    else if (camera.getFeatureInfo().renderMode == RenderMode::Lights)
    {
        displayPass(window, manager, m_lightPassTexture);
    }
    else if (camera.getFeatureInfo().renderMode == RenderMode::Normals)
    {
        displayPass(window, manager, m_normalSpecularTexture);
    }
    else  // Final mode
    {
        // Final display pass
        displayPass(window, manager, m_postProcessPassOutputTexture);
    }
}

DeferredRenderer *DeferredRenderer::create(IResourceManager &manager)
{
    DeferredRenderer *renderer = new DeferredRenderer;
    if (!renderer->init(manager))
    {
        delete renderer;
        renderer = nullptr;
        loge("Failed to create deferred renderer, initialization failed.");
    }
    return renderer;
}

void DeferredRenderer::geometryPass(const IScene &scene, const ICamera &camera, const Window &window,
                                    const IGraphicsResourceManager &manager, ISceneQuery &query)
{
    // Set framebuffer
    m_geometryBuffer.setActive(GL_FRAMEBUFFER);
    // Clear buffer
    // TODO Should be retrieved as scene parameters
    // Diffuse, glow
    GLfloat diffuseGlow[] = {0.f, 0.f, 0.f, 0.f};
    GLfloat normalSpecular[] = {0.5f, 0.5f, 1.f, 0.f};
    glClearBufferfv(GL_COLOR, 0, diffuseGlow);
    // Normal, specular
    glClearBufferfv(GL_COLOR, 1, normalSpecular);

    // Clear
    glClear(GL_DEPTH_BUFFER_BIT);

    // Geometry pass, uses gbuffer fbo
    ShaderProgram *geometryPassShader = manager.getShaderProgram(m_geometryPassShaderId);
    if (geometryPassShader == nullptr)
    {
        loge("Failed to retrieve geometry pass shader.");
        return;
    }

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Backface culling disabled for debugging
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Winding order, standard is counter-clockwise
    glFrontFace(GL_CCW);

    // Reset viewport
    glViewport(0, 0, window.getWidth(), window.getHeight());
    m_geometryBuffer.resize(window.getWidth(), window.getHeight());

    // Set view and projection matrices
    m_transformer.setViewMatrix(camera.getView());
    m_transformer.setProjectionMatrix(camera.getProjection());

    // Send view/projection to default shader
    geometryPassShader->setUniform(viewMatrixUniformName, m_transformer.getViewMatrix());
    geometryPassShader->setUniform(projectionMatrixUniformName, m_transformer.getProjectionMatrix());

    // Traverse visible objects
    while (query.hasNextObject())
    {
        // Get next visible object
        SceneObjectId id = query.getNextObject();

        // Object attributes
        ResourceId meshId = -1;
        ResourceId materialId = -1;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        bool visible;

        // Retrieve object data
        if (!scene.getObject(id, meshId, materialId, position, rotation, scale, visible))
        {
            // Invalid id
            loge("Invalid scene object id {}.", id);
        }
        else
        {
            // Resolve ids
            Mesh *mesh = manager.getMesh(meshId);
            Material *material = manager.getMaterial(materialId);

            // Set transformations
            m_transformer.setPosition(position);
            m_transformer.setRotation(rotation);
            m_transformer.setScale(scale);

            // Forward draw call
            draw(mesh, m_transformer.getTranslationMatrix(), m_transformer.getRotationMatrix(),
                 m_transformer.getScaleMatrix(), material, manager, geometryPassShader);
        }
    }

    // Disable geometry buffer
    m_geometryBuffer.setInactive(GL_FRAMEBUFFER);
}

void DeferredRenderer::shadowMapPass(const IScene &scene, const ICamera &camera, const Window &window,
                                     const IGraphicsResourceManager &manager)
{
    ShaderProgram *shadowMapPassShader = manager.getShaderProgram(m_shadowMapPassShaderId);

    // Set framebuffer
    m_shadowMapBuffer.setActive(GL_FRAMEBUFFER);
    shadowMapPassShader->setActive();

    // Clear
    glClear(GL_DEPTH_BUFFER_BIT);

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Backface culling disabled for debugging
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Winding order, standard is counter-clockwise
    glFrontFace(GL_CCW);

    // Reset viewport
    glViewport(0, 0, 4096, 4096);
    m_shadowMapBuffer.resize(4096, 4096);

    // Stores active transformations
    Transformer transformer;

    // Set view and projection matrices
    transformer.setViewMatrix(camera.getView());
    transformer.setProjectionMatrix(camera.getProjection());

    // Query visible scene objects
    SceneQuery query;
    scene.getVisibleObjects(camera, query);

    // Send view/projection to default shader
    shadowMapPassShader->setUniform(viewMatrixUniformName, transformer.getViewMatrix());
    shadowMapPassShader->setUniform(projectionMatrixUniformName, transformer.getProjectionMatrix());

    // Traverse visible objects
    while (query.hasNextObject())
    {
        // Get next visible object
        SceneObjectId id = query.getNextObject();

        // Object attributes
        ResourceId meshId = -1;
        ResourceId materialId = -1;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        bool visible;

        // Retrieve object data
        if (!scene.getObject(id, meshId, materialId, position, rotation, scale, visible))
        {
            // Invalid id
            loge("Invalid scene object id {}.", id);
        }
        else
        {
            // Resolve ids
            Mesh *mesh = manager.getMesh(meshId);
            Material *material = manager.getMaterial(materialId);

            // Set transformations
            transformer.setPosition(position);
            transformer.setRotation(rotation);
            transformer.setScale(scale);

            // Forward draw call
            draw(mesh, transformer.getTranslationMatrix(), transformer.getRotationMatrix(),
                 transformer.getScaleMatrix(), material, manager, shadowMapPassShader);
        }
    }

    // Disable geometry buffer
    m_shadowMapBuffer.setInactive(GL_FRAMEBUFFER);

    glCullFace(GL_BACK);
}

struct CameraDirection
{
    GLenum cubemapFace;
    glm::vec3 target;
    glm::vec3 up;
};

CameraDirection g_cameraDirections[6] = {
    {GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
    {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
    {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
    {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
    {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
    {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)}};

void DeferredRenderer::shadowCubePass(const IScene &scene, const ICamera &camera, const Window &window,
                                      const IGraphicsResourceManager &manager)
{
    ShaderProgram *shadowCubePassShader = manager.getShaderProgram(m_shadowCubePassShaderId);
    shadowCubePassShader->setActive();

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Backface culling disabled for debugging
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glDisable(GL_BLEND);

    // Winding order, standard is counter-clockwise
    glFrontFace(GL_CCW);

    // Reset viewport
    glViewport(0, 0, 1024, 1024);
    // m_shadowMapBuffer.resize(1024, 1024);

    shadowCubePassShader->setUniform(lightPositionUniformName, camera.getPosition());

    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    for (unsigned int i = 0; i < 6; ++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowCubeBuffer.getId());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, g_cameraDirections[i].cubemapFace,
                               m_shadowCubeTexture->getId(), 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, 1024, 1024);

        // Send view/projection to default shader
        glm::mat4 view = glm::lookAt(camera.getPosition(), camera.getPosition() + g_cameraDirections[i].target,
                                     g_cameraDirections[i].up);

        shadowCubePassShader->setUniform(projectionMatrixUniformName, camera.getProjection());
        shadowCubePassShader->setUniform(viewMatrixUniformName, view);

        // Query visible scene objects
        SceneQuery query;
        scene.getVisibleObjects(camera, query);

        // Traverse visible objects
        while (query.hasNextObject())
        {
            // Get next visible object
            SceneObjectId id = query.getNextObject();

            // Object attributes
            ResourceId meshId = -1;
            ResourceId materialId = -1;
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
            bool visible;

            // Retrieve object data
            if (!scene.getObject(id, meshId, materialId, position, rotation, scale, visible))
            {
                // Invalid id
                loge("Invalid scene object id {}.", id);
            }
            else
            {
                // Resolve ids
                Mesh *mesh = manager.getMesh(meshId);
                Material *material = manager.getMaterial(materialId);

                Transformer transformer;

                // Set transformations
                transformer.setPosition(position);
                transformer.setRotation(rotation);
                transformer.setScale(scale);

                // Forward draw call
                draw(mesh, transformer.getTranslationMatrix(), transformer.getRotationMatrix(),
                     transformer.getScaleMatrix(), material, manager, shadowCubePassShader);
            }
        }
    }

    // Disable buffer
    m_shadowCubeBuffer.setInactive(GL_FRAMEBUFFER);

    glClearColor(0, 0, 0, 0);
    glCullFace(GL_BACK);
}

void DeferredRenderer::lightPass(const IScene &scene, const ICamera &camera, const Window &window,
                                 const IGraphicsResourceManager &manager, ISceneQuery &query)
{
    // Prepare light pass frame buffer
    glViewport(0, 0, window.getWidth(), window.getHeight());
    // Resize
    m_lightPassFrameBuffer.resize(window.getWidth(), window.getHeight());
    // Enable light buffer
    m_lightPassFrameBuffer.setActive(GL_FRAMEBUFFER);

    // Retrieve ambient light
    glm::vec3 ambientColor;
    float ambientIntensity;
    scene.getAmbientLight(ambientColor, ambientIntensity);
    // Initialize light buffer with ambient light
    glClearColor(ambientColor.x * ambientIntensity, ambientColor.y * ambientIntensity,
                 ambientColor.z * ambientIntensity, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // No depth testing for light volumes
    glDisable(GL_DEPTH_TEST);
    // Additive blending for light accumulation
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    // Draw point light volumes
    pointLightPass(scene, camera, window, manager, query);

    // Draw directional lights
    directionalLightPass(scene, camera, window, manager, query);

    // Reset state and cleanup
    glDisable(GL_BLEND);
    m_lightPassFrameBuffer.setInactive(GL_FRAMEBUFFER);
}

void DeferredRenderer::pointLightPass(const IScene &scene, const ICamera &camera, const Window &window,
                                      const IGraphicsResourceManager &manager, ISceneQuery &query)
{
    // Point light pass
    ShaderProgram *pointLightPassShader = manager.getShaderProgram(m_pointLightPassShaderId);
    if (pointLightPassShader == nullptr)
    {
        loge("Shader program for point light pass could not be retrieved.");
        return;
    }

    Mesh *pointLightMesh = manager.getMesh(m_pointLightSphereId);
    if (pointLightMesh == nullptr)
    {
        loge("Mesh object for point light pass could not be retrieved.");
        return;
    }

    // Render point light volumes into light buffer
    while (query.hasNextPointLight())
    {
        // Retrieve light id
        SceneObjectId pointLightId = query.getNextPointLight();
        // Retrieve light parameters
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        float radius;
        bool castsShadow;

        if (!scene.getPointLight(pointLightId, position, radius, color, intensity, castsShadow))
        {
            loge("Failed to retrieve point light data from point light id {}.", pointLightId);
        }
        else
        {
            // fov should be 90.f instead of 89.54f, but this does not work, because
            // the view is too
            // wide in this case. 89.54f is determined by testing. 89.53 is already
            // too small and
            // 89.55 too big.
            glm::mat4 shadowProj = glm::perspective(89.54f, 1.0f, 0.01f, radius * 1.5f);
            StaticCamera shadowCamera(glm::mat4(), shadowProj, position);
            shadowCubePass(scene, shadowCamera, window, manager);

            // Prepare light pass frame buffer
            glViewport(0, 0, window.getWidth(), window.getHeight());
            m_lightPassFrameBuffer.setActive(GL_FRAMEBUFFER);

            // No depth testing for light volumes
            glDisable(GL_DEPTH_TEST);
            // Additive blending for light accumulation
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);

            // Cull front facing faces
            glCullFace(GL_FRONT);

            // Set textures for point light pass
            // Set depth texture
            m_depthTexture->setActive(lightPassDepthTextureUnit);
            pointLightPassShader->setUniform(depthTextureUniformName, lightPassDepthTextureUnit);

            // Set texture with world space normal and specular power
            m_normalSpecularTexture->setActive(lightPassNormalSpecularTextureUnit);
            pointLightPassShader->setUniform(normalSpecularTextureUniformName, lightPassNormalSpecularTextureUnit);

            // Set shadow texture for shadow mapping
            glActiveTexture(GL_TEXTURE0 + lightPassShadowMapTextureUnit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowCubeTexture->getId());
            pointLightPassShader->setUniform(shadowCubeTextureUniformName, lightPassShadowMapTextureUnit);

            // Set screen size
            pointLightPassShader->setUniform(screenWidthUniformName, (float)window.getWidth());
            pointLightPassShader->setUniform(screenHeightUniformName, (float)window.getHeight());

            // Camera position
            pointLightPassShader->setUniform(cameraPositionUniformName, camera.getPosition());

            // Inverse view-projection
            pointLightPassShader->setUniform(inverseViewProjectionMatrixUniformName,
                                             m_transformer.getInverseViewProjectionMatrix());

            m_transformer.setPosition(position);
            // Scale is calculated from light radius
            // Sphere model has radius 1.f
            m_transformer.setScale(glm::vec3(radius));
            // Point lights do not have rotation
            m_transformer.setRotation(glm::quat(0.f, 0.f, 0.f, 0.f));

            // Light volume transformation for vertex shader
            pointLightPassShader->setUniform(modelViewProjectionMatrixUniformName,
                                             m_transformer.getModelViewProjectionMatrix());

            // Set point light parameters for fragment shader
            pointLightPassShader->setUniform(lightPositionUniformName, position);
            pointLightPassShader->setUniform(lightRadiusUniformName, radius);
            pointLightPassShader->setUniform(lightColorUniformName, color);
            pointLightPassShader->setUniform(lightIntensityUniformName, intensity);
            ::draw(*pointLightMesh);
        }
    }

    return;
}

void DeferredRenderer::directionalLightPass(const IScene &scene, const ICamera &camera, const Window &window,
                                            const IGraphicsResourceManager &manager, ISceneQuery &query)
{
    // Restrieve shader
    ShaderProgram *directionalLightPassShader = manager.getShaderProgram(m_directionalLightPassShaderId);
    if (directionalLightPassShader == nullptr)
    {
        loge("Shader program for directional light pass could not be retrieved.");
        return;
    }

    // Retrieve fullscreen quad mesh
    Mesh *quadMesh = manager.getMesh(m_directionalLightScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for directional light pass could not be retrieved.");
        return;
    }

    // Render point light volumes into light buffer
    while (query.hasNextDirectionalLight())
    {
        // Retrieve light id
        SceneObjectId directionalLightId = query.getNextDirectionalLight();
        // Retrieve light parameters
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
        bool castsShadow;

        if (!scene.getDirectionalLight(directionalLightId, direction, color, intensity, castsShadow))
        {
            loge("Failed to retrieve directional light data from point light id {}.", directionalLightId);
        }
        else
        {
            // Create shadow camera
            glm::mat4 shadowView = glm::lookAt(glm::vec3(0), glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 shadowProj = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, -250.0f, 150.0f);
            StaticCamera shadowCamera(shadowView, shadowProj, camera.getPosition());

            // Render shadow map
            shadowMapPass(scene, shadowCamera, window, manager);

            // Prepare light pass frame buffer
            m_lightPassFrameBuffer.setActive(GL_FRAMEBUFFER);
            glViewport(0, 0, window.getWidth(), window.getHeight());

            // No depth testing for light volumes
            glDisable(GL_DEPTH_TEST);
            // Additive blending for light accumulation
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);

            // Reset culling
            glCullFace(GL_BACK);

            // Set shader active
            directionalLightPassShader->setActive();

            // Set textures for point light pass
            // Set depth texture
            m_depthTexture->setActive(lightPassDepthTextureUnit);
            directionalLightPassShader->setUniform(depthTextureUniformName, lightPassDepthTextureUnit);

            // Set texture with world space normal and specular power
            m_normalSpecularTexture->setActive(lightPassNormalSpecularTextureUnit);
            directionalLightPassShader->setUniform(normalSpecularTextureUniformName,
                                                   lightPassNormalSpecularTextureUnit);

            // Set shadow texture for shadow mapping
            m_shadowDepthTexture->setActive(lightPassShadowMapTextureUnit);
            directionalLightPassShader->setUniform(shadowMapTextureUniformName, lightPassShadowMapTextureUnit);

            // Set screen size
            directionalLightPassShader->setUniform(screenWidthUniformName, (float)window.getWidth());
            directionalLightPassShader->setUniform(screenHeightUniformName, (float)window.getHeight());

            // Inverse view-projection
            directionalLightPassShader->setUniform(inverseViewProjectionMatrixUniformName,
                                                   m_transformer.getInverseViewProjectionMatrix());

            // Shadow ViewProjectionBias
            glm::mat4 shadowViewProjBiasMatrix = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                           0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f) *
                                                 shadowProj * shadowView;
            directionalLightPassShader->setUniform(shadowViewProjectionBiasMatrixUniformName, shadowViewProjBiasMatrix);

            // Set directional light parameters
            directionalLightPassShader->setUniform(lightDirectionUniformName, direction);
            directionalLightPassShader->setUniform(lightColorUniformName, color);
            directionalLightPassShader->setUniform(lightIntensityUniformName, intensity);
            ::draw(*quadMesh);
        }
    }

    // Reset culling
    glCullFace(GL_BACK);

    return;
}

void DeferredRenderer::illuminationPass(const IScene &scene, const ICamera &camera, const Window &window,
                                        const IGraphicsResourceManager &manager, ISceneQuery &query)
{
    // Reset viewport
    glViewport(0, 0, window.getWidth(), window.getHeight());
    m_illumationPassFrameBuffer.resize(window.getWidth(), window.getHeight());
    // TODO Clear illumination pass buffer?

    // Get illumination shader
    ShaderProgram *illuminationShader = manager.getShaderProgram(m_illuminationPassShaderId);
    if (illuminationShader == nullptr)
    {
        loge("Shader program for illumination pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_illuminationPassScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for illumination pass could not be retrieved.");
        return;
    }

    // Set textures
    // L-buffer light texture
    m_lightPassTexture->setActive(illuminationPassLightTextureUnit);
    illuminationShader->setUniform(lightTextureUniformName, illuminationPassLightTextureUnit);

    // G-buffer diffuse glow texture
    m_diffuseGlowTexture->setActive(illuminationPassDiffuseGlowTextureUnit);
    illuminationShader->setUniform(diffuseGlowTextureUniformName, illuminationPassDiffuseGlowTextureUnit);

    // G-buffer depth texture
    m_depthTexture->setActive(illuminationPassDepthTextureUnit);
    illuminationShader->setUniform(depthTextureUniformName, illuminationPassDepthTextureUnit);

    // Screen parameters
    illuminationShader->setUniform(screenWidthUniformName, (float)window.getWidth());
    illuminationShader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Draw into frame buffer
    m_illumationPassFrameBuffer.setActive(GL_FRAMEBUFFER);
    ::draw(*quadMesh);
    m_illumationPassFrameBuffer.setInactive(GL_FRAMEBUFFER);
}

void DeferredRenderer::postProcessPass(const ICamera &camera, const Window &window,
                                       const IGraphicsResourceManager &manager, const std::shared_ptr<Texture> &texture)
{
    // Reset viewport
    glViewport(0, 0, window.getWidth(), window.getHeight());
    // Resize frame buffer
    m_postProcessPassFrameBuffer0.resize(window.getWidth(), window.getHeight());
    m_postProcessPassFrameBuffer1.resize(window.getWidth(), window.getHeight());
    m_postProcessPassFrameBuffer2.resize(window.getWidth(), window.getHeight());

    // Pass 1: fxaa
    m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
    if (camera.getFeatureInfo().fxaaActive)
    {
        fxaaPass(window, manager, texture);
    }
    else
    {
        // Passthrough
        passthroughPass(window, manager, texture);
    }
    // Anti aliased scene texture in texture0

    // Pass 2: fog
    // TODO Fog parameter
    m_postProcessPassFrameBuffer1.setActive(GL_FRAMEBUFFER);
    fogPass(camera, window, manager, m_postProcessPassTexture0);
    // Foggy scene in texture1

    // Pass 3: dof
    m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
    if (camera.getFeatureInfo().dofActive)
    {
        // Pass 3.1: gauss blur
        gaussBlurVerticalPass(window, manager, m_postProcessPassTexture1);
        // Blurred in texture0

        m_postProcessPassFrameBuffer2.setActive(GL_FRAMEBUFFER);
        gaussBlurHorizontalPass(window, manager, m_postProcessPassTexture0);
        // Blurred in texture2

        for (unsigned int i = 0; i < 3; ++i)
        {
            m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
            gaussBlurVerticalPass(window, manager, m_postProcessPassTexture2);
            // Blurred in texture0

            m_postProcessPassFrameBuffer2.setActive(GL_FRAMEBUFFER);
            gaussBlurHorizontalPass(window, manager, m_postProcessPassTexture0);
            // Blurred in texture2
        }

        // TODO DOF parameter
        m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
        depthOfFieldPass(camera, window, manager, m_postProcessPassTexture1, m_postProcessPassTexture2);
    }
    else
    {
        // Passthrough
        passthroughPass(window, manager, m_postProcessPassTexture1);
    }
    // Processed scene in texture0

    // Godray technique, consists of 2 passes
    if ((camera.getFeatureInfo().godRayActive || camera.getFeatureInfo().renderMode == RenderMode::GodRay))
    {
        // God ray pass 1
        m_postProcessPassFrameBuffer1.setActive(GL_FRAMEBUFFER);
        godRayPass1(window, manager, m_postProcessPassTexture0);
        // God ray texture in tex 1

        // Godray debug mode
        if (camera.getFeatureInfo().renderMode == RenderMode::GodRay)
        {
            // Return generated godray texture
            m_postProcessPassOutputTexture = m_postProcessPassTexture1;
            return;
        }

        // God ray pass 2
        m_postProcessPassFrameBuffer2.setActive(GL_FRAMEBUFFER);
        godRayPass2(window, manager, m_postProcessPassTexture0, m_postProcessPassTexture1);
    }
    else
    {
        // Passthrough into tex 2
        m_postProcessPassFrameBuffer2.setActive(GL_FRAMEBUFFER);
        passthroughPass(window, manager, m_postProcessPassTexture0);
    }

    // Vignette blur pass
    m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
    vignetteBlurPass(window, manager, m_postProcessPassTexture2);
    // Scene in texture 0

    if (camera.getFeatureInfo().bloomActive)
    {
        // Bloom pass
        m_postProcessPassFrameBuffer1.setActive(GL_FRAMEBUFFER);
        bloomPass1(window, manager, m_postProcessPassTexture0);
        // Bloom scene in texture 1

        // Additive blend
        m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
        bloomPass2(window, manager, m_postProcessPassTexture0, m_postProcessPassTexture1);
        // Scene with bloom in texture 0
    }

    if (camera.getFeatureInfo().lenseFlareActive)
    {
        // Lens flare pass
        m_postProcessPassFrameBuffer2.setActive(GL_FRAMEBUFFER);
        lensFlarePass(window, manager, m_postProcessPassTexture0);

        m_postProcessPassFrameBuffer1.setActive(GL_FRAMEBUFFER);
        lensFlarePass2(window, manager, m_postProcessPassTexture2);

        m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
        lensFlarePass3(window, manager, m_postProcessPassTexture0, m_postProcessPassTexture1);
        // Scene with flare in texture 0
    }

    // Tone map
    m_postProcessPassFrameBuffer1.setActive(GL_FRAMEBUFFER);
    toneMapPass(window, manager, m_postProcessPassTexture0);
    // Tone mapped scene in texture 1

    if (camera.getFeatureInfo().toonActive)
    {
        // Cel shading pass
        m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
        celPass(window, manager, m_postProcessPassTexture1);
    }
    else
    {
        m_postProcessPassFrameBuffer0.setActive(GL_FRAMEBUFFER);
        passthroughPass(window, manager, m_postProcessPassTexture1);
    }

    // Set output texture
    m_postProcessPassOutputTexture = m_postProcessPassTexture0;
    return;
}

void DeferredRenderer::fxaaPass(const Window &window, const IGraphicsResourceManager &manager,
                                const std::shared_ptr<Texture> &texture)
{
    // Get fxaa shader
    ShaderProgram *fxaaShader = manager.getShaderProgram(m_fxaaPassShaderId);
    if (fxaaShader == nullptr)
    {
        loge("Shader program for fxaa pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for fxaa pass could not be retrieved.");
        return;
    }

    // Set input texture
    texture->setActive(fxaaPassInputTextureUnit);
    fxaaShader->setUniform(sceneTextureUniformName, fxaaPassInputTextureUnit);

    // Set screen size
    fxaaShader->setUniform(screenWidthUniformName, (float)window.getWidth());
    fxaaShader->setUniform(screenHeightUniformName, (float)window.getHeight());

    ::draw(*quadMesh);
}

void DeferredRenderer::fogPass(const ICamera &camera, const Window &window, const IGraphicsResourceManager &manager,
                               const std::shared_ptr<Texture> &texture)
{
    // Get fog shader
    ShaderProgram *fogShader = manager.getShaderProgram(m_fogPassShaderId);
    if (fogShader == nullptr)
    {
        loge("Shader program for fog pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for fog pass could not be retrieved.");
        return;
    }

    fogShader->setActive();

    texture->setActive(fogPassSceneTextureUnit);
    fogShader->setUniform(sceneTextureUniformName, fogPassSceneTextureUnit);

    m_depthTexture->setActive(fogPassDepthTextureUnit);
    fogShader->setUniform(depthTextureUniformName, fogPassDepthTextureUnit);

    fogShader->setUniform(screenWidthUniformName, (float)window.getWidth());
    fogShader->setUniform(screenHeightUniformName, (float)window.getHeight());

    int fogType = 0;
    switch (camera.getFeatureInfo().fogType)
    {
    case FogType::None:
        fogType = 0;
        break;
    case FogType::Linear:
        fogType = 1;
        break;
    case FogType::Exp:
        fogType = 2;
        break;
    case FogType::Exp2:
        fogType = 3;
        break;
    default:
        fogType = 0;
    }
    fogShader->setUniform(fogPassTypeUniformName, fogType);

    ::draw(*quadMesh);
}

void DeferredRenderer::gaussBlurVerticalPass(const Window &window, const IGraphicsResourceManager &manager,
                                             const std::shared_ptr<Texture> &texture)
{
    // Get gauss shader
    ShaderProgram *shader = manager.getShaderProgram(m_gaussBlurVerticalShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for vertical gauss blur pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for vertical gauss blur pass could not be retrieved.");
        return;
    }

    // Input texture
    texture->setActive(gaussBlurVerticalPassInputTextureUnit);
    shader->setUniform(sceneTextureUniformName, gaussBlurVerticalPassInputTextureUnit);

    // Blur parameter
    shader->setUniform(blurStrengthUniformName, 2.f);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::gaussBlurHorizontalPass(const Window &window, const IGraphicsResourceManager &manager,
                                               const std::shared_ptr<Texture> &texture)
{
    // Get gauss shader
    ShaderProgram *shader = manager.getShaderProgram(m_gaussBlurHorizontalShaderId);
    if (shader == nullptr)
    {
        loge(
            "Shader program for horizontal gauss blur pass could not be "
            "retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for horizontal gauss blur pass could not be retrieved.");
        return;
    }

    // Input texture
    texture->setActive(gaussBlurHoriontalPassInputTextureUnit);
    shader->setUniform(sceneTextureUniformName, gaussBlurHoriontalPassInputTextureUnit);

    // Blur parameter
    shader->setUniform(blurStrengthUniformName, 2.f);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::depthOfFieldPass(const ICamera &camera, const Window &window,
                                        const IGraphicsResourceManager &manager,
                                        const std::shared_ptr<Texture> &sceneTexture,
                                        const std::shared_ptr<Texture> &blurTexture)
{
    // Get shader
    ShaderProgram *shader = manager.getShaderProgram(m_depthOfFieldPassShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for depth-of-field pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for depth-of-field pass could not be retrieved.");
        return;
    }

    // Input scene texture
    sceneTexture->setActive(depthOfFieldPassSceneTextureUnit);
    shader->setUniform(sceneTextureUniformName, depthOfFieldPassSceneTextureUnit);

    // Input blur texture
    blurTexture->setActive(depthOfFieldPassBlurTextureUnit);
    shader->setUniform(blurTextureUniformName, depthOfFieldPassBlurTextureUnit);

    // Input depth texture
    m_depthTexture->setActive(depthOfFieldPassDepthTextureUnit);
    shader->setUniform(depthTextureUniformName, depthOfFieldPassDepthTextureUnit);

    // Depth-of-field parameters
    shader->setUniform(blurNearUniformName, camera.getFeatureInfo().dofNearBlur);
    shader->setUniform(focusNearUniformName, camera.getFeatureInfo().dofNearFocus);
    shader->setUniform(focusFarUniformName, camera.getFeatureInfo().dofFarFocus);
    shader->setUniform(blurFarUniformName, camera.getFeatureInfo().dofFarBlur);

    // Camera position
    shader->setUniform(cameraPositionUniformName, camera.getPosition());

    // Transformation
    shader->setUniform(inverseViewProjectionMatrixUniformName, m_transformer.getInverseViewProjectionMatrix());

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::displayPass(const Window &window, const IGraphicsResourceManager &manager,
                                   const std::shared_ptr<Texture> &texture)
{
    // Set main FBO active
    FrameBuffer::setDefaultActive();
    passthroughPass(window, manager, texture);
}

void DeferredRenderer::passthroughPass(const Window &window, const IGraphicsResourceManager &manager,
                                       const std::shared_ptr<Texture> &texture)
{
    // Reset viewport
    glViewport(0, 0, window.getWidth(), window.getHeight());

    // Get display shader
    ShaderProgram *displayShader = manager.getShaderProgram(m_displayPassShaderId);
    if (displayShader == nullptr)
    {
        loge("Shader program for display pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_illuminationPassScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for illumination pass could not be retrieved.");
        return;
    }

    // Scene texture
    texture->setActive(displayPassSceneTextureUnit);
    displayShader->setUniform(sceneTextureUniformName, displayPassSceneTextureUnit);

    // Screen parameters
    displayShader->setUniform(screenWidthUniformName, (float)window.getWidth());
    displayShader->setUniform(screenHeightUniformName, (float)window.getHeight());

    ::draw(*quadMesh);
}

void DeferredRenderer::godRayPass1(const Window &window, const IGraphicsResourceManager &manager,
                                   const std::shared_ptr<Texture> &texture)
{
    // Get shader
    ShaderProgram *shader = manager.getShaderProgram(m_godRayPass1ShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for god ray 1 pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for god ray 1 pass could not be retrieved.");
        return;
    }

    // Input texture
    texture->setActive(godRayPass1InputTextureUnit);
    shader->setUniform(sceneTextureUniformName, godRayPass1InputTextureUnit);

    // Depth texture
    m_depthTexture->setActive(godRayPass1DepthTextureUnit);
    shader->setUniform(depthTextureUniformName, godRayPass1DepthTextureUnit);

    // Inverse projection
    shader->setUniform(inverseProjectionMatrixUniformName, m_transformer.getInverseProjectionMatrix());

    // Light position
    shader->setUniform(lightPositionScreenUniformName, glm::vec2(0.5, 0.5));

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::godRayPass2(const Window &window, const IGraphicsResourceManager &manager,
                                   const std::shared_ptr<Texture> &sceneTexture,
                                   const std::shared_ptr<Texture> &godrayTexture)
{
    // Get gauss shader
    ShaderProgram *shader = manager.getShaderProgram(m_godRayPass2ShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for god ray 2 pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for god ray 2 pass could not be retrieved.");
        return;
    }

    // Scene texture
    sceneTexture->setActive(godRayPass2SceneTextureUnit);
    shader->setUniform(sceneTextureUniformName, godRayPass2SceneTextureUnit);

    // God ray texture
    godrayTexture->setActive(godRayPass2GodRayTextureUnit);
    shader->setUniform(godRayTextureUniformName, godRayPass2GodRayTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::visualizeDepthPass(const ICamera &camera, const Window &window,
                                          const IGraphicsResourceManager &manager)
{
    // Get shader
    ShaderProgram *shader = manager.getShaderProgram(m_visualizeDepthPassShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for depth visualization pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for depth visualization pass could not be retrieved.");
        return;
    }

    // Depth texture
    m_depthTexture->setActive(visualizeDepthPassDepthTextureUnit);
    shader->setUniform(depthTextureUniformName, visualizeDepthPassDepthTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Camera parameters
    // TODO Replace
    shader->setUniform(cameraZNearUniformName, 0.01f);
    shader->setUniform(cameraZFarUniformName, 1000.f);

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::vignetteBlurPass(const Window &window, const IGraphicsResourceManager &manager,
                                        const std::shared_ptr<Texture> &texture)
{
    // Get distortion shader
    ShaderProgram *shader = manager.getShaderProgram(m_vignetteBlurPassShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for vignette blur pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for vignette blur pass could not be retrieved.");
        return;
    }

    // Set input texture
    texture->setActive(vignetteBlurPassInputTextureUnit);
    shader->setUniform(sceneTextureUniformName, vignetteBlurPassInputTextureUnit);

    // Set screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    ::draw(*quadMesh);
}

void DeferredRenderer::bloomPass1(const Window &window, const IGraphicsResourceManager &manager,
                                  const std::shared_ptr<Texture> &texture)
{
    // Get bloom pass 1 shader
    ShaderProgram *shader = manager.getShaderProgram(m_bloomPass1ShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for bloom 1 pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for bloom 1 pass could not be retrieved.");
        return;
    }

    // Set input texture
    texture->setActive(bloomPass1InputTextureUnit);
    shader->setUniform(sceneTextureUniformName, bloomPass1InputTextureUnit);

    // Set screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    ::draw(*quadMesh);
}

void DeferredRenderer::bloomPass2(const Window &window, const IGraphicsResourceManager &manager,
                                  const std::shared_ptr<Texture> &sceneTexture,
                                  const std::shared_ptr<Texture> &bloomTexture)
{
    // TODO Actually same as godray pass 2, create additive blend pass
    // Get bloom pass 2 shader
    ShaderProgram *shader = manager.getShaderProgram(m_bloomPass2ShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for bloom pass 2 could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for bloom pass 2 could not be retrieved.");
        return;
    }

    // Scene texture
    sceneTexture->setActive(bloomPass2SceneTextureUnit);
    shader->setUniform(sceneTextureUniformName, bloomPass2SceneTextureUnit);

    // Bloom texture
    bloomTexture->setActive(bloomPass2BloomTextureUnit);
    shader->setUniform(bloomTextureUniformName, bloomPass2BloomTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::lensFlarePass(const Window &window, const IGraphicsResourceManager &manager,
                                     const std::shared_ptr<Texture> &sceneTexture)
{
    ShaderProgram *shader = manager.getShaderProgram(m_lensFlarePassShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for lens flare could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for lens flare could not be retrieved.");
        return;
    }

    // Scene texture
    sceneTexture->setActive(lensFlare1InputTextureUnit);
    shader->setUniform(sceneTextureUniformName, lensFlare1InputTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::lensFlarePass2(const Window &window, const IGraphicsResourceManager &manager,
                                      const std::shared_ptr<Texture> &sceneTexture)
{
    ShaderProgram *shader = manager.getShaderProgram(m_lensFlarePass2ShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for lens flare could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for lens flare could not be retrieved.");
        return;
    }

    // Scene texture
    sceneTexture->setActive(lensFlare2InputTextureUnit);
    shader->setUniform(sceneTextureUniformName, lensFlare2InputTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::lensFlarePass3(const Window &window, const IGraphicsResourceManager &manager,
                                      const std::shared_ptr<Texture> &sceneTexture,
                                      const std::shared_ptr<Texture> &lensTexture)
{
    ShaderProgram *shader = manager.getShaderProgram(m_lensFlarePass3ShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for lens flare could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for lens flare could not be retrieved.");
        return;
    }

    // Scene texture
    sceneTexture->setActive(lensFlare3SceneTextureUnit);
    shader->setUniform(sceneTextureUniformName, lensFlare3SceneTextureUnit);

    // Scene texture
    lensTexture->setActive(lensFlare3LensTextureUnit);
    shader->setUniform(lensFlareTextureUniformName, lensFlare3LensTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::celPass(const Window &window, const IGraphicsResourceManager &manager,
                               const std::shared_ptr<Texture> &sceneTexture)
{
    ShaderProgram *shader = manager.getShaderProgram(m_celPassShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for cel Pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for cel Pass could not be retrieved.");
        return;
    }

    // Scene texture
    sceneTexture->setActive(celPassInputTextureUnit);
    shader->setUniform(sceneTextureUniformName, celPassInputTextureUnit);

    // Screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    // Perform pass
    ::draw(*quadMesh);
}

void DeferredRenderer::toneMapPass(const Window &window, const IGraphicsResourceManager &manager,
                                   const std::shared_ptr<Texture> &texture)
{
    // Get tone map shader
    ShaderProgram *shader = manager.getShaderProgram(m_toneMapPassShaderId);
    if (shader == nullptr)
    {
        loge("Shader program for tone map pass could not be retrieved.");
        return;
    }

    // Get screen space quad
    Mesh *quadMesh = manager.getMesh(m_postProcessScreenQuadId);
    if (quadMesh == nullptr)
    {
        loge("Mesh object for tone map pass could not be retrieved.");
        return;
    }

    // Set input texture
    texture->setActive(toneMapPassInputTextureUnit);
    shader->setUniform(sceneTextureUniformName, toneMapPassInputTextureUnit);

    // Set screen size
    shader->setUniform(screenWidthUniformName, (float)window.getWidth());
    shader->setUniform(screenHeightUniformName, (float)window.getHeight());

    ::draw(*quadMesh);
}

void DeferredRenderer::draw(Mesh *mesh, const glm::mat4 &translation, const glm::mat4 &rotation, const glm::mat4 &scale,
                            Material *material, const IGraphicsResourceManager &manager, ShaderProgram *shader)
{
    // TODO Only called from geometry pass, rename?
    shader->setActive();

    // Transformation matrices
    // shader->setUniform(translationMatrixUniformName, translation);
    shader->setUniform(rotationMatrixUniformName, rotation);
    // shader->setUniform(scaleMatrixUniformName, scale);
    shader->setUniform(modelMatrixUniformName, translation * rotation * scale);

    // Send material textures to shader
    if (material->hasDiffuse())
    {
        material->getDiffuse()->setActive(diffuseTextureUnit);
    }
    else
    {
        manager.getDefaultDiffuseTexture()->setActive(diffuseTextureUnit);
    }
    shader->setUniform(diffuseTextureUniformName, diffuseTextureUnit);

    if (material->hasNormal())
    {
        material->getNormal()->setActive(normalTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultNormalTexture()->setActive(normalTextureUnit);
    }
    shader->setUniform(normalTextureUniformName, normalTextureUnit);

    if (material->hasSpecular())
    {
        material->getSpecular()->setActive(specularTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultSpecularTexture()->setActive(specularTextureUnit);
    }
    shader->setUniform(specularTextureUniformName, specularTextureUnit);

    if (material->hasGlow())
    {
        material->getGlow()->setActive(glowTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultGlowTexture()->setActive(glowTextureUnit);
    }
    shader->setUniform(glowTextureUniformName, glowTextureUnit);

    // TODO Sort materials with alpha texture for blending.
    if (material->hasAlpha())
    {
        material->getAlpha()->setActive(alphaTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultAlphaTexture()->setActive(alphaTextureUnit);
    }
    shader->setUniform(alphaTextureUniformName, alphaTextureUnit);

    // Draw mesh
    // TODO Consider custom shader bindings for meshes
    ::draw(*mesh);

    // TODO Cleanup?
}

bool DeferredRenderer::initGeometryPass(IResourceManager &manager)
{
    // Init geometry pass shader
    // TODO Read file name from config?
    // Geometry pass shader for filling gbuffer
    std::string geometryPassShaderFile("data/shader/deferred/geometry_pass.ini");
    m_geometryPassShaderId = manager.loadShader(geometryPassShaderFile);

    // Check if ok
    if (m_geometryPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", geometryPassShaderFile.c_str());
        return false;
    }

    // Init gbuffer
    // Diffuse texture, stores base color and glow mask.
    m_diffuseGlowTexture = std::make_shared<Texture>();
    m_diffuseGlowTexture->init(800, 600, GL_RGBA8);

    // Normal texture, store x and y, z normals and specularity.
    m_normalSpecularTexture = std::make_shared<Texture>();
    m_normalSpecularTexture->init(800, 600, GL_RGBA16F);

    // Depth texture
    m_depthTexture = std::make_shared<Texture>();
    m_depthTexture->init(800, 600, GL_DEPTH_COMPONENT24);

    // Total 96 bit per pixel
    m_geometryBuffer.attach(m_depthTexture, GL_DEPTH_ATTACHMENT);
    m_geometryBuffer.attach(m_diffuseGlowTexture, GL_COLOR_ATTACHMENT0);
    m_geometryBuffer.attach(m_normalSpecularTexture, GL_COLOR_ATTACHMENT1);
    logi("GBuffer state: {}.", m_geometryBuffer.getState());

    // Reset framebuffer
    m_geometryBuffer.setInactive(GL_FRAMEBUFFER);
    return true;
}

bool DeferredRenderer::initShadowCubePass(IResourceManager &manager)
{
    std::string shaderFile("data/shader/shadow_cube_pass.ini");
    m_shadowCubePassShaderId = manager.loadShader(shaderFile);

    if (m_shadowCubePassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }

    m_shadowCubeBuffer.resize(1024, 1024);

    // Depth texture
    m_shadowCubeDepthTexture = std::make_shared<Texture>();
    m_shadowCubeDepthTexture->init(1024, 1024, GL_DEPTH_COMPONENT24);

    // TODO Cleanup and move into cube texture class
    glBindTexture(GL_TEXTURE_2D, m_shadowCubeDepthTexture->getId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
    // GL_COMPARE_REF_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, 1024, 1024, 0, GL_RED, GL_FLOAT, NULL);
    }

    m_shadowCubeTexture =
        std::make_shared<Texture>(textureId, false, 1024, 1024, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowCubeBuffer.getId());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowCubeDepthTexture->getId(), 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    m_shadowCubeBuffer.setInactive(GL_FRAMEBUFFER);
    logi("Shadow cube buffer state: {}.", m_shadowCubeBuffer.getState().c_str());

    // Reset framebuffer
    m_shadowCubeBuffer.setInactive(GL_FRAMEBUFFER);
    return true;
}

bool DeferredRenderer::initShadowMapPass(IResourceManager &manager)
{
    std::string shaderFile("data/shader/shadow_map_pass.ini");
    m_shadowMapPassShaderId = manager.loadShader(shaderFile);

    if (m_shadowMapPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }

    // Depth texture
    m_shadowDepthTexture = std::make_shared<Texture>();
    m_shadowDepthTexture->init(4096, 4096, GL_DEPTH_COMPONENT24);

    // Set to PCF parameters
    glBindTexture(GL_TEXTURE_2D, m_shadowDepthTexture->getId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Total 96 bit per pixel
    m_shadowMapBuffer.attach(m_shadowDepthTexture, GL_DEPTH_ATTACHMENT);
    m_shadowMapBuffer.setActive(GL_FRAMEBUFFER);
    glDrawBuffer(GL_NONE);
    m_shadowMapBuffer.setInactive(GL_FRAMEBUFFER);
    logi("Shadow map buffer state: {}.", m_shadowMapBuffer.getState().c_str());

    // Reset framebuffer
    m_shadowMapBuffer.setInactive(GL_FRAMEBUFFER);
    return true;
}

bool DeferredRenderer::initPointLightPass(IResourceManager &manager)
{
    // Point light shader
    std::string pointLightPassShaderFile("data/shader/deferred/point_light_pass.ini");
    m_pointLightPassShaderId = manager.loadShader(pointLightPassShaderFile);

    // Check if ok
    if (m_pointLightPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", pointLightPassShaderFile.c_str());
        return false;
    }

    // Load sphere mesh for point light representation
    std::string sphereMesh = "data/mesh/sphere.obj";
    m_pointLightSphereId = manager.loadMesh(sphereMesh);
    if (m_pointLightSphereId == InvalidResource)
    {
        loge("Failed to load point light volume mesh {}.", sphereMesh.c_str());
        return false;
    }

    // Attach texture for light data
    m_lightPassTexture = std::make_shared<Texture>();
    if (!m_lightPassTexture->init(800, 600, GL_RGBA16F))
    {
        loge("Failed to create color texture for light pass frame buffer.");
        return false;
    }
    m_lightPassFrameBuffer.attach(m_lightPassTexture, GL_COLOR_ATTACHMENT0);

    // Attach depth buffer
    // Depth gets discarded, depth values from geometry pass are used
    std::shared_ptr<RenderBuffer> depthBuffer = std::make_shared<RenderBuffer>();
    if (!depthBuffer->init(800, 600, GL_DEPTH_COMPONENT))
    {
        loge("Failed to create depth buffer for light pass frame buffer.");
        return false;
    }
    m_lightPassFrameBuffer.attach(depthBuffer, GL_DEPTH_ATTACHMENT);
    return true;
}

bool DeferredRenderer::initDirectionalLightPass(IResourceManager &manager)
{
    // Uses same frame buffer as point light pass
    // Directional light shader
    std::string directionalLightPassShaderFile("data/shader/deferred/directional_light_pass.ini");
    m_directionalLightPassShaderId = manager.loadShader(directionalLightPassShaderFile);

    // Check if ok
    if (m_directionalLightPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", directionalLightPassShaderFile.c_str());
        return false;
    }

    // Load quad mesh for directional light representation
    std::string quadMesh = "data/mesh/screen_quad.obj";
    m_directionalLightScreenQuadId = manager.loadMesh(quadMesh);
    if (m_directionalLightScreenQuadId == InvalidResource)
    {
        loge("Failed to load screen quad mesh {}.", quadMesh.c_str());
        return false;
    }

    return true;
}

bool DeferredRenderer::initIlluminationPass(IResourceManager &manager)
{
    // Load illumination shader
    std::string illuminationPassShaderFile = "data/shader/deferred/illumination_pass.ini";
    m_illuminationPassShaderId = manager.loadShader(illuminationPassShaderFile);
    // Check if ok
    if (m_illuminationPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", illuminationPassShaderFile.c_str());
        return false;
    }

    // Screen quad mesh
    std::string quadMesh = "data/mesh/screen_quad.obj";
    m_illuminationPassScreenQuadId = manager.loadMesh(quadMesh);
    // Check if ok
    if (m_illuminationPassScreenQuadId == InvalidResource)
    {
        loge("Failed to load screen quad mesh {}.", quadMesh.c_str());
        return false;
    }

    // FBO
    m_illuminationPassTexture = std::make_shared<Texture>();
    if (!m_illuminationPassTexture->init(800, 600, GL_RGB16F))
    {
        loge("Failed to initialize illumination pass texture.");
        return false;
    }
    m_illumationPassFrameBuffer.attach(m_illuminationPassTexture, GL_COLOR_ATTACHMENT0);

    // TODO Check FBO

    return true;
}

bool DeferredRenderer::initPostProcessPass(IResourceManager &manager)
{
    // Gauss blur pass
    if (!initGaussBlurHorizontalPass(manager))
    {
        loge("Failed to initialize gauss blur horizontal pass.");
        return false;
    }
    if (!initGaussBlurVerticalPass(manager))
    {
        loge("Failed to initialize gauss blur vertical pass.");
        return false;
    }

    // FXAA
    if (!initFxaaPass(manager))
    {
        loge("Failed to initialize fxaa pass.");
        return false;
    }

    // Fog
    if (!initFogPass(manager))
    {
        loge("Failed to initialize fog pass.");
        return false;
    }

    // Godray
    if (!initGodRayPass1(manager))
    {
        loge("Failed to initialize god ray 1 pass.");
        return false;
    }

    // Should actually compose by additive blending
    if (!initGodRayPass2(manager))
    {
        loge("Failed to initialize god ray 2 pass.");
        return false;
    }

    // DOF pass
    if (!initDepthOfFieldPass(manager))
    {
        loge("Failed to initialize depth of field pass.");
        return false;
    }

    // Vignette blur pass
    if (!initVignetteBlurPass(manager))
    {
        loge("Failed to initialize depth visualization pass.");
        return false;
    }

    // Bloom pass 1
    if (!initBloomPass1(manager))
    {
        loge("Failed to initialize bloom pass 1.");
        return false;
    }

    // Bloom pass 2
    if (!initBloomPass2(manager))
    {
        loge("Failed to initialize bloom pass 2.");
        return false;
    }

    if (!initLensFlarePass(manager))
    {
        loge("Failed to initialize lens flare pass.");
        return false;
    }

    if (!initLensFlarePass2(manager))
    {
        loge("Failed to initialize lens flare pass.");
        return false;
    }

    if (!initLensFlarePass3(manager))
    {
        loge("Failed to initialize lens flare pass.");
        return false;
    }

    if (!initCelPass(manager))
    {
        loge("Failed to initialize cel pass.");
        return false;
    }

    // Tone map pass (non-adaptive)
    if (!initToneMapPass(manager))
    {
        loge("Failed to initialize non-adaptive tonemap pass.");
    }

    // Screen quad mesh
    std::string quadMesh = "data/mesh/screen_quad.obj";
    m_postProcessScreenQuadId = manager.loadMesh(quadMesh);
    if (m_directionalLightScreenQuadId == InvalidResource)
    {
        loge("Failed to load screen quad mesh {}.", quadMesh.c_str());
        return false;
    }

    // Init post processing FBOs
    // Texture 0
    m_postProcessPassTexture0 = std::make_shared<Texture>();
    if (!m_postProcessPassTexture0->init(800, 600, GL_RGB16F))
    {
        loge("Failed to initialize post process pass texture 0.");
        return false;
    }
    m_postProcessPassTexture0->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    m_postProcessPassTexture0->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    m_postProcessPassTexture0->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Texture 1
    m_postProcessPassTexture1 = std::make_shared<Texture>();
    if (!m_postProcessPassTexture1->init(800, 600, GL_RGB16F))
    {
        loge("Failed to initialize post process pass texture 1.");
        return false;
    }
    m_postProcessPassTexture1->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    m_postProcessPassTexture1->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    m_postProcessPassTexture1->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Texture 2
    m_postProcessPassTexture2 = std::make_shared<Texture>();
    if (!m_postProcessPassTexture2->init(800, 600, GL_RGB16F))
    {
        loge("Failed to initialize post process pass texture 2.");
        return false;
    }
    m_postProcessPassTexture2->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    m_postProcessPassTexture2->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    m_postProcessPassTexture2->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    m_postProcessPassFrameBuffer0.attach(m_postProcessPassTexture0, GL_COLOR_ATTACHMENT0);
    m_postProcessPassFrameBuffer1.attach(m_postProcessPassTexture1, GL_COLOR_ATTACHMENT0);
    m_postProcessPassFrameBuffer2.attach(m_postProcessPassTexture2, GL_COLOR_ATTACHMENT0);
    // TODO Depth attachment required?
    return true;
}

bool DeferredRenderer::initDepthOfFieldPass(IResourceManager &manager)
{
    // Depth of field shader
    std::string depthOfFieldShaderFile = "data/shader/post/depth_of_field_pass.ini";
    m_depthOfFieldPassShaderId = manager.loadShader(depthOfFieldShaderFile);
    // Check if ok
    if (m_depthOfFieldPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", depthOfFieldShaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initGaussBlurVerticalPass(IResourceManager &manager)
{
    // Depth of field shader
    std::string gaussBlurVerticalShaderFile = "data/shader/post/gauss_blur_vertical_pass.ini";
    m_gaussBlurVerticalShaderId = manager.loadShader(gaussBlurVerticalShaderFile);
    // Check if ok
    if (m_gaussBlurVerticalShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", gaussBlurVerticalShaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initGaussBlurHorizontalPass(IResourceManager &manager)
{
    // Depth of field shader
    std::string gaussBlurHorizontalShaderFile = "data/shader/post/gauss_blur_horizontal_pass.ini";
    m_gaussBlurHorizontalShaderId = manager.loadShader(gaussBlurHorizontalShaderFile);
    // Check if ok
    if (m_gaussBlurHorizontalShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", gaussBlurHorizontalShaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initFxaaPass(IResourceManager &manager)
{
    // Fxaa shader
    std::string fxaaShaderFile = "data/shader/post/fxaa_pass.ini";
    m_fxaaPassShaderId = manager.loadShader(fxaaShaderFile);
    // Check if ok
    if (m_fxaaPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", fxaaShaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initFogPass(IResourceManager &manager)
{
    // Fog shader
    std::string fogShaderFile = "data/shader/post/fog_pass.ini";
    m_fogPassShaderId = manager.loadShader(fogShaderFile);
    // Check if ok
    if (m_fogPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", fogShaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initGodRayPass1(IResourceManager &manager)
{
    // Get shader
    std::string shader = "data/shader/post/god_ray_1_pass.ini";
    m_godRayPass1ShaderId = manager.loadShader(shader);
    // Check if ok
    if (m_godRayPass1ShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shader.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initGodRayPass2(IResourceManager &manager)
{
    // Get shader
    std::string shader = "data/shader/post/god_ray_2_pass.ini";
    m_godRayPass2ShaderId = manager.loadShader(shader);
    // Check if ok
    if (m_godRayPass2ShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shader.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initDisplayPass(IResourceManager &manager)
{
    // Display shader
    std::string displayPassShaderFile = "data/shader/display_pass.ini";
    m_displayPassShaderId = manager.loadShader(displayPassShaderFile);
    // Check if ok
    if (m_displayPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", displayPassShaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initVisualizeDepthPass(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/debug/visualize_depth_buffer_pass.ini";
    m_visualizeDepthPassShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_visualizeDepthPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initVignetteBlurPass(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/vignette_blur_pass.ini";
    m_vignetteBlurPassShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_vignetteBlurPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initBloomPass1(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/bloom_1_pass.ini";
    m_bloomPass1ShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_bloomPass1ShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initBloomPass2(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/bloom_2_pass.ini";
    m_bloomPass2ShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_bloomPass2ShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initLensFlarePass(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/lens_flare_pass.ini";
    m_lensFlarePassShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_lensFlarePassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initLensFlarePass2(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/lens_flare_pass2.ini";
    m_lensFlarePass2ShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_lensFlarePass2ShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initLensFlarePass3(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/lens_flare_pass3.ini";
    m_lensFlarePass3ShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_lensFlarePass3ShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initCelPass(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/cel_pass.ini";
    m_celPassShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_celPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

bool DeferredRenderer::initToneMapPass(IResourceManager &manager)
{
    // Get shader
    std::string shaderFile = "data/shader/post/tonemap_pass.ini";
    m_toneMapPassShaderId = manager.loadShader(shaderFile);
    // Check if ok
    if (m_toneMapPassShaderId == InvalidResource)
    {
        loge("Failed to initialize the shader from file {}.", shaderFile.c_str());
        return false;
    }
    return true;
}
