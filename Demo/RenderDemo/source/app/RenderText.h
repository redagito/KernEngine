#pragma once

#include "RenderApplication.h"
#include "gfx/Shader.h"
#include "gfx/Font.h"

class RenderText : public RenderApplication
{
   private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Font> font;
    GLuint VAO = 0;
    GLuint VBO = 0;

    bool setup() override;

    void render() override;
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color);
   public:
    ~RenderText();
};