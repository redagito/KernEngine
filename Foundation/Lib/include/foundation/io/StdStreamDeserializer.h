#pragma once

#include <cstdint>
#include <istream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

/**
 * \brief Boolean type.
 */
bool deserialize(std::istream &stream, bool &value);

/**
 * \brief Integer types.
 */
bool deserialize(std::istream &stream, int8_t &value);
bool deserialize(std::istream &stream, int16_t &value);
bool deserialize(std::istream &stream, int32_t &value);
bool deserialize(std::istream &stream, int64_t &value);
bool deserialize(std::istream &stream, uint8_t &value);
bool deserialize(std::istream &stream, uint16_t &value);
bool deserialize(std::istream &stream, uint32_t &value);
bool deserialize(std::istream &stream, uint64_t &value);

/**
 * \brief Floating types.
 */
bool deserialize(std::istream &stream, float &value);
bool deserialize(std::istream &stream, double &value);

/**
 * \brief GLM vector types.
 */
bool deserialize(std::istream &stream, glm::vec2 &value);
bool deserialize(std::istream &stream, glm::vec3 &value);
bool deserialize(std::istream &stream, glm::vec4 &value);

/**
 * \brief GLM matrix types.
 */
bool deserialize(std::istream &stream, glm::mat2 &value);
bool deserialize(std::istream &stream, glm::mat3 &value);
bool deserialize(std::istream &stream, glm::mat4 &value);

/**
 * \brief String type.
 */
bool deserialize(std::istream &stream, std::string &value);

/**
 * \brief Vector type.
 */
bool deserialize(std::istream &stream, std::vector<uint8_t> &value);
bool deserialize(std::istream &stream, std::vector<uint16_t> &value);
bool deserialize(std::istream &stream, std::vector<uint32_t> &value);
bool deserialize(std::istream &stream, std::vector<uint64_t> &value);

bool deserialize(std::istream &stream, std::vector<float> &value);

bool deserialize(std::istream &stream, std::vector<glm::vec3> &value);