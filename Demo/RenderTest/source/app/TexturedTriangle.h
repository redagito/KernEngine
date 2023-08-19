#pragma once

#include <memory>

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"

// Triangle with texture applied
class TexturedTriangle : public RenderApplication
{
   private:
    std::unique_ptr<Shader> m_shader;
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;
    GLuint texture = 0;

    bool setup() override;

    void render() override;

   public:
    ~TexturedTriangle();
};