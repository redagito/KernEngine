#pragma once

#include <ft2build.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include FT_FREETYPE_H

#include "gfx/Shader.h"

class Font
{
   public:
    struct Character
    {
        GLuint textureId = 0;      // ID handle of the glyph texture
        glm::ivec2 size;           // Size of glyph
        glm::ivec2 bearing;        // Offset from baseline to left/top of glyph
        unsigned int advance = 0;  // Offset to advance to next glyph
    };

    Font(const std::string& font);

    std::unordered_map<char, Character> characters;
};