#pragma once

#include <string>

namespace error
{
    bool state();
    void reset();
    void throwError(std::string message);
    void draw();
}
