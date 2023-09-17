#pragma once

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/VertexBuffer.h"

class LightingMaps : public RenderApplication
{
   public:
    ~LightingMaps();

   private:
    bool setup() override;
    void render() override;

    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<Texture> m_diffuse;
    std::shared_ptr<Texture> m_specular;
    std::shared_ptr<Texture> m_emission;

    GLuint m_vertexArray = 0;
    glm::vec3 m_lightPos = glm::vec3(1.2f, 1.f, 2.f);
    glm::vec3 m_lightColor = glm::vec3(1.f);
};