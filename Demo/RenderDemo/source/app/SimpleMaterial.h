#pragma once

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/VertexBuffer.h"

class SimpleMaterial : public RenderApplication
{
   public:
    ~SimpleMaterial();

   private:
    bool setup() override;
    void render() override;

    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    GLuint m_vertexArray = 0;
    glm::vec3 m_lightPos = glm::vec3(1.2f, 1.f, 2.f);
    glm::vec3 m_lightColor = glm::vec3(1.f);
};