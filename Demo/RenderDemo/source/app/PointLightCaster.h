#pragma once

#include "RenderApplication.h"
#include "gfx/PointLight.h"
#include "gfx/Shader.h"
#include "gfx/VertexArrayObject.h"
#include "gfx/VertexBuffer.h"

class PointLightCaster : public RenderApplication
{
   public:
    ~PointLightCaster() = default;

   private:
    bool setup() override;
    void render() override;

    std::vector<glm::vec3> m_cubePositions;
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<Texture> m_diffuse;
    std::shared_ptr<Texture> m_specular;
    VertexArrayObject m_vao;

    PointLight m_pointLight;

    glm::vec3 m_lightPos = glm::vec3(1.2f, 1.f, 2.f);
    glm::vec3 m_lightColor = glm::vec3(1.f);
};