#include "app/TransformedQuad.h"

#include <glm/gtc/matrix_transform.hpp>

bool TransformedQuad::setup()
{
    static const char* vertexCode = R"##(
#version 460 core

layout (location = 0) in vec2 vp_ndc;
layout (location = 1) in vec2 uv;

out vec2 texCoord;

uniform mat4 transform;

void main()
{
texCoord = uv;
gl_Position = transform * vec4(vp_ndc, 0.f, 1.f);
}
)##";

    static const char* fragmentCode = R"##(
#version 460 core

uniform sampler2D baseTex;
uniform sampler2D blendTex;
uniform float mixRatio;

in vec2 texCoord;

out vec4 color;

void main()
{
vec4 base = texture(baseTex, texCoord);
vec4 blend = texture(blendTex, texCoord);
color = mix(base, blend, mixRatio);
}

)##";

    // Shader
    shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    // Texture
    texContainer.id = textureIdFromFile("container.jpg", "data/texture/");
    texAwesome.id = textureIdFromFile("awesomeface.png", "data/texture/");

    // Vertex data, x/y, v/v
    const GLfloat vertexData[] = {// First triangle, upper right, upper left, lower left
                                  0.5f, 0.5f, 1.f, 1.f, -0.5f, 0.5f, 0.f, 1.f, -0.5f, -0.5f, 0.f, 0.f,
                                  // Second triangle, upper right, lower left, lower right
                                  0.5f, 0.5f, 1.f, 1.f, -0.5f, -0.5f, 0.f, 0.f, 0.5f, -0.5f, 1.f, 0.f};

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // x/y
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, nullptr);
    // u/v
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 2));

    glBindVertexArray(0);

    // Clear color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);

    return true;
}

void TransformedQuad::render()
{
    // Timer
    static float timeAcc = 0;
    timeAcc += getTimeDelta();

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader
    shader->setActive();

    // Tranformation order: scale, rotate, translate
    // With matrix denotation: T * R * S, applied in reverse order
    glm::mat4 transform = glm::mat4{1.f};
    // Translate on x axis
    transform = glm::translate(transform, {std::sin(timeAcc * 1.5f), 0.f, 0.f});
    // Rotate on Z axis, 10 degrees each second
    transform = glm::rotate(transform, glm::radians(timeAcc * 10.f), {0.f, 0.f, 1.f});
    // Scale on each axis
    transform = glm::scale(transform, glm::vec3{(std::sin(timeAcc) + 1.f) / 2.f});

    // Set transformation
    shader->set("transform", transform);

    // Texture
    // Activate the binding location
    glActiveTexture(GL_TEXTURE0);
    // Bind the actual texture to active location
    glBindTexture(GL_TEXTURE_2D, texContainer.id);
    // Set texture location manually
    shader->set("baseTex", 0);

    // Face texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texAwesome.id);
    shader->set("blendTex", 1);

    // Mix ratio
    shader->set("mixRatio", (std::sin(timeAcc * 3.f) + 1.f) / 2.f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

TransformedQuad::~TransformedQuad()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}