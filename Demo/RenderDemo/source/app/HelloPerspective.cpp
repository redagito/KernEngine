#include "app/HelloPerspective.h"

#include <glm/gtc/matrix_transform.hpp>

bool HelloPerspective::setup()
{
    static const char* vertexCode = R"##(
#version 460 core

layout (location = 0) in vec2 vp_ndc;
layout (location = 1) in vec2 uv;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
texCoord = uv;
    // Matrix multiplication is read left to right
    gl_Position = projection * view * model * vec4(vp_ndc, 0.f, 1.f);
}
)##";

    static const char* fragmentCode = R"##(
#version 460 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D baseTex;
uniform sampler2D blendTex;

void main()
{
    color = mix(texture(baseTex, texCoord), texture(blendTex, texCoord), 0.5f);
}

)##";

    // Shader
    shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    // Texture
    texBase.id = textureIdFromFile("container.jpg", "data/texture/");
    texBlend.id = textureIdFromFile("awesomeface.png", "data/texture/");

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

void HelloPerspective::render()
{
    static float timeAcc = 0;
    timeAcc += getTimeDelta();

    // Model matrix
    glm::mat4 model{1.f};
    // Rotation on x axis
    model = glm::rotate(model, glm::radians(-55.f), {1.f, 0.f, 0.f});

    // View matrix
    glm::mat4 view{1.f};
    view = glm::translate(view, {0.f, 0.f, -3.f});

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.f), getWindow().getAspectRation(), 0.1f, 100.f);

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader
    shader->setActive();

    // Transformation data for shader
    shader->set("model", model);
    shader->set("view", view);
    shader->set("projection", projection);

    // Textures
    // Activate the binding location
    glActiveTexture(GL_TEXTURE0);
    // Bind the actual texture to active location
    glBindTexture(GL_TEXTURE_2D, texBase.id);
    // Set texture location manually
    shader->set("baseTex", 0);

    
    // Activate the binding location
    glActiveTexture(GL_TEXTURE1);
    // Bind the actual texture to active location
    glBindTexture(GL_TEXTURE_2D, texBlend.id);
    // Set texture location manually
    shader->set("blendTex", 1);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

HelloPerspective::~HelloPerspective()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}