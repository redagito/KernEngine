#include "app/SimpleMaterial.h"

//
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

//
#include "gfx/BasicMeshes.h"
#include "gfx/PhongColorMaterial.h"

bool SimpleMaterial::setup()
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

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    // final
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.f);
}
	)##";

    // For light source, always white
    const char* fragmentCodeLightSource = R"##(
#version 460 core

out vec4 fragColor;

uniform vec3 color;

void main(){
    fragColor = vec4(color, 1.f);
}
	)##";

    m_modelShader = std::make_unique<Shader>(vertexCode, fragmentCode);
    m_lightShader = std::make_unique<Shader>(vertexCode, fragmentCodeLightSource);

    m_vertexBuffer = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices) / sizeof(float), std::vector<unsigned int>{3, 2, 3});

    glCreateVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    m_vertexBuffer->bind(GL_ARRAY_BUFFER);

    // Buffer has x/y/z/u/v/x/y/z
    // Location 0, vertex data x/y/z
    glEnableVertexAttribArray(0);
    auto attr = m_vertexBuffer->getAttributes();
    glVertexAttribPointer(0, attr.at(0).componentCount, GL_FLOAT, false, m_vertexBuffer->getStride(), (void*)attr.at(0).offset);

    // Location 1, normal data
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GLfloat) * 8, (void*)(5 * sizeof(float)));
    glVertexAttribPointer(1, attr.at(2).componentCount, GL_FLOAT, false, m_vertexBuffer->getStride(),
                          (void*)attr.at(2).offset);

    // uv data is ignored
    glBindVertexArray(0);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void SimpleMaterial::render()
{
    // Update light movement and color
    m_lightPos.y = std::sin(getTimeTotal() * 0.3f) * 2.f;
    m_lightColor.x = std::abs(std::sin(getTimeTotal() * 2.f));
    m_lightColor.y = std::abs(std::sin(getTimeTotal() * 0.7f));
    m_lightColor.z = std::abs(std::sin(getTimeTotal() * 1.3f));

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.f), getWindow().getAspectRation(), 0.001f, 100.f);

    glm::vec3 cubePos(0.f);

    // Draw cube model
    m_modelShader->setActive();
    // Material
    auto mat = PhongColorMaterial::getMaterial("gold");
    mat.set(*m_modelShader);
    //m_modelShader->set("material.ambient", glm::vec3(1.f, 0.5f, 0.31f));
    //m_modelShader->set("material.diffuse", glm::vec3(1.f, 0.5f, 0.31f));
    //m_modelShader->set("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    //m_modelShader->set("material.shininess", 64.f);

    // Light
    m_modelShader->set("light.ambient", glm::vec3(1.f)); // m_lightColor * 0.2f);
    m_modelShader->set("light.diffuse", glm::vec3(1.f));  // m_lightColor * 0.5f);
    m_modelShader->set("light.specular", glm::vec3(1.f));  // m_lightColor);
    m_modelShader->set("light.position", m_lightPos);

    m_modelShader->set("viewPos", getCamera().getPosition());

    m_modelShader->set("model", glm::translate(cubePos));
    m_modelShader->set("view", getCamera().getView());
    m_modelShader->set("projection", projection);

    glBindVertexArray(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw light model
    m_lightShader->setActive();

    m_lightShader->set("color", m_lightColor);
    m_lightShader->set("model", glm::scale(glm::translate(m_lightPos), glm::vec3(0.3f)));
    m_lightShader->set("view", getCamera().getView());
    m_lightShader->set("projection", projection);

    // glBindVertexArray(m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

SimpleMaterial::~SimpleMaterial()
{
    glDeleteVertexArrays(1, &m_vertexArray);
}