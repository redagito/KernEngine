#include "app/LightingMaps.h"

//
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

//
#include "gfx/BasicMeshes.h"
#include "gfx/PhongColorMaterial.h"

bool LightingMaps::setup()
{
    // For model and light source
    const char* vertexCode = R"##(
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    texCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
	)##";

    // For model
    const char* fragmentCode = R"##(
#version 460 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
    float glow;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    // Textures
    vec3 diffuseTex = vec3(texture(material.diffuse, texCoords));
    vec3 specularTex = vec3(texture(material.specular, texCoords));
    vec3 emissiveTex = vec3(texture(material.emissive, texCoords));

    // ambient
    vec3 ambient = light.ambient * diffuseTex;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = diff * diffuseTex * light.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * specularTex * light.specular;

    // Emission
    vec3 emission = emissiveTex * material.glow;

    // final
    vec3 result = ambient + diffuse + specular + emission;
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

    // Shaders
    m_modelShader = std::make_unique<Shader>(vertexCode, fragmentCode);
    m_lightShader = std::make_unique<Shader>(vertexCode, fragmentCodeLightSource);

    // Mesh
    m_vertexBuffer = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices) / sizeof(float),
                                                    std::vector<unsigned int>{3, 2, 3});

    glCreateVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);
    m_vertexBuffer->bind(GL_ARRAY_BUFFER);

    // Buffer has x/y/z/u/v/x/y/z
    // Location 0, vertex data x/y/z
    auto attr = m_vertexBuffer->getAttributes();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, attr.at(0).componentCount, GL_FLOAT, false, m_vertexBuffer->getStride(),
                          (void*)attr.at(0).offset);

    // Location 1, normal data
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, attr.at(2).componentCount, GL_FLOAT, false, m_vertexBuffer->getStride(),
                          (void*)attr.at(2).offset);

    // Location 2, uv data
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, attr.at(1).componentCount, GL_FLOAT, false, m_vertexBuffer->getStride(),
                          (void*)attr.at(1).offset);

    glBindVertexArray(0);

    // Textures
    m_diffuse = textureFromFile("container2.png", "data/texture/", "diffuse");
    m_specular = textureFromFile("container2_specular.png", "data/texture/", "specular");
    m_emission = textureFromFile("matrix.jpg", "data/texture/", "emission");

    // Initial state setup
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void LightingMaps::render()
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
    m_modelShader->setTexture("material.diffuse", *m_diffuse, 0);
    m_modelShader->setTexture("material.specular", *m_specular, 1);
    m_modelShader->setTexture("material.emissive", *m_emission, 2);
    m_modelShader->set("material.shininess", 64.f);
    m_modelShader->set("material.glow", (1.f + std::sin(getTimeTotal())) / 2.f);

    // Light
    m_modelShader->set("light.ambient", m_lightColor * 0.05f);
    m_modelShader->set("light.diffuse", m_lightColor * 0.5f);
    m_modelShader->set("light.specular", m_lightColor);
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

LightingMaps::~LightingMaps() { 
    glDeleteVertexArrays(1, &m_vertexArray); 
    m_diffuse = nullptr;
    m_emission = nullptr;
    m_specular = nullptr;
}