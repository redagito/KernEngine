#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gfx/Shader.h"
#include "gfx/Texture.h"

// Represents per-vertex data in model space
struct Vertex
{
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 normal = glm::vec3(0.f);
    glm::vec2 texCoords = glm::vec3(0.f);
};

class Mesh
{
   public:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    // Textures
    std::vector<std::shared_ptr<Texture>> m_diffuseTextures;
    std::vector<std::shared_ptr<Texture>> m_specularTextures;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices,
         const std::vector<std::shared_ptr<Texture>>& diffuseTextures,
         const std::vector<std::shared_ptr<Texture>>& specularTextures);
    ~Mesh();

    void draw(Shader& shader) const;

   private:
    // Vertex array object
    GLuint m_vao = 0;
    // Vertex buffer, interleaved vertex data
    GLuint m_vbo = 0;
    // Element buffer, index buffer for vbo
    GLuint m_ebo = 0;
};