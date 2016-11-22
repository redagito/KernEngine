#pragma once

#include <string>

#include "foundation/util/Configuration.h"

/**
 * Application interface provides an application model.
 */
class IApplication
{
public:
  virtual ~IApplication();

  /**
   * Company name.
   */
  virtual void setCompanyName(const std::string &name) = 0;
  virtual const std::string &getCompanyName() const = 0;

  /**
   * Application name.
   */
  virtual void setApplicationName(const std::string &name) = 0;
  virtual const std::string &getApplicationName() const = 0;

  /**
   * Application ID.
   */
  virtual void setApplicationId(const std::string &name) = 0;
  virtual const std::string &getApplicationId() const = 0;

  /**
   * Application version.
   */
  virtual void setApplicationVersion(const std::string &name) = 0;
  virtual const std::string &getApplicationVersion() const = 0;

  /**
   * Set command line arguments.
   */
  virtual void setCommandLineArguments(const Configuration &arguments) = 0;
  virtual const Configuration &getCommandLineArguments() const = 0;

  /**
   * Application initialization and deinitialization.
   */
  virtual bool isInitialized() const = 0;
  virtual bool initialize() = 0;
  virtual void deinitialize() = 0;

  /**
   * Runs the application and starts main loop.
   * \return Return code.
   */
  virtual int run() = 0;
};
