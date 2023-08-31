#pragma once

#include <vector>

#include "kern/graphics/renderer/RendererCoreConfig.h"
#include "kern/resource/ResourceId.h"
#include "kern/resource/ColorFormat.h"

/**
 * \brief Texture class.
 */
class Texture
{
   public:
    Texture();

    /**
     * \brief Create from image data.
     */
    Texture(const std::vector<unsigned char> &imageData, unsigned int width, unsigned int height,
             ColorFormat format, bool createMipmaps = true);

    /**
     * \brief Creates empty texture.
     */
    Texture(unsigned int width, unsigned int height, ColorFormat format,
             bool createMipmaps = true);

    // TODO remove this and introduce ability to create cubetextures (necessary
    // for point light
    // shadows)
    Texture(GLint id, bool hasMipmaps, unsigned int width, unsigned int height, GLint format,
             GLenum externalFormat);

    ~Texture();

    /**
     * \brief Initializes texture data.
     */
    bool init(const std::vector<unsigned char> &imageData, unsigned int width, unsigned int height,
              ColorFormat format, bool createMipmaps = true);

    bool init(unsigned int width, unsigned int height, GLint format);

    /**
     * \brief Sets filtering.
     */
    void setParameter(GLenum parameterName, GLint value);

    /**
     * \brief Resizes texture.
     */
    void resize(unsigned int width, unsigned int height);

    /**
     * \brief Returns texture id.
     */
    GLuint getId() const;

    /**
     * \brief Returns texture validity.
     */
    bool isValid() const;

    /**
     * \bref Sets texture active as texture unit.
     */
    void setActive(GLint textureUnit) const;

    /**
     * \brief Saves texture data as png.
     */
    void saveAsPng(const std::string &file);

   protected:
    bool init(const std::vector<unsigned char> &imageData, unsigned int width, unsigned int height,
              GLint format, bool createMipmaps);

   private:
    bool m_valid = false;
    bool m_hasMipmaps = false;
    GLuint m_textureId = 0;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    GLint m_format = 0;
    GLenum m_externalFormat = 0;
};
