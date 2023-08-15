#include "foundation/io/JsonDeserialize.h"

namespace glm
{
void from_json(const nlohmann::json &j, glm::vec2 &val)
{
    val.x = j[0];
    val.y = j[1];
}

void from_json(const nlohmann::json &j, glm::vec3 &val)
{
    val.x = j[0];
    val.y = j[1];
    val.z = j[2];
}

void from_json(const nlohmann::json &j, glm::vec4 &val)
{
    val.x = j[0];
    val.y = j[1];
    val.z = j[2];
    val.w = j[3];
}

void from_json(const nlohmann::json &j, glm::quat &val)
{
    val.x = j[0];
    val.y = j[1];
    val.z = j[2];
    val.w = j[3];
}
}