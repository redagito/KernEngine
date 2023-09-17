#include "Mesh.h"

#include <cstddef>
#include <stdexcept>
#include <fmtlog/fmtlog.h>

// |x y z u  v| x  y  z  u  v |x  y  z  u  v
//  1 2 3 4  5  6  7  8  9  10 11 12 13 14 15
//  0 4 8 12 16 20 24 28 32 36 40 44 48 52 56
// xyz - attribute 1, 3 * 4 bytes = 12 byte, offset 0
// uv - attribute 2, 2 * 4 byte = 8 byte, offset 3 * 4 = 12
// xyzuv - attribute block, 5 * 4 = 20 byte = stride
// position of attribute element at index i = offset + stride * i
// Example: attribute uv, index 2 position in buffer = 12 + 20 * 2 = 48 bytes from start

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices,
           const std::vector<std::shared_ptr<Texture>>& textures)
    : m_vertices(vertices), m_indices(indices), m_textures(textures)
{
    logi("Creating mesh with {} vertices and {} indices", m_vertices.size(), m_indices.size());

    // VAO Stores buffer states
    glGenVertexArrays(1, &m_vao);
    // Per-vertex data buffer
    glGenBuffers(1, &m_vbo);
    // Index buffer
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    // Write data
    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(decltype(m_vertices)::value_type), m_vertices.data(),
                 GL_STATIC_DRAW);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(decltype(m_indices)::value_type), m_indices.data(),
                 GL_STATIC_DRAW);

    // Set attributes
    // Vertex buffer has attributes for vertices, normals and texcoords
    // Layout is v0, v1, v2, n0, n1, n2, t0, t1
    // All values are float
    // Vertices at index 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // Normals at index 1, requires offset
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture coordinates at index 2, requires offset
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Disable vao
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Mesh::draw(Shader& shader) const
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    // Bind all textures in the mesh to the shader
    for (GLint i = 0; i < m_textures.size(); ++i)
    {
        // Activate texture unit
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures.at(i)->type;

        // Texture type
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr);
            ++diffuseNr;
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr);
            ++specularNr;
        }
        else
        {
            throw std::runtime_error{"Invalid texture type"};
        }

        // Bind textures to shader
        // Set sampler to texture unit
        auto uniformName = /*"material." +*/ name + number;

        shader.set(uniformName, i);
        // Bind texture to index
        glBindTexture(GL_TEXTURE_2D, m_textures.at(i)->id);
    }

    // Reset active texture unit
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}