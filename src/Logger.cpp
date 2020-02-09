#include "Logger.hpp"

bool Logger::_logOn = false;

std::unique_ptr<std::ostream> Logger::_logFile = std::unique_ptr<std::ofstream>(nullptr);

Logger::Logger()
{}

/* TODO: Error with stdout. Improve it */
void Logger::init(const std::string &fileName)
{

    _logOn = true;

    if (fileName == "stdout") {
        _logFile = std::unique_ptr<std::ostream>(&std::cout);
    }
    else {
        _logFile = std::make_unique<std::ofstream>(fileName);
    }

}

void Logger::logOn() noexcept
{

    _logOn = true;

}

void Logger::logOff() noexcept
{

    _logOn = false;

}