#pragma once

#include <vector>
#include <unordered_map>

#include "foundation/io/EMediaType.h"

/**
 * Represents a Media (MIME) type.
 */
class CMediaType
{
public:
  CMediaType();
  CMediaType(const std::string &str);

  bool isValid() const;
  EMediaType getType() const;
  const std::vector<std::string> &getTree() const;
  const std::string &getSubType() const;
  const std::vector<std::string> &getSuffixes() const;
  const std::unordered_map<std::string, std::string> &getParameters() const;

private:
  bool parse(const std::string &str);

  bool m_valid = false;
  EMediaType m_type = EMediaType::Unknown;
  std::vector<std::string> m_tree;
  std::string m_subType;
  std::vector<std::string> m_suffixes;
  std::unordered_map<std::string, std::string> m_parameters;
};
