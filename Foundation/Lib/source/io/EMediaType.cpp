#include "foundation/io/EMediaType.h"

EMediaType fromString(const std::string &str)
{
    if (str == "application")
    {
        return EMediaType::Application;
    }
    else if (str == "audio")
    {
        return EMediaType::Audio;
    }
    else if (str == "example")
    {
        return EMediaType::Example;
    }
    else if (str == "image")
    {
        return EMediaType::Image;
    }
    else if (str == "message")
    {
        return EMediaType::Message;
    }
    else if (str == "model")
    {
        return EMediaType::Model;
    }
    else if (str == "multipart")
    {
        return EMediaType::Multipart;
    }
    else if (str == "text")
    {
        return EMediaType::Text;
    }
    else if (str == "video")
    {
        return EMediaType::Video;
    }
    return EMediaType::Unknown;
}

std::string toString(EMediaType type)
{
    switch (type)
    {
    case EMediaType::Application:
        return "application";

    case EMediaType::Audio:
        return "audio";

    case EMediaType::Example:
        return "example";

    case EMediaType::Image:
        return "image";

    case EMediaType::Message:
        return "message";

    case EMediaType::Model:
        return "model";

    case EMediaType::Multipart:
        return "multipart";

    case EMediaType::Text:
        return "text";

    case EMediaType::Video:
        return "video";

    case EMediaType::Unknown:
    default:
        return "unknown";
    }
}
