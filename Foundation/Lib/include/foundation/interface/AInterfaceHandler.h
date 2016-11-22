#pragma once

#include <string>

#include "foundation/message/IMessageHandler.h"

class AInterfaceHandler : public IMessageHandler
{
public:
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

private:
  std::string m_companyName;
  std::string m_applicationName;
};