#include "foundation/util/Configuration.h"

#include <cstring>

static bool parseArg(const char *arg, Configuration &config)
{
  std::string key;
  std::string value;
  int index = 0;
  int length = static_cast<int>(strlen(arg));

  // Parse key
  for (; index < length; ++index)
  {
    if (arg[index] == '=')
    {
      break;
    }
    key.push_back(arg[index]);
  }
  ++index; // Skip '='
  // Parse value
  bool inString = false;
  bool escaped = false;
  for (; index < length; ++index)
  {
    if (arg[index] == '"' && !escaped && !inString)
    {
      inString = true;
      continue;
    }
    else if (arg[index] == '"' && !escaped && inString)
    {
      inString = false;
      continue;
    }
    else if (arg[index] == '\\' && !escaped)
    {
      escaped = true;
      continue;
    }
    value.push_back(arg[index]);
    escaped = false;
  }

  if (key.empty())
  {
    return false;
  }
  if (value.empty())
  {
    return false;
  }
  if (inString)
  {
    return false;
  }
  if (escaped)
  {
    return false;
  }

  // Add entry
  config[key] = CValue{value};
  return true;
}

bool parse(int argc, const char **argv, Configuration &config)
{
  for (int i = 0; i < argc; ++i)
  {
    if (!parseArg(argv[i], config))
    {
      return false;
    }
  }
  return true;
}
