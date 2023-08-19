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
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
   public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices,
         const std::vector<std::shared_ptr<Texture>>& textures);
    ~Mesh();

    void draw(Shader& shader) const;

   private:
    // Vertex array object
    GLuint vao = 0;
    // Vertex buffer, interleaved vertex data
    GLuint vbo = 0;
    // Element buffer, index buffer for vbo
    GLuint ebo = 0;

    void setup();
};