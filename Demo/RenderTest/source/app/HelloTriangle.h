#pragma once

#include <memory>

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/VertexBuffer.h"

// Displays 2 triangles
class HelloTriangle : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    GLuint vertexBuffer = 0;
    GLuint vertexArray = 0;

    bool setup() override;

    void render() override;

   public:
    ~HelloTriangle();
};