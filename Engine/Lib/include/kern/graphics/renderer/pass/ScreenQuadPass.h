#pragma once

#include <memory>

#include "kern/graphics/IGraphicsResourceManager.h"
#include "kern/graphics/renderer/FrameBuffer.h"
#include "kern/graphics/resource/Mesh.h"
#include "kern/graphics/resource/ShaderProgram.h"
#include "kern/graphics/resource/Texture.h"

#include "kern/resource/IResourceManager.h"

/**
 * \brief Draws textured screen space quad.
 */
class ScreenQuadPass
{
   public:
    ScreenQuadPass();

    bool init(IResourceManager &manager);

    /**
     * \brief Draws screen space quad with specified texture to specified frame
     * buffer.
     */
    void draw(Texture *diffuseGlowTexture, Texture *lightTexture, Texture *depthTexture,
              const glm::mat4 &inverseViewProj, FrameBuffer *fbo,
              const IGraphicsResourceManager *manager);

   private:
    std::unique_ptr<Mesh> m_quad = nullptr;
    ShaderProgram *m_shader = nullptr;
    ResourceId m_shaderId = -1;
};