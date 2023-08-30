#pragma once

#include <memory>

#include "kern/graphics/resource/Mesh.h"

class IResourceManager;
class Texture;
class FrameBuffer;
class IGraphicsResourceManager;

/**
 * \brief Represents a draw pass to implement screen space effects.
 * Performs a screen space effect with a texture input and frame buffer output.
 */
class ScreenSpacePass
{
   public:
    /**
     * \brief Performs mesh initialization.
     */
    ScreenSpacePass();

    /**
     * \brief Initialize resources.
     */
    bool init(const std::string &shaderFile, IResourceManager *manager);

    /**
     * \brief Draws screen space quad with shader to render target and supplies
     * textures as shader inputs.
     */
    void draw(const IGraphicsResourceManager *manager, FrameBuffer *fbo,
              Texture *texture0 = nullptr, Texture *texture1 = nullptr,
              Texture *texture2 = nullptr, Texture *texture3 = nullptr);

   private:
    std::unique_ptr<Mesh> m_quad = nullptr; /**< Dummy mesh for fullscreen quad. */
    ResourceId m_shaderId = -1;              /**< Shader program resource id. */
};
