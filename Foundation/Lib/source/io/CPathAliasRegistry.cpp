#include "foundation/io/CPathAliasRegistry.h"

static bool resolveAlias(const std::string &alias, const std::string &path,
                         std::string &str)
{
  auto start = str.find(alias + ":");
  // Alias not found
  if (start == std::string::npos)
  {
    return false;
  }
  if (path.back() != '/')
  {
    str.replace(start, alias.size() + 1, path + "/");
  }
  else
  {
    str.replace(start, alias.size() + 1, path);
  }
  return true;
}

CPathAliasRegistry::CPathAliasRegistry(const CCoreSystem &coreSystem)
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);

  // System path aliases
  // Root directory
  setAlias("root", coreSystem.getRootDirectory());

  // Data directory
  setAlias("export", "root:export");
  setAlias("data", "export:data");

  // Content directory aliases
  // Mesh data location
  setAlias("mesh", "data:mesh");
  // Animation data location
  setAlias("anim", "data:animation");
  // Video data location
  setAlias("video", "data:video");
  // Database location
  setAlias("db", "data:database");
  // Sequence data location
  setAlias("seq", "data:sequence");
  // Audio data location
  setAlias("audio", "data:audio");
  // Audio streaming data location
  setAlias("stream", "data:audio");
  // Shaders
  setAlias("shader", "data:shader");
  // Textures
  setAlias("texture", "data:texture");

  setAlias("frame", "data:frame");
  // Models
  setAlias("model", "data:model");
  // Physics data location
  setAlias("physics", "data:physics");
  // Simple UI
  setAlias("sui", "data:sui");
  // Materials
  setAlias("material", "data:material");

  setAlias("surface", "data:surface");
  // Scripts
  setAlias("script", "data:script");
  // GUI data
  setAlias("gui", "data:gui");

  // Particle mesh may use original mesh but needs explicit load
  setAlias("particlemesh", "data:mesh");

  // Navigation meshes
  setAlias("navmesh", "data:navmesh");

  // System resources
  // System meshes
  setAlias("sysmesh", "data:mesh");
  // System textures
  setAlias("systexture", "data:texture");
  // System animations
  setAlias("sysanim", "data:animation");
}

CPathAliasRegistry::~CPathAliasRegistry() {}

void CPathAliasRegistry::setAlias(const std::string &alias,
                                  const std::string &path)
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);
  // Path is not an alias or and does not end with '/'
  if (path.back() != ':' && path.back() != '/')
  {
    m_pathAliases[alias] = path + "/";
  }
  else
  {
    m_pathAliases[alias] = path;
  }
}

void CPathAliasRegistry::setAlias(const CPathAlias &alias)
{
  setAlias(alias.getAlias(), alias.getPath());
}

bool CPathAliasRegistry::hasAlias(const std::string &alias) const
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);
  auto iter = m_pathAliases.find(alias);
  return iter != m_pathAliases.end();
}

std::string CPathAliasRegistry::getPath(const std::string &alias) const
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);
  return m_pathAliases.at(alias);
}

void CPathAliasRegistry::removeAlias(const std::string &alias)
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);
  m_pathAliases.erase(alias);
}

std::vector<CPathAlias> CPathAliasRegistry::getAllAliases() const
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);

  std::vector<CPathAlias> aliases;
  aliases.reserve(m_pathAliases.size());
  for (const auto &entry : m_pathAliases)
  {
    aliases.emplace_back(entry.first, entry.second);
  }
  return aliases;
}

CUniformResourceIdentifier
CPathAliasRegistry::resolveAliases(const CUniformResourceIdentifier &uri) const
{
  return uri;
}

std::string CPathAliasRegistry::resolveAliases(const std::string &uriStr)
{
  std::lock_guard<std::recursive_mutex> lg(m_pathAliasesMutex);

  // Prevent infinite recursion
  unsigned int maxReplaces = 1000;

  std::string result = uriStr;
  bool replace = true;
  unsigned int replaces = 0;
  while (replaces < maxReplaces && replace)
  {
    // Assume no replace happens this pass
    replace = false;

    // Try resolve for each alias
    for (const auto &alias : m_pathAliases)
    {
      // Attempt to resolve current alias
      if (resolveAlias(alias.first, alias.second, result))
      {
        // Alias was resolved, start again from start
        ++replaces;
        replace = true;
        break;
      }
    }
  }
  return result;
}
