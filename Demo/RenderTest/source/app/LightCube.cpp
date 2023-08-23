#include "app/LightCube.h"

#include "gfx/BasicMeshes.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

bool LightCube::setup()
{
    // For model and light source
    const char* vertexCode = R"##(
#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
	)##";

    // For model
    const char* fragmentCode = R"##(
#version 460 core

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
    fragColor = vec4(objectColor * lightColor, 1.f);
}
	)##";

    // For light source, always white
    const char* fragmentCodeLightSource = R"##(
#version 460 core

out vec4 fragColor;

void main(){
    fragColor = vec4(1.f);
}
	)##";
    m_modelShader = std::make_unique<Shader>(vertexCode, fragmentCode);
    m_lightShader = std::make_unique<Shader>(vertexCode, fragmentCodeLightSource);

    // Camera setup
    m_camera.setPosition(glm::vec3(2.f, 0.f, 5.f));
    m_camera.setUp(glm::vec3(0.f, 1.f, 0.f));
    m_camera.setLookAt(glm::vec3(0.f));

    glCreateBuffers(1, &m_vertexBuffer);
    glNamedBufferData(m_vertexBuffer, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

    // Buffer has x/y/z/u/v/x/y/u
    // Location 0, vertex data x/y/z
    // uv and normal data is ignored
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 8, nullptr);

    // Rest gets ignored
    glBindVertexArray(0);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    return true;
}

void LightCube::render()
{
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.f), getWindow().getAspectRation(), 0.1f, 100.f);

    glm::vec3 cubePos(0.f);
    glm::vec3 lightPos(1.2f, 1.f, 1.f);

    // Draw cube model
    m_modelShader->setActive();
    m_modelShader->set("objectColor", glm::vec3(1.f, 0.5f, 0.31f));
    m_modelShader->set("lightColor", glm::vec3(1.f, 1.f, 1.f));

    m_modelShader->set("model", glm::translate(cubePos));
    m_modelShader->set("view", m_camera.getView());
    m_modelShader->set("projection", projection);

    glBindVertexArray(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw light model
    m_lightShader->setActive();
    m_lightShader->set("model", glm::scale(glm::translate(lightPos), glm::vec3(0.2f)));
    m_lightShader->set("view", m_camera.getView());
    m_lightShader->set("projection", projection);

    // glBindVertexArray(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

LightCube::~LightCube() 
{ 
    glDeleteBuffers(1, &m_vertexBuffer); 
    glDeleteVertexArrays(1, &m_vertexArray);
}