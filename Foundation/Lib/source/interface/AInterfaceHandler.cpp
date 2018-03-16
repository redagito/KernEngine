#include "foundation/interface/AInterfaceHandler.h"

void AInterfaceHandler::setCompanyName(const std::string &name) { m_companyName = name; }

const std::string &AInterfaceHandler::getCompanyName() const { return m_companyName; }

void AInterfaceHandler::setApplicationName(const std::string &name) { m_applicationName = name; }

const std::string &AInterfaceHandler::getApplicationName() const { return m_applicationName; }