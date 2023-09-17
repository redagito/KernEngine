#pragma once

#include "RenderApplication.h"
#include "gfx/SpotLight.h"
#include "gfx/Shader.h"
#include "gfx/VertexArrayObject.h"
#include "gfx/VertexBuffer.h"

class SpotLightCaster : public RenderApplication
{
   public:
    ~SpotLightCaster() = default;

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

    SpotLight m_spotLight;

    glm::vec3 m_lightPos = glm::vec3(1.2f, 1.f, 2.f);
    glm::vec3 m_lightColor = glm::vec3(1.f);
};