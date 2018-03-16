#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "graphics/graphics/renderer/ETextureSemantic.h"
#include "graphics/graphics/renderer/core/RendererCoreConfig.h"

class CTexture;
class CRenderBuffer;

/**
 * \brief Represents a frame buffer (rendering target).
 */
class CFrameBuffer
{
   public:
    CFrameBuffer();
    ~CFrameBuffer();

    GLuint getId() const { return m_fboId; };

    bool init();

    bool isValid() const;

    std::string getState();

    void setActive(GLenum target);
    void setInactive(GLenum target);

    void resize(unsigned int width, unsigned int height);

    void attach(const std::shared_ptr<CTexture> &texture, GLenum attachment);
    void attach(const std::shared_ptr<CTexture> &texture, GLenum attachment,
                ETextureSemantic semantic);
    void attach(const std::shared_ptr<CRenderBuffer> &renderBuffer, GLenum attachment);

    /**
     * \brief returns attached texture based on semantic.
     *
     * Not const because the returned texture can be modified.
     */
    std::shared_ptr<CTexture> getTexture(ETextureSemantic semantic);

    static void setDefaultActive();

   private:
    std::vector<GLenum> m_drawBuffers; /**< Stores draw buffer attachments. */

    std::unordered_map<GLenum, std::shared_ptr<CTexture>>
        m_textures; /**< Stores attached textures by attachment type. */
    std::unordered_map<GLenum, std::shared_ptr<CRenderBuffer>>
        m_renderBuffers; /**< Stores attached render buffers. */
    std::map<ETextureSemantic, std::shared_ptr<CTexture>>
        m_texturesBySemantic; /**< Stores textures by their semantic. */

    GLuint m_fboId; /**< Frame buffer id. */
    bool m_valid;   /**< Frame buffer validity. */
};
