#include "kern/graphics/renderer/RenderBuffer.h"

RenderBuffer::RenderBuffer() {}

RenderBuffer::~RenderBuffer()
{
    if (m_init)
    {
        glDeleteRenderbuffers(1, &m_bufferId);
    }
}

bool RenderBuffer::init(unsigned int width, unsigned int height, GLenum format)
{
    if (m_init)
    {
        glDeleteRenderbuffers(1, &m_bufferId);
    }
    glGenRenderbuffers(1, &m_bufferId);
    setActive();
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    m_width = width;
    m_height = height;
    m_format = format;
    return true;
}

bool RenderBuffer::resize(unsigned int width, unsigned int height)
{
    if (m_width != width || m_height != height)
    {
        init(width, height, getFormat());
        return true;
    }
    return false;
}

GLuint RenderBuffer::getId() const { return m_bufferId; }

GLenum RenderBuffer::getFormat() const { return m_format; }

void RenderBuffer::setActive() const { glBindRenderbuffer(GL_RENDERBUFFER, m_bufferId); }
