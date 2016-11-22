#pragma once

#include <string>

/**
 * Top level media types
 */
enum class EMediaType
{
  Application,
  Audio,
  Example,
  Image,
  Message,
  Model,
  Multipart,
  Text,
  Video,

  Unknown
};

EMediaType fromString(const std::string &str);
std::string toString(EMediaType type);
