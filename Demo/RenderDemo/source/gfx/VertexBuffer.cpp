#include "gfx/VertexBuffer.h"

#include <stdexcept>

VertexBuffer::VertexBuffer(const float* data, std::size_t size, const std::vector<unsigned int>& componentsPerAttribute)
{
    // Buffer can have multiple interleaved attributes with varying component counts
    // One entry has 1-n attributes
    for (auto components : componentsPerAttribute)
    {
        if (components > 4)
            throw std::runtime_error("Invalid number of components");
        AttributeDescriptor desc;
        desc.componentCount = components;
        desc.offset = m_entrySize * sizeof(float);
        m_entrySize += components;
        m_attributes.push_back(desc);
    }

    if (size % m_entrySize != 0)
        throw std::runtime_error("Invalid number of components for size");

    m_entries = size / m_entrySize;

    // To bytes per entry
    m_entrySize *= sizeof(float);
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size * sizeof(float), data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    //
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind(GLenum target)
{
    //
    glBindBuffer(target, m_id);
}

const std::vector<VertexBuffer::AttributeDescriptor>& VertexBuffer::getAttributes() const { return m_attributes; }

unsigned int VertexBuffer::getEntryCount() const { return m_entries; }

unsigned int VertexBuffer::getStride() const { return m_entrySize; }

GLenum VertexBuffer::getType() const
{ return GL_FLOAT; }