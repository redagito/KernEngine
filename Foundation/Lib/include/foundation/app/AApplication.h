#pragma once

#include "foundation/app/IApplication.h"

class AApplication : public IApplication
{
public:
  virtual ~AApplication();

  void setCompanyName(const std::string &name) override;
  const std::string &getCompanyName() const override;

  void setApplicationName(const std::string &name) override;
  const std::string &getApplicationName() const override;

  void setApplicationId(const std::string &id) override;
  const std::string &getApplicationId() const override;

  void setApplicationVersion(const std::string &version) override;
  const std::string &getApplicationVersion() const override;

  void setCommandLineArguments(const Configuration &arguments) override;
  const Configuration &getCommandLineArguments() const override;

  bool isInitialized() const override;

  /**
   * Initializes logging mechanism
   */
  virtual bool initialize() override;
  virtual void deinitialize() override;

private:
  // Company name
  std::string m_companyName;
  std::string m_applicationName;
  std::string m_applicationId;
  std::string m_applicationVersion;
  std::string m_initializationTimestamp;
  // Application configuration
  Configuration m_configuration;
  bool m_initialized = false;
};
