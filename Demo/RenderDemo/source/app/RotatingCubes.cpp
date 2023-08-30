#include "app/RotatingCubes.h"
#include "gfx/BasicMeshes.h"

#include <glm/gtc/matrix_transform.hpp>

bool RotatingCubes::setup()
{
    static const char* vertexCode = R"##(
#version 460 core

layout (location = 0) in vec3 vp_ms;
layout (location = 1) in vec2 uv;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
texCoord = uv;
// Matrix multiplication is read left to right
gl_Position = projection * view * model * vec4(vp_ms, 1.f);
}
)##";

    static const char* fragmentCode = R"##(
#version 460 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D baseTex;
uniform sampler2D blendTex;
uniform float mixRatio;

void main()
{
color = mix(
    texture(baseTex, texCoord), 
    texture(blendTex, texCoord), 
    clamp(mixRatio, 0.2, 0.8));
}

)##";

    // Shader
    shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    // Texture
    texContainer.id = textureIdFromFile("wall.jpg", "data/texture/");
    // texAwesome.id = textureIdFromFile("awesomeface.png", "data/texture/");
    texAwesome.id = textureIdFromFile("container.jpg", "data/texture/");

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // x/y/z
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 8, nullptr);
    // u/v
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
    // Ignore normals
    glBindVertexArray(0);

    // Set render state

    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void RotatingCubes::render()
{
    static float timeAcc = 0;
    timeAcc += getTimeDelta();

    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                 glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                 glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                 glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                 glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    // View matrix
    glm::mat4 view{1.f};
    view = glm::translate(view, {0.f, 0.f, -5.f});

    // Projection
    auto aspectRatio = (float)getWindow().getWidth() / (float)getWindow().getHeight();
    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 100.f);

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    // Use shader
    shader->setActive();
    shader->set("view", view);
    shader->set("projection", projection);

    // Texture
    // Activate the binding location
    glActiveTexture(GL_TEXTURE0);
    // Bind the actual texture to active location
    glBindTexture(GL_TEXTURE_2D, texContainer.id);
    // Set texture location manually
    shader->set("baseTex", 0);

    // Blend texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texAwesome.id);
    shader->set("blendTex", 1);

    // Same cube with different positions
    float i = 0.f;
    for (const auto& position : cubePositions)
    {
        // Model matrix, different rotation and translation for each cube
        glm::mat4 model{1.f};
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(timeAcc * (5.f + i)), {1.f, 0.3f, 0.7f});

        // Transformation data for shader
        shader->set("model", model);

        // Mix ratio
        shader->set("mixRatio", (std::sin(timeAcc * (1.f + i)) + 1.f) / 2.f);

        auto vertexCount = sizeof(cubeVertices) / sizeof(GLfloat) / 8;
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
        i += 1.f;
    }
    glBindVertexArray(0);
}

RotatingCubes::~RotatingCubes()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}