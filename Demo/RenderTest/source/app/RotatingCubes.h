#pragma once

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/Texture.h"

class RotatingCubes : public RenderApplication
{
   private:
    std::unique_ptr<Shader> shader;
    Texture texContainer;
    Texture texAwesome;
    GLuint vao = 0;
    GLuint vbo = 0;

    bool setup() override;

    void render() override;

   public:
    ~RotatingCubes();
};