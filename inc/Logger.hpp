#ifndef LOGGER
#define LOGGER

#include <string>
#include <fstream>
#include <iostream>
#include <memory>

class Logger
{

  public:
    /* @brief method initializes Logger object. By default log is printed in to "log.txt".
     * @param fileName - name of the output log file. 
     * If this parametr set to "stdout", log will be printed on standart output. */
    static void init(const std::string &fileName = "log.txt");

    /* @brief turn on the logging. Logging is turned on by default. */
    static void logOn() noexcept;

    /* @brief turn off the logging. Logging is turned on by default. */
    static void logOff() noexcept;

    /* @brief writes a line into log file
     * @param obj - object of class T whith overloaded operator << */
    template<typename T>
    static void writeLine(const T &obj)
    {

        if (_logOn) {
            (*_logFile) << obj << std::endl;
        }

    }

    Logger(const Logger &logger) = delete;

    Logger& operator=(const Logger &logger) = delete;

  private:
    /* True if logging turned on */
    static bool _logOn;

    /* Log output file */
    static std::unique_ptr<std::ostream> _logFile;

    Logger();

};

#endif