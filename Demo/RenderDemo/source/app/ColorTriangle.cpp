#include "app/ColorTriangle.h"

#include <cmath>

bool ColorTriangle::setup()
{
    const char* vertexCode = R"##(
#version 460 core

layout(location = 0) in vec2 vertexPosition_normalizedScreenSpace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform float xoffset;
uniform float yoffset;

void main(){
gl_Position = vec4(vertexPosition_normalizedScreenSpace, 0.f, 1.f);
gl_Position.x += xoffset;
gl_Position.y += yoffset;
fragmentColor = vertexColor;
}
	)##";

    const char* fragmentCode = R"##(
#version 460 core

in vec3 fragmentColor;
out vec4 color;

uniform float brightness;

void main(){
color = vec4(fragmentColor * brightness, 1.f);
}
	)##";

    // Vertices
    // vec2 position + vec3 rgb
    const GLfloat vertexData[] = {
        -0.5f, -0.5f, 1.f, 0.f, 0.f, 
        0.5f, -0.5f, 0.f, 1.f, 0.f, 
        0.f, 0.5f, 0.f, 0.f, 1.f
    };

    const GLuint indices[] = {
        0, 1, 2
    };

    // Create vertex buffer
    glCreateBuffers(1, &m_vertexBuffer);
    glNamedBufferData(m_vertexBuffer, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Index buffer
    glCreateBuffers(1, &m_indexBuffer);
    glNamedBufferData(m_indexBuffer, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex array object
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    // Setup vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    // Vec2, xy positions in normalized device coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, nullptr);
    // Vec3, RGB vertex colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
    // Setup index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    glBindVertexArray(0);

    m_shader = std::make_unique<Shader>(vertexCode, fragmentCode);
    return true;
}

void ColorTriangle::render()
{
    static float accTime = 0;
    accTime += getTimeDelta();

    // Clear color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw
    m_shader->setActive();
    m_shader->set("brightness", std::abs(std::sin(accTime * 5.f)) / 2.f + 0.5f);
    m_shader->set("xoffset", std::sin(accTime) / 2.f);
    m_shader->set("yoffset", std::cos(accTime) / 2.f);

    glBindVertexArray(m_vertexArray);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

ColorTriangle::~ColorTriangle()
{
    glDeleteVertexArrays(1, &m_vertexArray);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
}