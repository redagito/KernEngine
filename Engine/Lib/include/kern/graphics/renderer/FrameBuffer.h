#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "kern/graphics/renderer/TextureSemantic.h"
#include "kern/graphics/renderer/RendererCoreConfig.h"

class Texture;
class RenderBuffer;

/**
 * \brief Represents a frame buffer (rendering target).
 */
class FrameBuffer
{
   public:
    FrameBuffer();
    ~FrameBuffer();

    GLuint getId() const { return m_fboId; };

    bool init();

    bool isValid() const;

    std::string getState();

    void setActive(GLenum target);
    void setInactive(GLenum target);

    void resize(unsigned int width, unsigned int height);

    void attach(const std::shared_ptr<Texture> &texture, GLenum attachment);
    void attach(const std::shared_ptr<Texture> &texture, GLenum attachment,
                TextureSemantic semantic);
    void attach(const std::shared_ptr<RenderBuffer> &renderBuffer, GLenum attachment);

    /**
     * \brief returns attached texture based on semantic.
     *
     * Not const because the returned texture can be modified.
     */
    std::shared_ptr<Texture> getTexture(TextureSemantic semantic);

    static void setDefaultActive();

   private:
    std::vector<GLenum> m_drawBuffers; /**< Stores draw buffer attachments. */

    std::unordered_map<GLenum, std::shared_ptr<Texture>>
        m_textures; /**< Stores attached textures by attachment type. */
    std::unordered_map<GLenum, std::shared_ptr<RenderBuffer>>
        m_renderBuffers; /**< Stores attached render buffers. */
    std::map<TextureSemantic, std::shared_ptr<Texture>>
        m_texturesBySemantic; /**< Stores textures by their semantic. */

    GLuint m_fboId; /**< Frame buffer id. */
    bool m_valid;   /**< Frame buffer validity. */
};
