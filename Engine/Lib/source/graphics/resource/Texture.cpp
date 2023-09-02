#include "kern/graphics/resource/Texture.h"

#include <cassert>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <fmtlog/fmtlog.h>
#include <stb_image_write.h>

Texture::Texture()
{
    // empty
}

Texture::Texture(const std::vector<unsigned char> &image, unsigned int width, unsigned int height, ColorFormat format,
                 bool createMipmaps)
{
    // Init texture with data
    if (!init(image, width, height, format, createMipmaps))
    {
        loge("Failed to initialize texture.");
    }
}

Texture::Texture(unsigned int width, unsigned int height, ColorFormat format, bool createMipmaps)
{
    // Init texture with data
    if (!init({}, width, height, format, createMipmaps))
    {
        loge("Failed to initialize texture.");
    }
}

Texture::Texture(GLuint id, bool hasMipmaps, unsigned int width, unsigned int height, GLint format,
                 GLenum externalFormat)
{
    m_valid = true;
    m_hasMipmaps = hasMipmaps;
    m_textureId = id;
    m_width = width;
    m_height = height;
    m_format = format;
    m_externalFormat = externalFormat;
}

Texture::~Texture()
{
    if (m_valid)
    {
        glDeleteTextures(1, &m_textureId);
    }
}

bool Texture::init(const std::vector<unsigned char> &image, unsigned int width, unsigned int height, ColorFormat format,
                   bool createMipmaps)
{
    // Set format
    GLint internalFormat;
    switch (format)
    {
    case ColorFormat::GreyScale8:
        internalFormat = GL_R8;
        break;
    case ColorFormat::RGB24:
        internalFormat = GL_RGB8;
        break;
    case ColorFormat::RGBA32:
        internalFormat = GL_RGBA8;
        break;
    default:
        return false;
        break;
    }
    return init(image, width, height, internalFormat, createMipmaps);
}

bool Texture::init(unsigned int width, unsigned int height, GLint format)
{
    return init({}, width, height, format, false);
}

void Texture::resize(unsigned int width, unsigned int height)
{
    // TODO Remove resizing functionality
    if (m_width == width && m_height == height)
    {
        return;
    }
    logd("Texture resize from {}, {} to {}, {}.", m_width, m_height, width, height);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_externalFormat, GL_UNSIGNED_BYTE, nullptr);
    m_width = width;
    m_height = height;
}

GLuint Texture::getId() const { return m_textureId; }

bool Texture::isValid() const { return m_valid; }

void Texture::setActive(GLint textureUnit) const
{
    assert(isValid());
    glBindTextureUnit(textureUnit, m_textureId);
}

void Texture::saveAsPng(const std::string &file)
{
    std::vector<unsigned char> image;
    image.resize(m_width * m_height * 3);
    glGetTextureImage(m_textureId, 0, GL_RGB, GL_UNSIGNED_BYTE, image.size(), image.data());

    if (stbi_write_png(file.c_str(), m_width, m_height, 3, image.data(), m_width * 3) != 1)
    {
        loge("Failed to save png");
    }
}

bool Texture::init(const std::vector<unsigned char> &image, unsigned int width, unsigned int height, GLint format,
                   bool createMipmaps)
{
    // Sanity checks
    if (width == 0 || height == 0)
    {
        return false;
    }

    // Set format
    m_format = format;
    unsigned int bytePerPixel = 0;
    m_hasMipmaps = createMipmaps;
    // External data type
    GLenum type = GL_UNSIGNED_BYTE;

    // Set external format and type, based on texture format
    switch (format)
    {
    case GL_R8:  // Red component only, used for greyscale
        m_externalFormat = GL_RED;
        bytePerPixel = 1;
        break;
    case GL_RG8:  // Red and green component
        m_externalFormat = GL_RG;
        bytePerPixel = 2;
        break;
    case GL_RGB:  // RGB texture with values from 0-255
    case GL_RGB8:
        m_externalFormat = GL_RGB;
        bytePerPixel = 3;
        break;
    case GL_DEPTH_COMPONENT24:  // Depth texture for FBO
        m_externalFormat = GL_DEPTH_COMPONENT;
        bytePerPixel = 3;
        break;
    case GL_RGBA:  // RGB texture with alpha channel
    case GL_RGBA8:
        m_externalFormat = GL_RGBA;
        bytePerPixel = 4;
        break;
    case GL_RGB16F:  // RGB texture with half float precision, for hdr
        m_externalFormat = GL_RGB;
        bytePerPixel = 6;
        break;
    case GL_RGBA16F:  // RGB texture with alpha channel with half float precision
        m_externalFormat = GL_RGBA;
        bytePerPixel = 8;
        break;
    default:
        return false;
        break;
    }

    // Image data is either empty or has correct size
    if (!image.empty() && (image.size() != width * height * bytePerPixel))
    {
        return false;
    }

    // Create id
    GLuint textureId = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

    // TODO Filter should be based on arguments and mip map level
    // Set filters
    if (width == 1 && height == 1)
    {
        // Special case 1x1 texture
        // NOTE Needs nearest filtering otherwise texture is black?
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else
    {
        if (m_hasMipmaps)
        {
            // Bilinear filtering
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        }
        else
        {
            // Default value is linear filtering
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    }

    // Set wrap mode
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

    int levels = 1;
    if (createMipmaps)
    {
        levels = (int)std::floor(std::log2(std::min(width, height))) + 1;
    }

    // Use glTexImage for mutable storage for resizing later
    // TODO Disallow resizing of textures / create completely new texture on resize
    // NOTE Actually disallowing and forcing a new texture to be created would be better?
    // TODO For FBO resize, the FBO would need to be recreated completely
    glBindTexture(GL_TEXTURE_2D, textureId);
    //  Load data
    if (image.empty())
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, m_externalFormat, type, nullptr);
    }
    else
    {
        // Load image data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, m_externalFormat, type, image.data());
    }

    // Mipmaps
    if (levels > 1)
    {
        glGenerateTextureMipmap(textureId);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up previously created id
    if (m_textureId != 0)
    {
        glDeleteTextures(1, &m_textureId);
    }

    // Set new texture id
    m_textureId = textureId;
    m_width = width;
    m_height = height;
    m_valid = true;
    return true;
}

void Texture::setParameter(GLenum parameterName, GLint value)
{
    glTextureParameteri(m_textureId, parameterName, value);
}
