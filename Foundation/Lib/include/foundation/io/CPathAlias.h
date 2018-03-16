#pragma once

#include <string>

/**
 * A path alias associates a alias with a
 * file system path.
 */
class CPathAlias
{
   public:
    /**
     * Default constructor.
     */
    CPathAlias();

    /**
     * Construct with alias and path.
     */
    CPathAlias(const std::string &alias, const std::string &path);

    /**
     * \return Stored alias.
     */
    const std::string &getAlias() const;

    /**
     * \return Associated path.
     */
    const std::string &getPath() const;

    /**
     * \return False if alias or path is empty and true otherwise.
     */
    bool isValid() const;

   private:
    std::string m_alias;
    std::string m_path;
};
