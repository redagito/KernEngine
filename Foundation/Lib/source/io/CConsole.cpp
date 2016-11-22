#include "foundation/io/CConsole.h"

#include <cassert>
#include <algorithm>

#include "foundation/io/IConsoleHandler.h"

CConsole::~CConsole() { stop(); }

void CConsole::start() { m_running = true; }

void CConsole::stop() { m_running = false; }

void CConsole::update()
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  if (!m_running)
  {
    return;
  }
  for (const auto &handler : m_handlers)
  {
    handler->update();
  }
}

void CConsole::attachHandler(const std::shared_ptr<IConsoleHandler> &handler)
{
  assert(handler != nullptr);
  if (handler == nullptr)
  {
    return;
  }

  std::lock_guard<std::mutex> lg{m_handlersMutex};
  // Check for duplicate handler to enforce unique constraint
  if (std::find(m_handlers.begin(), m_handlers.end(), handler) !=
      m_handlers.end())
  {
    return;
  }
  // Add handler first
  m_handlers.push_back(handler);
  // Attach callback AFTER adding
  handler->onAttach(*this);
}

void CConsole::detachHandler(const std::shared_ptr<IConsoleHandler> &handler)
{
  assert(handler != nullptr);
  if (handler == nullptr)
  {
    return;
  }

  std::lock_guard<std::mutex> lg{m_handlersMutex};
  // Check if handler exists
  auto iter = std::find(m_handlers.begin(), m_handlers.end(), handler);
  if (iter == m_handlers.end())
  {
    // Unknown handler
    return;
  }
  // Handler exists, remove from list
  m_handlers.erase(iter);
  // Detach callback AFTER erasing
  handler->onDetach(*this);
}

std::list<std::shared_ptr<IConsoleHandler>> CConsole::getHandlers() const
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  return m_handlers;
}

void CConsole::print(const std::string &text)
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  if (!m_running)
  {
    return;
  }
  for (const auto &handler : m_handlers)
  {
    handler->print(text);
  }
}

void CConsole::debug(const std::string &text)
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  if (!m_running)
  {
    return;
  }
  for (const auto &handler : m_handlers)
  {
    handler->debug(text);
  }
}

void CConsole::warning(const std::string &text)
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  if (!m_running)
  {
    return;
  }
  for (const auto &handler : m_handlers)
  {
    handler->warning(text);
  }
}

void CConsole::error(const std::string &text)
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  if (!m_running)
  {
    return;
  }
  for (const auto &handler : m_handlers)
  {
    handler->error(text);
  }
}

bool CConsole::tryGetInput(std::string &text)
{
  std::lock_guard<std::mutex> lg{m_handlersMutex};
  if (!m_running)
  {
    return false;
  }
  for (auto &handler : m_handlers)
  {
    if (handler->tryGetInput(text))
    {
      return true;
    }
  }
  return false;
}
