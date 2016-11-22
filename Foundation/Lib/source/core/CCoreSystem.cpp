#include "foundation/core/CCoreSystem.h"

CCoreSystem::~CCoreSystem() { stop(); }

void CCoreSystem::setCompanyName(const std::string &name)
{
  m_companyName = name;
}

const std::string &CCoreSystem::getCompanyName() const { return m_companyName; }

void CCoreSystem::setApplicationName(const std::string &name)
{
  m_applicationName = name;
}

const std::string &CCoreSystem::getApplicationName() const
{
  return m_applicationName;
}

void CCoreSystem::setRootDirectory(const std::string &root)
{
  m_rootDirectory = root;
}

const std::string &CCoreSystem::getRootDirectory() const
{
  return m_rootDirectory;
}

void CCoreSystem::start()
{
  if (!m_started)
  {
    m_started = true;
    m_console.start();
  }
}

void CCoreSystem::stop()
{
  if (m_started)
  {
    m_started = false;
    m_console.stop();
  }
}

void CCoreSystem::update()
{
  if (m_started)
  {
    m_console.update();
  }
}
