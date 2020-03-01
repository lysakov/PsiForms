#ifndef LOGGER
#define LOGGER

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <typeinfo>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_OFF
};

class Logger
{

  public:
    /** 
     * @brief method initializes Logger object. By default log is printed in to "log.txt".
     * By default logging level is LOG_DEBUG.
     * @returns reference to Logger object 
     **/
    static Logger& getInstance();

    /** 
     * @brief method sets log file. By default log is printed in to "log.txt".
     * @param path - path to the output log file. 
     * If this parametr set to "stdout", log will be printed on standart output. 
     **/
    static void setLogFile(const std::string &path);

    /**
     * @brief sets logger level
     * @param level - logger level
     **/
    static void setLevel(LogLevel level);

    /**
     * @brief get cerrent logger level
     * @returns - logger level
     **/
    static LogLevel getLevel();

    /**
     * @brief operator << for Logger. Message is printed into the log file 
     * only if logging is turned on.
     * @param obj - reference to object to be printed in to log file
     * @returns reference to Logger 
     **/
    template<typename T>
    Logger& operator<<(const T &obj)
    {

        std::lock_guard<std::mutex> lock(_mtx);

        if (_level < LOG_OFF) {
            (*_outputPtr) << obj;
        }

        return *this;

    }

    /** 
     * @brief writes a line into log file
     * @param obj - object of class T whith overloaded operator << 
     * @param level - logging level of message
     **/
    template<typename T>
    static void writeLine(const T &obj, LogLevel level = LOG_INFO)
    {

        Logger &log = getInstance();

        if (log._level <= level) {
            log << obj << '\n';
        }

    }

    /**
     * @brief writes an empty line in to log file 
     **/
    static void writeLine(LogLevel level = LOG_INFO)
    {

        Logger &log = getInstance();

        if (log._level <= level) {
            log << '\n';
        }

    }

    /**
     * @brief Writes debug info into logger
     * @param obj - object whith overloaded operator <<
     **/
    template<typename T>
    static void debug(const T &obj)
    {

        writeLine(obj, LOG_DEBUG);

    }

    /**
     * @brief Writes informational message into logger
     * @param obj - object whith overloaded operator <<
     **/
    template<typename T>
    static void info(const T &obj)
    {

        writeLine(obj, LOG_INFO);

    }

    /**
     * @brief Writes warning about potentionaly harmfull situations
     * @param obj - object whith overloaded operator <<
     **/
    template<typename T>
    static void warn(const T &obj)
    {

        writeLine(obj, LOG_WARN);

    }

    /**
     * @brief Writes error message
     * @param obj - object whith overloaded operator <<
     **/
    template<typename T>
    static void error(const T &obj)
    {

        writeLine(obj, LOG_ERROR);

    }

    /**
     * @brief Writes information about situations which lead the application to be abort
     * @param obj - object whith overloaded operator <<
     **/
    template<typename T>
    static void fatal(const T &obj)
    {

        writeLine(obj, LOG_FATAL);

    }

    ~Logger();

    Logger(const Logger &logger) = delete;

    Logger& operator=(const Logger &logger) = delete;

  private:
    /* Logger level */
    LogLevel _level = LOG_DEBUG;

    /* Log output file */
    std::ostream *_outputPtr = nullptr;

    /* thread-safety */
    std::mutex _mtx;

    Logger();

};

#endif