#include "gfx/Font.h"

#include <fmtlog/fmtlog.h>

Font::Font(const std::string& font)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        loge("Could not init FreeType Library");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        loge("Failed to load font");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            loge("Failed to load Glyph");
            continue;
        }

        // generate texture
        GLuint texture = 0;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        // set texture options
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (face->glyph->bitmap.buffer != nullptr)
        {
            // create storage
            glTextureStorage2D(texture, 1, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows);
            // Write data
            glTextureSubImage2D(texture, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED,
                                GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        }
        else
        {
            // Glyph has no data -> not present in our font
            // Create dummy texture with single blank pixel
            uint8_t p = 0;
            glTextureStorage2D(texture, 1, GL_R8, 1, 1);
            glTextureSubImage2D(texture, 0, 0, 0, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &p);
        }

        // now store character for later use
        Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               (unsigned int)face->glyph->advance.x};

        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}