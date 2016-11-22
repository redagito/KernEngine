#include "foundation/io/CPathAlias.h"

CPathAlias::CPathAlias() {}

CPathAlias::CPathAlias(const std::string &alias, const std::string &path)
    : m_alias(alias), m_path(path)
{
}

const std::string &CPathAlias::getAlias() const { return m_alias; }

const std::string &CPathAlias::getPath() const { return m_path; }

bool CPathAlias::isValid() const { return !m_alias.empty() && !m_path.empty(); }
