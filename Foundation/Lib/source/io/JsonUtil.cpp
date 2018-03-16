#include <fstream>

#include "foundation/debug/Log.h"
#include "foundation/io/JsonDeserialize.h"
#include "foundation/io/JsonUtil.h"

bool load(const std::string &file, Json::Value &value)
{
    std::ifstream ifs(file);
    if (!ifs.is_open())
    {
        LOG_ERROR("failed to open json file %s.", file.c_str());
        return false;
    }

    Json::Reader reader;
    if (!reader.parse(ifs, value))
    {
        LOG_ERROR("Failed to parse json file %s: %s", file.c_str(),
                  reader.getFormattedErrorMessages().c_str());
        return false;
    }
    return true;
}

bool load(const Json::Value &node, const std::string &name, int &i)
{
    if (!deserialize(node[name], i))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const Json::Value &node, const std::string &name, unsigned int &u)
{
    if (!deserialize(node[name], u))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const Json::Value &node, const std::string &name, float &f)
{
    if (!deserialize(node[name], f))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const Json::Value &node, const std::string &name, bool &b)
{
    if (!deserialize(node[name], b))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const Json::Value &node, const std::string &name, glm::vec3 &vec)
{
    if (!deserialize(node[name], vec))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const Json::Value &node, const std::string &name, std::string &str)
{
    if (!deserialize(node[name], str))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}