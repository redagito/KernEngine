#pragma once

#include <memory>
#include <list>
#include <mutex>
#include <atomic>

class IConsoleHandler;

/**
 * The console class acts as the central point for command
 * line based communication with the user.
 * The console forwards its input to any number of handler
 * objects which can perform work based on the input and
 * may provide text input back to the console.
 */
class CConsole
{
public:
  ~CConsole();

  void start();
  void stop();
  void update();

  void attachHandler(const std::shared_ptr<IConsoleHandler> &handler);
  void detachHandler(const std::shared_ptr<IConsoleHandler> &handler);

  /**
   * \return Currently attached handlers.
   */
  std::list<std::shared_ptr<IConsoleHandler>> getHandlers() const;

  /**
   * Print line of text.
   */
  void print(const std::string &text);

  /**
   * Print debug information.
   */
  void debug(const std::string &text);

  /**
   * Print warning message.
   */
  void warning(const std::string &text);

  /**
   * Print error message.
   */
  void error(const std::string &text);

  /**
   * Attempts to get input from the console.
   */
  bool tryGetInput(std::string &text);

private:
  std::atomic_bool m_running{false};

  std::list<std::shared_ptr<IConsoleHandler>> m_handlers;
  mutable std::mutex m_handlersMutex;
};
