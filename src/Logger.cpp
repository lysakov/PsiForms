#include "Logger.hpp"



/******************************
 * Logger methods realization *
 ******************************/



Logger::Logger()
{

    _outputPtr = new std::ofstream("log.txt");

}

Logger& Logger::getInstance()
{

    static Logger logger;

    return logger;

}

void Logger::setLogFile(const std::string &path)
{

    Logger &log = getInstance();

    if (log._outputPtr != nullptr && typeid(*(log._outputPtr)) != typeid(std::cout)) {
        delete log._outputPtr;
    }

    if (path == "stdout") {
        log._outputPtr = &std::cout;
    }
    else
    {
        log._outputPtr = new std::ofstream(path);
    }    

}

void Logger::setLevel(LogLevel level)
{

    Logger::getInstance()._level = level;

}

LogLevel Logger::getLevel()
{

    return Logger::getInstance()._level;

}

Logger::~Logger()
{

    if (_outputPtr != nullptr && typeid(*_outputPtr) != typeid(std::cout)) {
        delete _outputPtr;
    }

}