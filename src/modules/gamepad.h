#pragma once

#include <sol/sol.hpp>
#include <raylib.h>

namespace gamepad
{
    // Types
    class Gamepad
    {
    public:
        int id;

        std::string type();

        int getId();
        std::string getName();
        bool isDown(GamepadButton button);
        bool isPressed(GamepadButton button);
        bool isReleased(GamepadButton button);
        int getAxisCount();
        float getAxisMovement(GamepadAxis axis);
    };

    // Functions

    // Utility
    sol::table registerGamepadAPI(sol::state& lua);
    void update();
    void resetState();

    // API
    sol::table getGamepads();
    int getGamepadCount();
}
