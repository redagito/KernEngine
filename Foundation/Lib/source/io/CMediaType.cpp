#include "foundation/io/CMediaType.h"

CMediaType::CMediaType() {}

CMediaType::CMediaType(const std::string &str) { m_valid = parse(str); }

bool CMediaType::isValid() const { return m_valid; }

EMediaType CMediaType::getType() const { return m_type; }

const std::vector<std::string> &CMediaType::getTree() const { return m_tree; }

const std::string &CMediaType::getSubType() const { return m_subType; }

const std::vector<std::string> &CMediaType::getSuffixes() const
{
  return m_suffixes;
}

const std::unordered_map<std::string, std::string> &
CMediaType::getParameters() const
{
  return m_parameters;
}

bool CMediaType::parse(const std::string &str) { return false; }
