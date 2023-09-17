#include "app/PointLightCaster.h"

//
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

//
#include "gfx/BasicMeshes.h"
#include "gfx/PhongColorMaterial.h"

bool PointLightCaster::setup()
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
    float shininess;
};

// Point light
struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // ambient
    vec3 ambient = light.ambient * diffuseTex * attenuation;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos); // Direction vector from fragment to light source
    float diff = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = diff * diffuseTex * light.diffuse * attenuation;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * specularTex * light.specular * attenuation;

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

    // Shaders
    m_modelShader = std::make_unique<Shader>(vertexCode, fragmentCode);
    m_lightShader = std::make_unique<Shader>(vertexCode, fragmentCodeLightSource);

    // Mesh
    m_vertexBuffer = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices) / sizeof(float),
                                                    std::vector<unsigned int>{3, 2, 3});

    glBindVertexArray(m_vao.getId());
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

    // Light
    m_pointLight.position = glm::vec3(0.f);
    m_pointLight.ambient = m_lightColor * 0.05f;
    m_pointLight.diffuse = m_lightColor * 0.5f;
    m_pointLight.specular = m_lightColor;
    
    // Models
    m_cubePositions.push_back(glm::vec3{2.f, 0.f, 0.f});
    m_cubePositions.push_back(glm::vec3{-2.f, 0.f, 0.f});
    m_cubePositions.push_back(glm::vec3{0.f, 2.f, 0.f});
    m_cubePositions.push_back(glm::vec3{0.f, -2.f, 0.f});
    m_cubePositions.push_back(glm::vec3{2.f, 2.f, 0.f});
    m_cubePositions.push_back(glm::vec3{2.f, -2.f, 0.f});
    m_cubePositions.push_back(glm::vec3{-2.f, 2.f, 0.f});
    m_cubePositions.push_back(glm::vec3{-2.f, -2.f, 0.f});

    m_cubePositions.push_back(glm::vec3{0.f, 0.f, 2.f});
    m_cubePositions.push_back(glm::vec3{2.f, 0.f, 2.f});
    m_cubePositions.push_back(glm::vec3{-2.f, 0.f, 2.f});
    m_cubePositions.push_back(glm::vec3{0.f, 2.f, 2.f});
    m_cubePositions.push_back(glm::vec3{0.f, -2.f, 2.f});
    m_cubePositions.push_back(glm::vec3{2.f, 2.f, 2.f});
    m_cubePositions.push_back(glm::vec3{2.f, -2.f, 2.f});
    m_cubePositions.push_back(glm::vec3{-2.f, 2.f, 2.f});
    m_cubePositions.push_back(glm::vec3{-2.f, -2.f, 2.f});

    m_cubePositions.push_back(glm::vec3{0.f, 0.f, -2.f});
    m_cubePositions.push_back(glm::vec3{2.f, 0.f, -2.f});
    m_cubePositions.push_back(glm::vec3{-2.f, 0.f, -2.f});
    m_cubePositions.push_back(glm::vec3{0.f, 2.f, -2.f});
    m_cubePositions.push_back(glm::vec3{0.f, -2.f, -2.f});
    m_cubePositions.push_back(glm::vec3{2.f, 2.f, -2.f});
    m_cubePositions.push_back(glm::vec3{2.f, -2.f, -2.f});
    m_cubePositions.push_back(glm::vec3{-2.f, 2.f, -2.f});
    m_cubePositions.push_back(glm::vec3{-2.f, -2.f, -2.f});

    // Initial state setup
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void PointLightCaster::render()
{
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube model
    m_modelShader->setActive();

    // Material
    m_modelShader->setTexture("material.diffuse", *m_diffuse, 0);
    m_modelShader->setTexture("material.specular", *m_specular, 1);
    m_modelShader->set("material.shininess", 64.f);

    // Light
    m_pointLight.set(*m_modelShader);

    // Camera
    m_modelShader->set("viewPos", getCamera().getPosition());
    m_modelShader->set("view", getCamera().getView());

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.f), getWindow().getAspectRation(), 0.001f, 100.f);
    m_modelShader->set("projection", projection);

    // Cube vao
    glBindVertexArray(m_vao.getId());

    // Draw models
    for (int i = 0; i < m_cubePositions.size(); ++i)
    {
        glm::mat4 model{1.f};
        model = glm::translate(model, m_cubePositions.at(i));
        // Angle in degree
        float angle = 15.f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, 0.3f, 0.5f));
        m_modelShader->set("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Draw light
    m_lightShader->setActive();

    m_lightShader->set("color", m_lightColor);
    m_lightShader->set("model", glm::scale(glm::translate(m_pointLight.position), glm::vec3(0.3f)));
    m_lightShader->set("view", getCamera().getView());
    m_lightShader->set("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}