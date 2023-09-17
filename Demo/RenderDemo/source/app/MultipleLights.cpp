#include "app/MultipleLights.h"

//
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

//
#include "gfx/BasicMeshes.h"
#include "gfx/PhongColorMaterial.h"

bool MultipleLights::setup()
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

// Spot light
struct SpotLight
{
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec3 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform Material material;

uniform DirectionalLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.f);

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

    // Combine
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.f);

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // Combine
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords)) * attenuation;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords)) * attenuation;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords)) * attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    
    if (theta > light.outerCutOff)
    {
        // Inside outer light cone, do normal light calculation
        float epsilon = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

        // diffuse
        float diff = max(dot(normal, lightDir), 0.f);

        // specular
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

        // attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        // combine
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords)) * attenuation * intensity;
        vec3 diffuse = diff * vec3(texture(material.diffuse, texCoords)) * light.diffuse * attenuation * intensity;
        vec3 specular = spec * vec3(texture(material.specular, texCoords)) * light.specular * attenuation * intensity;

        return ambient + diffuse + specular;
    }
    return vec3(0.f);
}

void main() 
{
    vec3 norm = normalize(normal);
    vec3 viewDir =normalize(viewPos - fragPos);

    // Directional light
    vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);
    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        result += calculatePointLight(pointLights[i], norm, fragPos, viewDir);
    }

    // Spot Light
    result += calculateSpotLight(spotLight, norm, fragPos, viewDir);

    fragColor = result;
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
    m_dirLight.ambient = glm::vec3(0.1);
    m_dirLight.diffuse = glm::vec3(0.3);
    m_dirLight.specular = glm::vec3(0.9);
    m_dirLight.direction = glm::vec3(0.3f, -1.f, -0.1f);

    glm::vec3 positions[4] = { 
        glm::vec3(0.7f, 0.2f, 2.f),
        glm::vec3(2.3f, -3.3f, -4.f),
        glm::vec3(-4.f, 2.f, -12.f),
        glm::vec3(0.f, 0.f, -3.f),
    };

    for (int i = 0; i < 4; ++i)
    {
        m_pointLights[i].ambient = glm::vec3(0.01);
        m_pointLights[i].diffuse = glm::vec3(0.2);
        m_pointLights[i].specular = glm::vec3(0.7);
        m_pointLights[i].position = positions[i];
    }

    m_spotLight.ambient = glm::vec3(0.01f);
    m_spotLight.diffuse = glm::vec3(0.4f);
    m_spotLight.specular = glm::vec3(1.f);
    m_spotLight.position = getCamera().getPosition();
    m_spotLight.direction = getCamera().getDirection();

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

void MultipleLights::render()
{
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube model
    m_modelShader->setActive();

    // Material
    m_modelShader->setTexture("material.diffuse", *m_diffuse, 0);
    m_modelShader->setTexture("material.specular", *m_specular, 1);
    m_modelShader->set("material.shininess", 64.f);

    // Lights
    // Update
    m_spotLight.position = getCamera().getPosition();
    m_spotLight.direction = getCamera().getDirection();

    m_dirLight.set(*m_modelShader, "dirLight");
    for (int i = 0; i < 4; ++i)
    {
        m_pointLights[i].set(*m_modelShader, "pointLights[" + std::to_string(i) + "]");
    }
    m_spotLight.set(*m_modelShader, "spotLight");

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

    // Draw lights as small white cubes
    m_lightShader->setActive();
    for (int i = 0; i < 4; ++i)
    {
        m_lightShader->set("color", glm::vec3(1.f));
        m_lightShader->set("model", glm::scale(glm::translate(m_pointLights[i].position), glm::vec3(0.25f)));
        m_lightShader->set("view", getCamera().getView());
        m_lightShader->set("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}