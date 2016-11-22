#pragma once

#include "foundation/io/CUniformResourceIdentifier.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "foundation/io/CPathAlias.h"
#include "foundation/core/CCoreSystem.h"

/**
 * Central registry for path aliases.
 * The registry is thread safe and visible from all threads
 */
class CPathAliasRegistry
{
public:
  CPathAliasRegistry(const CCoreSystem &coreSystem);
  ~CPathAliasRegistry();

  void setAlias(const std::string &alias, const std::string &path);
  void setAlias(const CPathAlias &alias);
  bool hasAlias(const std::string &alias) const;
  std::string getPath(const std::string &alias) const;
  void removeAlias(const std::string &alias);

  std::vector<CPathAlias> getAllAliases() const;

  CUniformResourceIdentifier
  resolveAliases(const CUniformResourceIdentifier &uri) const;
  std::string resolveAliases(const std::string &uriStr);

private:
  std::unordered_map<std::string, std::string> m_pathAliases;
  mutable std::recursive_mutex m_pathAliasesMutex;
};
