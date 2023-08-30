#include "app/BasicLighting.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <fmtlog/fmtlog.h>

#include "gfx/BasicMeshes.h"

bool BasicLighting::setup()
{
    // For model and light source
    const char* vertexCode = R"##(
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
	)##";

    // For model
    const char* fragmentCode = R"##(
#version 460 core

out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform float ambientStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    // ambient
    vec3 ambient = lightColor * ambientStrength;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // final
    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.f);
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

    glCreateBuffers(1, &m_vertexBuffer);
    glNamedBufferData(m_vertexBuffer, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

    // Buffer has x/y/z/u/v/x/y/z
    // Location 0, vertex data x/y/z
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 8, nullptr);

    // Location 1, normal data
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GLfloat) * 8, (void*)(5 * sizeof(float)));

    // uv data is ignored
    glBindVertexArray(0);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void BasicLighting::render()
{
    // Update light movement
    m_lightPos.y = std::sin(getTimeTotal() * 0.3f) * 2.f;

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.f), getWindow().getAspectRation(), 0.001f, 100.f);

    glm::vec3 cubePos(0.f);

    // Draw cube model
    m_modelShader->setActive();
    m_modelShader->set("ambientStrength", 0.1f);
    m_modelShader->set("objectColor", glm::vec3(1.f, 0.5f, 0.31f));
    m_modelShader->set("lightColor", glm::vec3(1.f, 1.f, 1.f));
    m_modelShader->set("lightPos", m_lightPos);
    m_modelShader->set("viewPos", getCamera().getPosition());

    m_modelShader->set("model", glm::translate(cubePos));
    m_modelShader->set("view", getCamera().getView());
    m_modelShader->set("projection", projection);

    glBindVertexArray(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw light model
    m_lightShader->setActive();
    m_lightShader->set("model", glm::scale(glm::translate(m_lightPos), glm::vec3(0.3f)));
    m_lightShader->set("view", getCamera().getView());
    m_lightShader->set("projection", projection);

    // glBindVertexArray(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

BasicLighting::~BasicLighting()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteVertexArrays(1, &m_vertexArray);
}