#include "app/HelloTriangle.h"

#include <iostream>

bool HelloTriangle::setup()
{
    const char* vertexCode = R"##(
#version 460 core

layout(location = 0) in vec2 vpos_ndc;

in vec3 base_color;
out vec3 fragmentColor;

void main(){
    gl_Position = vec4(vpos_ndc, 0.f, 1.f);
    fragmentColor = base_color;
}
	)##";

    const char* fragmentCode = R"##(
#version 460 core

out vec3 color;

void main(){
    color = vec3(1.f, 0.f, 0.f);
}
	)##";

    // Vertices
    const std::vector<glm::vec2> vertices{{-0.5f, -0.5f}, {0.5f, -0.5f}, {0.f, 0.5f}};

    // Create vertex buffer
    std::vector<unsigned int> components{2};
    m_vertexBuffer = std::make_unique<VertexBuffer>((const float*)vertices.data(), vertices.size() * 2, components);
    //glCreateBuffers(1, &vertexBuffer);
    //glNamedBufferData(vertexBuffer, vertices.size() * sizeof(float) * 2, (float*)vertices.data(), GL_STATIC_DRAW);

    // Vertex array object
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    m_vertexBuffer->bind(GL_ARRAY_BUFFER);

    // Vec2, xy positions in normalized device coordinates
    glEnableVertexAttribArray(0);
    auto attribute = m_vertexBuffer->getAttributes().at(0);
    glVertexAttribPointer(0, attribute.componentCount, m_vertexBuffer->getType(), GL_FALSE, m_vertexBuffer->getStride(), (void*)attribute.offset);
    glBindVertexArray(0);

    m_shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    // Global state
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);

    return true;
}

void HelloTriangle::render()
{
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw
    glBindVertexArray(vertexArray);

    m_shader->setActive();
    m_shader->set("base_color", {1.f, 0.f, 0.f});

    glDrawArrays(GL_TRIANGLES, 0, m_vertexBuffer->getEntryCount());
    glBindVertexArray(0);
}

HelloTriangle::~HelloTriangle() { glDeleteVertexArrays(1, &vertexArray); }