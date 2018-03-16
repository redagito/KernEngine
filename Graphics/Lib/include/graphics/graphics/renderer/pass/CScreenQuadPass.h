#pragma once

#include <memory>

#include "graphics/graphics/IGraphicsResourceManager.h"
#include "graphics/graphics/renderer/CFrameBuffer.h"
#include "graphics/graphics/resource/CMesh.h"
#include "graphics/graphics/resource/CShaderProgram.h"
#include "graphics/graphics/resource/CTexture.h"

#include "graphics/resource/IResourceManager.h"

/**
 * \brief Draws textured screen space quad.
 */
class CScreenQuadPass
{
   public:
    CScreenQuadPass();

    bool init(IResourceManager &manager);

    /**
     * \brief Draws screen space quad with specified texture to specified frame
     * buffer.
     */
    void draw(CTexture *diffuseGlowTexture, CTexture *lightTexture, CTexture *depthTexture,
              const glm::mat4 &inverseViewProj, CFrameBuffer *fbo,
              const IGraphicsResourceManager *manager);

   private:
    std::unique_ptr<CMesh> m_quad = nullptr;
    CShaderProgram *m_shader = nullptr;
    ResourceId m_shaderId = -1;
};