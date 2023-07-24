#pragma once

#include <string>

namespace debug
{
    struct DebugMessage
    {
        float time = 3;
        float alpha = 255;
        std::string message;

        DebugMessage(std::string message)
        {
            this->message = message;
        }
    };

    void setDebug(bool debug);
    bool getDebug();
    void message(std::string message);
    void update();
    void draw();
}
