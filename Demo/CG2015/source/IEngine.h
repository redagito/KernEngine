#pragma once

/**
 * \brief Game engine interface.
 */
class IEngine
{
   public:
    /**
     * \brief Virtual destructor for interface class.
     */
    virtual ~IEngine();

    /**
     * \brief Initializes engine and subsystems.
     * \parm config Config file name containing initialization parameters.
     * \return True on success and false on error.
     */
    virtual bool init(const char *configFile) = 0;

    /**
     * \brief Starts game loop.
     *
     * The function returns after the game loop ends.
     */
    virtual void run() = 0;
};
