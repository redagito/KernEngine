#pragma once

#include <string>

#include "foundation/io/CConsole.h"

/**
 * Initializes core runtime environment to start up
 * the rest of the application.
 * The core system object should be the first object
 * that is created by the application and the last one
 * to be destroyed.
 */
class CCoreSystem
{
public:
  ~CCoreSystem();

  /**
   * Company name.
   */
  void setCompanyName(const std::string &name);
  const std::string &getCompanyName() const;

  /**
   * Application name.
   */
  void setApplicationName(const std::string &name);
  const std::string &getApplicationName() const;

  /**
   * Application root directory.
   * Default value "home:"
   */
  void setRootDirectory(const std::string &root);
  const std::string &getRootDirectory() const;

  /**
   * Start core system.
   */
  void start();

  /**
   * Stop core system.
   */
  void stop();

  /**
   * Update core system while started.
   */
  void update();

private:
  bool m_started = false;
  std::string m_companyName = "MyCompany";
  std::string m_applicationName = "MyApplication";
  std::string m_rootDirectory = "home:";
  CConsole m_console;
};
