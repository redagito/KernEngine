#include "app/SimpleMoveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

// Cube vertices, 36 entries
// clang-format off
static const float cubeVertices[] = {
    // x, y, z, u, v
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// clang-format on

bool SimpleMoveCamera::setup()
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

void main()
{
    color = texture(baseTex, texCoord);
}

)##";

    // Shader
    shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    // Camera
    camera.setPosition({0.f, 0.f, 5.f});
    // Look into negative z
    camera.setDirection({0.f, 0.f, -1.f});
    camera.setUp({0.f, 1.f, 0.f});

    // Texture
    texContainer.id = textureIdFromFile("container.jpg", "data/texture/");

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // x/y/z
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 5, nullptr);
    // u/v
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));

    glBindVertexArray(0);

    // Set render state

    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void SimpleMoveCamera::render()
{
    static float timeAcc = 0;
    timeAcc += getTimeDelta();

    // Camera parameters
    float cameraSpeed = 2.f * getTimeDelta();

    glm::vec3 position = camera.getPosition();
    auto direction = camera.getDirection();
    auto right = camera.getRight();

    // Process input for camera movement
    // Forwards, backward
    if (getWindow().getKey(GLFW_KEY_W) == GLFW_PRESS)
        position += direction * cameraSpeed;
    if (getWindow().getKey(GLFW_KEY_S) == GLFW_PRESS)
        position -= direction * cameraSpeed;
    // Left, right
    if (getWindow().getKey(GLFW_KEY_A) == GLFW_PRESS)
        position -= right * cameraSpeed;
    if (getWindow().getKey(GLFW_KEY_D) == GLFW_PRESS)
        position += right * cameraSpeed;

    camera.setPosition(position);

    // Projection
    auto aspectRatio = (float)getWindow().getWidth() / (float)getWindow().getHeight();
    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 100.f);

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    // Use shader
    shader->setActive();
    shader->set("view", camera.getView());
    shader->set("projection", projection);

    // Texture
    // Activate the binding location
    glActiveTexture(GL_TEXTURE0);
    // Bind the actual texture to active location
    glBindTexture(GL_TEXTURE_2D, texContainer.id);
    // Set texture location manually
    shader->set("baseTex", 0);

    glm::vec3 positions[] = {{0.f, 0.f, 0.f},  {2.f, 0.f, 0.f}, {0.f, -2.f, 0.f},
                             {0.f, 2.f, -2.f}, {0.f, 0.f, 2.f}, {3.f, -3.f, 2.f}};

    auto vertexCount = sizeof(cubeVertices) / sizeof(GLfloat) / 5;

    // Draw cubes
    for (const auto position : positions)
    {
        // Model matrix
        glm::mat4 model{1.f};
        model = glm::translate(model, position);

        // Transformation data for shader
        shader->set("model", model);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
    }
    glBindVertexArray(0);
}

SimpleMoveCamera::~SimpleMoveCamera()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}