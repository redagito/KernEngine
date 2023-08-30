#include "kern/graphics/renderer/core/IndexBuffer.h"

#include <cassert>

#include <fmtlog/fmtlog.h>

#include "kern/graphics/renderer/debug/RendererDebug.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned int> &indices, GLenum usage)
    : m_bufferId(0), m_size(0), m_valid(false)
{
    if (indices.empty())
    {
        loge("Index buffer data is empty.");
        return;
    }
    glGenBuffers(1, &m_bufferId);
    // Unchecked bind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
    // Set data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 usage);
    m_size = (unsigned int)indices.size();
    // TODO Check error
    // Unbind
    setInactive();

    // Error check
    std::string error;
    if (hasGLError(error))
    {
        loge("GL Error: {}", error.c_str());
        return;
    }
    m_valid = true;
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_bufferId); }

void IndexBuffer::setActive() const
{
    assert(isValid());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
}

void IndexBuffer::setInactive() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

bool IndexBuffer::isValid() const { return m_valid; }

GLuint IndexBuffer::getId() const { return m_bufferId; }

unsigned int IndexBuffer::getSize() const { return m_size; }