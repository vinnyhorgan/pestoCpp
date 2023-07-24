#include "logger.h"

#include <spdlog/spdlog.h>

namespace logger
{
    void setDebug(bool debug)
    {
        spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::info);
    }

    void rayLog(int msgType, const char* text, va_list args)
    {
        char message[1024];

        vsprintf(message, text, args);

        switch (msgType)
        {
        case 3:
            spdlog::info(message);
            break;
        case 5:
            spdlog::error(message);
            break;
        case 4:
            spdlog::warn(message);
            break;
        case 2:
            spdlog::debug(message);
            break;
        }
    }

    void info(std::string message)
    {
        spdlog::info(message);
    }

    void error(std::string message)
    {
        spdlog::error(message);
    }

    void warn(std::string message)
    {
        spdlog::warn(message);
    }

    void debug(std::string message)
    {
        spdlog::debug(message);
    }
}
