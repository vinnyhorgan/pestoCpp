#pragma once

#include <string>

namespace logger
{
    void setDebug(bool debug);
    void rayLog(int msgType, const char* text, va_list args);
    void info(std::string message);
    void error(std::string message);
    void warn(std::string message);
    void debug(std::string message);
}
