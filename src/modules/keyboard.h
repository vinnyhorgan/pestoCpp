#pragma once

#include <sol/sol.hpp>
#include <raylib.h>

namespace keyboard
{
    // Functions

    // Utility
    sol::table registerKeyboardAPI(sol::state& lua);

    // API
    bool isDown(KeyboardKey key);
    bool isPressed(KeyboardKey key);
    bool isReleased(KeyboardKey key);
}
