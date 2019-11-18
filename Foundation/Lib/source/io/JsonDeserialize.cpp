#include "foundation/io/JsonDeserialize.h"

bool load(const std::string &file, nlohmann::json &value)
{
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        LOG_ERROR("failed to open json file %s.", file.c_str());
        return false;
    }

    try
    {
        value = nlohmann::json::parse(ifs);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        LOG_ERROR("Failed to parse json file %s: %s", file.c_str(), e.what());
        return false;
    }
    return true;
}