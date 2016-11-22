#pragma once

#include <string>

/**
 * Game content system class.
 * Initializes access to the game content.
 */
class CGameContentSystem
{
public:
  CGameContentSystem();
  ~CGameContentSystem();

  /**
   * Human readable game title.
   */
  void setTitle(const std::string &title);
  const std::string &getTitle() const;

  /**
   * Game title id.
   */
  void setTitleId(const std::string &id);
  const std::string &getTitleId() const;

  /**
   * Game title version.
   */
  void setVersion(const std::string &version);
  const std::string &getVersion() const;

  void start();
  void stop();

private:
  std::string m_title;
  std::string m_titleId;
  std::string m_version;
  bool m_running = false;
};
