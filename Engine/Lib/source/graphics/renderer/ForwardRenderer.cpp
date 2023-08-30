#include "kern/graphics/renderer/ForwardRenderer.h"

#include <fmtlog/fmtlog.h>

#include <cassert>
#include <glm/ext.hpp>
#include <string>

#include "kern/foundation/Transformer.h"
#include "kern/graphics/ICamera.h"
#include "kern/graphics/IGraphicsResourceManager.h"
#include "kern/graphics/IScene.h"
#include "kern/graphics/IWindow.h"
#include "kern/graphics/renderer/Draw.h"
#include "kern/graphics/renderer/core/RendererCoreConfig.h"
#include "kern/graphics/renderer/debug/RendererDebug.h"
#include "kern/graphics/resource/Material.h"
#include "kern/graphics/resource/Mesh.h"
#include "kern/graphics/resource/ShaderProgram.h"
#include "kern/graphics/resource/Texture.h"
#include "kern/graphics/resource/IResourceManager.h"
#include "kern/graphics/scene/SceneQuery.h"

// Shader sources
#include "graphics/renderer/shader/ShaderForwardRenderer.h"

ForwardRenderer::ForwardRenderer() { return; }

ForwardRenderer::~ForwardRenderer()
{
    // TODO Cleanup
}

bool ForwardRenderer::init(IResourceManager &manager)
{
    // Set clear color
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Backface culling disabled for debugging
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Winding order, standard is counter-clockwise
    glFrontFace(GL_CCW);

    // Error check
    std::string error;
    if (hasGLError(error))
    {
        loge("GL Error: {}", error.c_str());
        return false;
    }

    // Load and init default shaders
    return initShaders(manager);
}

void ForwardRenderer::draw(const IScene &scene, const ICamera &camera, const IWindow &window,
                            const IGraphicsResourceManager &manager)
{
    // Draw init
    window.setActive();

    // Retrieve and use forward shader
    m_forwardShader = manager.getShaderProgram(m_forwardShaderId);
    m_forwardShader->setActive();

    // Initializiation
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Reset viewport
    glViewport(0, 0, window.getWidth(), window.getHeight());

    // Set view and projection matrices
    m_currentView = camera.getView();
    m_currentProjection = camera.getProjection();

    // Query visible scene objects
    SceneQuery query;
    scene.getVisibleObjects(camera, query);

    // Send view/projection to default shader
    m_forwardShader->setUniform(viewMatrixUniformName, m_currentView);
    m_forwardShader->setUniform(projectionMatrixUniformName, m_currentProjection);

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
            loge("Invalid scene object id %l.", id);
        }
        else
        {
            // Resolve ids
            Mesh *mesh = manager.getMesh(meshId);
            Material *material = manager.getMaterial(materialId);

            // Create matrices
            Transformer transformer;
            transformer.setPosition(position);
            transformer.setRotation(rotation);
            transformer.setScale(scale);

            // Forward draw call
            draw(mesh, transformer.getTranslationMatrix(), transformer.getRotationMatrix(),
                 transformer.getScaleMatrix(), material, manager);
        }
    }

    // Post draw error check
    std::string error;
    if (hasGLError(error))
    {
        loge("GL Error: {}", error.c_str());
    }
}

ForwardRenderer *ForwardRenderer::create(IResourceManager &manager)
{
    ForwardRenderer *renderer = new ForwardRenderer;
    if (!renderer->init(manager))
    {
        delete renderer;
        renderer = nullptr;
        loge("Failed to create forward renderer, initialization failed.");
    }
    return renderer;
}

void ForwardRenderer::draw(Mesh *mesh, const glm::mat4 &translation, const glm::mat4 &rotation,
                            const glm::mat4 &scale, Material *material, const IGraphicsResourceManager &manager)
{
    // Transformation matrices
    m_forwardShader->setUniform(translationMatrixUniformName, translation);
    m_forwardShader->setUniform(rotationMatrixUniformName, rotation);
    m_forwardShader->setUniform(scaleMatrixUniformName, scale);
    m_forwardShader->setUniform(modelMatrixUniformName, translation * rotation * scale);

    // Send material textures to shader

    // Diffuse texture (base color)
    if (material->hasDiffuse())
    {
        material->getDiffuse()->setActive(diffuseTextureUnit);
    }
    else
    {
        manager.getDefaultDiffuseTexture()->setActive(diffuseTextureUnit);
    }
    m_forwardShader->setUniform(diffuseTextureUniformName, diffuseTextureUnit);

    // Normal texture
    if (material->hasNormal())
    {
        material->getNormal()->setActive(normalTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultNormalTexture()->setActive(normalTextureUnit);
    }
    m_forwardShader->setUniform(normalTextureUniformName, normalTextureUnit);

    // Specular texture
    if (material->hasSpecular())
    {
        material->getSpecular()->setActive(specularTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultSpecularTexture()->setActive(specularTextureUnit);
    }
    m_forwardShader->setUniform(specularTextureUniformName, specularTextureUnit);

    // Glow texture
    if (material->hasGlow())
    {
        material->getGlow()->setActive(glowTextureUnit);
    }
    else
    {
        // Use default texture
        manager.getDefaultGlowTexture()->setActive(glowTextureUnit);
    }
    m_forwardShader->setUniform(glowTextureUniformName, glowTextureUnit);

    // TODO Sort materials with alpha texture for blending.
    // Alpha texture
    if (material->hasAlpha())
    {
        material->getAlpha()->setActive(alphaTextureUnit);
    }
    else
    {
        manager.getDefaultAlphaTexture()->setActive(alphaTextureUnit);
    }
    m_forwardShader->setUniform(alphaTextureUniformName, alphaTextureUnit);

    // Draw mesh
    ::draw(*mesh);
}

bool ForwardRenderer::initShaders(IResourceManager &manager)
{
    std::string defaultShaderFile("data/shader/forward_test_0.ini");

    auto vertexShaderId = manager.createString(getForwardRendererVertexShader());
    auto fragmentShaderId = manager.createString(getForwardRendererFragmentShader());
    m_forwardShaderId =
        manager.createShader(vertexShaderId, invalidResource, invalidResource, invalidResource, fragmentShaderId);

    // Check if ok
    if (m_forwardShaderId == invalidResource)
    {
        loge("Failed to initialize the farward shader.");
        return false;
    }
    return true;
}
