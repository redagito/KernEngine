#include <vector>

#include <lodepng.h>
#include <jpeglib.h>

#include "LoadImage.h"
#include "util/StringUtil.h"

#include <foundation/debug/Log.h>

bool load(const std::string &file, EColorFormat format, SImage &image)
{
  auto extension = getFileExtension(file);
  if (extension == "png")
  {
    return loadPng(file, format, image);
  }
  else if (extension == "jpg" || extension == "jpeg")
  {
    return loadJpeg(file, format, image);
  }
  LOG_ERROR("The image file %s has an invalid or unknown format.",
            file.c_str());
  return false;
}

bool loadPng(const std::string &file, EColorFormat format, SImage &image)
{
  // Map color type
  LodePNGColorType colorType;
  switch (format)
  {
  case EColorFormat::GreyScale8:
    colorType = LCT_GREY;
    break;
  case EColorFormat::RGB24:
    colorType = LCT_RGB;
    break;
  case EColorFormat::RGBA32:
    colorType = LCT_RGBA;
    break;
  default:
    LOG_ERROR("Unknown color format encountered while loading image file %s.",
              file.c_str());
    return false;
    break;
  }

  // Decode image data
  unsigned int err = lodepng::decode(image.m_data, image.m_width,
                                     image.m_height, file, colorType);
  if (err != 0)
  {
    LOG_ERROR("An error occured while decoding the image file %s: %s",
              file.c_str(), lodepng_error_text(err));
    return false;
  }
  image.m_format = format;
  return true;
}

bool loadJpeg(const std::string &file, EColorFormat format, SImage &image)
{
  return false;
}
