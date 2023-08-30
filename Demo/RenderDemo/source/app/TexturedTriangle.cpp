#include "app/TexturedTriangle.h"

bool TexturedTriangle::setup()
{
    const char* vertexCode = R"##(
#version 460 core

layout (location = 0) in vec2 vp_ndc;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

void main(){
gl_Position = vec4(vp_ndc, 0.f, 1.f);
texCoords = uv;
}
	)##";

    const char* fragmentCode = R"##(
#version 460 core

out vec4 color;

in vec2 texCoords;

uniform sampler2D tex;

void main(){
color = texture(tex, texCoords);
}
	)##";

    // Shader
    m_shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    texture = textureIdFromFile("wall.jpg", "data/texture/");

    // Vertices, x, y, u, v
    const float vertexData[] = {// Top middle
                                0.0f, 0.8f, 0.5f, 1.0f,
                                // Lower left
                                -0.8f, -0.8f, 0.f, 0.f,
                                // Lower right
                                0.8f, -0.8f, 1.f, 0.f};

    // Create vertex buffer
    glCreateBuffers(1, &vertexBuffer);
    glNamedBufferData(vertexBuffer, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Vertex array object
    glGenVertexArrays(1, &vertexArray);

    // Setup VAO state
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Vertex x/y
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

    // Texcoord u/v
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

    glBindVertexArray(0);

    return true;
}

void TexturedTriangle::render()
{
    // Clear color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw
    // Active shader
    m_shader->setActive();

    // VAO state with vertex buffer object state and data
    // Also binds vertex data to the attrobute locations
    glBindVertexArray(vertexArray);

    // Model textures, defaults to texture location 0
    // So this call is actually unnecessary
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
}

TexturedTriangle::~TexturedTriangle()
{
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteTextures(1, &texture);
}