#include "kern/graphics/renderer/pass/ScreenSpacePass.h"

// Debugging
#include <fmtlog/fmtlog.h>

// Resource management
#include "kern/resource/IResourceManager.h"

// Graphics API
#include "kern/graphics/IGraphicsResourceManager.h"
#include "kern/graphics/renderer/FrameBuffer.h"
#include "kern/graphics/renderer/RendererCoreConfig.h"
#include "kern/graphics/resource/ShaderProgram.h"
#include "kern/graphics/resource/Texture.h"

ScreenSpacePass::ScreenSpacePass()
{
    std::vector<float> vertices = {0.f, 0.f, 0.f};
    std::vector<unsigned int> indices = {1};
    std::vector<float> normals = {0.f, 1.f, 0.f};
    std::vector<float> uvs = {0.f, 0.f, 0.f};
    m_quad.reset(new Mesh(vertices, indices, normals, uvs, PrimitiveType::Point));
}

bool ScreenSpacePass::init(const std::string &shaderFile, IResourceManager *manager)
{
    logi("Initializing screen space pass with shader {}.", shaderFile.c_str());
    m_shaderId = manager->loadShader(shaderFile);
    if (m_shaderId == InvalidResource)
    {
        loge("Failed to initialize the screen space pass from shader {}.", shaderFile.c_str());
        return false;
    }
    return true;
}

void ScreenSpacePass::draw(const IGraphicsResourceManager *manager, FrameBuffer *fbo, Texture *texture0,
                           Texture *texture1, Texture *texture2, Texture *texture3)
{
    if (m_shaderId == InvalidResource)
    {
        loge("The screen space pass shader id is not valid.");
        return;
    }

    // Fetch shader based on id.
    // TODO Cache shader and only fetch on change (make listener)?
    ShaderProgram *shader = manager->getShaderProgram(m_shaderId);
    if (shader == nullptr)
    {
        loge("Failed to load screen space pass shader with id %.");
        return;
    }

    // Set rendering target
    if (fbo == nullptr)
    {
        // Default FBO
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    {
        // Use supplied FBO
        fbo->setActive(GL_FRAMEBUFFER);
    }

    // Screen space shader does not use depth testing
    // TODO Cache depth testing state.
    glDisable(GL_DEPTH_TEST);

    shader->setActive();
    // Set textures active if not null
    if (texture0 != nullptr)
    {
        texture0->setActive(0);
        shader->setUniform("texture0", 0);
    }
    if (texture1 != nullptr)
    {
        texture1->setActive(1);
        shader->setUniform("texture1", 1);
    }
    if (texture2 != nullptr)
    {
        texture2->setActive(2);
        shader->setUniform("texture2", 2);
    }
    if (texture3 != nullptr)
    {
        texture3->setActive(3);
        shader->setUniform("texture3", 3);
    }

    m_quad->getVertexArray()->setActive();
    glDrawArrays(GL_POINTS, 0, 1);
    m_quad->getVertexArray()->setInactive();
    shader->setInactive();

    if (fbo != nullptr)
    {
        fbo->setInactive(GL_FRAMEBUFFER);
    }
    glEnable(GL_DEPTH_TEST);
}
