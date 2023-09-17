#include "kern/graphics/renderer/FrameBuffer.h"

#include <cassert>

#include "kern/graphics/renderer/RenderBuffer.h"
#include "kern/graphics/resource/Texture.h"

FrameBuffer::FrameBuffer() : m_fboId(0), m_valid(false) { init(); }

FrameBuffer::~FrameBuffer()
{
    if (m_valid)
    {
        glDeleteFramebuffers(1, &m_fboId);
    }
}

bool FrameBuffer::init()
{
    GLuint bufferId;
    glGenFramebuffers(1, &bufferId);

    m_fboId = bufferId;
    m_valid = true;

    return m_valid;
}

bool FrameBuffer::isValid() const { return m_valid; }

std::string FrameBuffer::getState()
{
    assert(m_valid);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    GLenum state = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch (state)
    {
    case GL_FRAMEBUFFER_UNDEFINED:
        return "The current FBO binding is 0, but no default framebuffer exists";
        break;
    case GL_FRAMEBUFFER_COMPLETE:
        return "A user-defined FBO is bound and is complete. OK to render";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "One of the buffers enabled for rendering is incomplete";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "No buffers are attached to the FBO.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        return "One of the buffer attachments enabled for rendering does not have "
               "a buffer "
               "attached";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        return "One of the buffer attachments enabled for reading does not have a "
               "buffer attached";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "The combination of internal buffer formats is not supported";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        return "The number of samples or the value for "
               "TEXTURE_FIXED_SAMPLE_LOCATIONS for all "
               "buffers does not match";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        return "Not all color attachments are layered textures or bound to the "
               "same target";
    }
    return "Invalid state encountered";
}

void FrameBuffer::setActive(GLenum target)
{
    assert(m_valid);
    glBindFramebuffer(target, m_fboId);
    // Set draw buffers
    if (!m_drawBuffers.empty())
    {
        glDrawBuffers((GLsizei)m_drawBuffers.size(), m_drawBuffers.data());
    }
}

void FrameBuffer::setInactive(GLenum target) { glBindFramebuffer(target, 0); }

void FrameBuffer::resize(unsigned int width, unsigned int height)
{
    for (auto &entry : m_textures)
    {
        entry.second->resize(width, height);
    }

    for (auto &entry : m_renderBuffers)
    {
        // Need renderbuffer connot be resized
        // Instead they are recreated and reattached
        if (entry.second->resize(width, height))
        {
            attach(entry.second, entry.first);
        }
    }
}

void FrameBuffer::attach(const std::shared_ptr<Texture> &texture, GLenum attachment,
                          TextureSemantic semantic)
{
    // Bind
    assert(m_valid);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    // Attach
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture->getId(), 0);
    // Add color attachments to draw buffers
    if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT &&
        attachment != GL_DEPTH_STENCIL_ATTACHMENT)
    {
        m_drawBuffers.push_back(attachment);
    }
    m_textures[attachment] = texture;
    if (semantic != TextureSemantic::Unknown)
    {
        m_texturesBySemantic[semantic] = texture;
    }
    setInactive(GL_FRAMEBUFFER);
}

void FrameBuffer::attach(const std::shared_ptr<Texture> &texture, GLenum attachment)
{
    TextureSemantic semantic = TextureSemantic::Unknown;
    // On depth attachment automatically set semantic
    if (attachment == GL_DEPTH_ATTACHMENT)
    {
        semantic = TextureSemantic::Depth;
    }
    attach(texture, attachment, semantic);
}

void FrameBuffer::setDefaultActive() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::attach(const std::shared_ptr<RenderBuffer> &renderBuffer, GLenum attachment)
{
    // Bind
    assert(m_valid);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    // Attach
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer->getId());
    // Add color attachments to draw buffers
    if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT &&
        attachment != GL_DEPTH_STENCIL_ATTACHMENT)
    {
        m_drawBuffers.push_back(attachment);
    }
    m_renderBuffers[attachment] = renderBuffer;
    setInactive(GL_FRAMEBUFFER);
}

std::shared_ptr<Texture> FrameBuffer::getTexture(TextureSemantic semantic)
{
    // TODO Slow, fix this
    if (m_texturesBySemantic.count(semantic) == 0)
    {
        return nullptr;
    }
    return m_texturesBySemantic.at(semantic);
}