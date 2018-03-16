#pragma once

#include <string>

class CConsole;
/**
 * Console handler interface.
 * Console handlers are attached to a console object and
 * are notified by the console on console input.
 * A console handler can also provide input to the console.
 */
class IConsoleHandler
{
   public:
    virtual ~IConsoleHandler();

    /**
     * Called on attached to console.
     */
    virtual void onAttach(const CConsole &console) = 0;

    /**
     * Called on detach from console.
     */
    virtual void onDetach(const CConsole &console) = 0;

    /**
     * Update call from console.
     */
    virtual void update() = 0;

    /**
     * Called on text output.
     */
    virtual void print(const std::string &text) = 0;

    /**
     * Called on debug output.
     */
    virtual void debug(const std::string &text) = 0;

    /**
     * Called on warning output.
     */
    virtual void warning(const std::string &text) = 0;

    /**
     * Called on error output.
     */
    virtual void error(const std::string &text) = 0;

    /**
     * Reads the input from the handler into the console.
     */
    virtual bool tryGetInput(std::string &text) = 0;
};
