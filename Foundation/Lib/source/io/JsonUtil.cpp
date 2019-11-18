#include <fstream>

#include "foundation/debug/Log.h"
#include "foundation/io/JsonDeserialize.h"
#include "foundation/io/JsonUtil.h"

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
	catch (const nlohmann::json::parse_error& e)
	{
        LOG_ERROR("Failed to parse json file %s: %s", file.c_str(),
                  e.what());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, int &i)
{
    if (node.find(name) == node.end())
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node.at(name), i))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, unsigned int &u)
{
    if (node.find(name) == node.end())
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node.at(name), u))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, float &f)
{
    if (node.find(name) == node.end())
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node.at(name), f))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, bool &b)
{
    if (node.find(name) == node.end())
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node.at(name), b))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, glm::vec3 &vec)
{
    if (node.find(name) == node.end())
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node.at(name), vec))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}

bool load(const nlohmann::json &node, const std::string &name, std::string &str)
{
    if (node.find(name) == node.end())
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }

    if (!deserialize(node.at(name), str))
    {
        LOG_ERROR("Failed to load '%s' parameter.", name.c_str());
        return false;
    }
    return true;
}