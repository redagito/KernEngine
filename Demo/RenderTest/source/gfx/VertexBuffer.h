#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

class VertexBuffer
{
   public:
    struct AttributeDescriptor
    {
        unsigned int componentCount = 0;
        // Initial offset
        unsigned int offset = 0;
    };
    // From raw data
    VertexBuffer(const float* data, unsigned int size, const std::vector<unsigned int>& componentCountPerAttribute);
    ~VertexBuffer();

    void bind(GLenum target);

    const std::vector<AttributeDescriptor>& getAttributes() const;
    // Total number of entries / indices
    unsigned int getEntryCount() const;
    // Stride between entries in byte
    unsigned int getStride() const;
    GLenum getType() const;

   private:
    std::vector<AttributeDescriptor> m_attributes;

    // One data entry consists of one or multiple components
    // Total number of values in the buffer is m_enries * m_components
    // Total buffer size in bytes is m_entries * m_components * sizeof(componentType)
    // Total number of data entries in the buffer
    GLuint m_id = 0;
    // Number of entries / indices
    unsigned int m_entries = 0;
    // Total size of an entry in bytes
    unsigned int m_entrySize = 0;
};