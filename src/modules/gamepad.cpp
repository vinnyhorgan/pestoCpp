#include "gamepad.h"

#include "../pesto.h"
#include "../error.h"

#define MAX_GAMEPADS 4

int gamepadCount = 0;

namespace gamepad
{
    // Types
    std::string Gamepad::type()
    {
        return "Gamepad";
    }

    int Gamepad::getId()
    {
        return id;
    }

    std::string Gamepad::getName()
    {
        return GetGamepadName(id);
    }

    bool Gamepad::isDown(GamepadButton button)
    {
        return IsGamepadButtonDown(id, button);
    }

    bool Gamepad::isPressed(GamepadButton button)
    {
        return IsGamepadButtonPressed(id, button);
    }

    bool Gamepad::isReleased(GamepadButton button)
    {
        return IsGamepadButtonReleased(id, button);
    }

    int Gamepad::getAxisCount()
    {
        return GetGamepadAxisCount(id);
    }

    float Gamepad::getAxisMovement(GamepadAxis axis)
    {
        return GetGamepadAxisMovement(id, axis);
    }

    // Functions

    // Utility
    sol::table registerGamepadAPI(sol::state& lua)
    {
        lua.new_enum<GamepadButton>("GamepadButton", {
            {"Unknown", GAMEPAD_BUTTON_UNKNOWN},
            {"LeftFaceUp", GAMEPAD_BUTTON_LEFT_FACE_UP},
            {"LeftFaceRight", GAMEPAD_BUTTON_LEFT_FACE_RIGHT},
            {"LeftFaceDown", GAMEPAD_BUTTON_LEFT_FACE_DOWN},
            {"LeftFaceLeft", GAMEPAD_BUTTON_LEFT_FACE_LEFT},
            {"RightFaceUp", GAMEPAD_BUTTON_RIGHT_FACE_UP},
            {"RightFaceRight", GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},
            {"RightFaceDown", GAMEPAD_BUTTON_RIGHT_FACE_DOWN},
            {"RightFaceLeft", GAMEPAD_BUTTON_RIGHT_FACE_LEFT},
            {"LeftTrigger1", GAMEPAD_BUTTON_LEFT_TRIGGER_1},
            {"LeftTrigger2", GAMEPAD_BUTTON_LEFT_TRIGGER_2},
            {"RightTrigger1", GAMEPAD_BUTTON_RIGHT_TRIGGER_1},
            {"RightTrigger2", GAMEPAD_BUTTON_RIGHT_TRIGGER_2},
            {"MiddleLeft", GAMEPAD_BUTTON_MIDDLE_LEFT},
            {"Middle", GAMEPAD_BUTTON_MIDDLE},
            {"MiddleRight", GAMEPAD_BUTTON_MIDDLE_RIGHT},
            {"LeftThumb", GAMEPAD_BUTTON_LEFT_THUMB},
            {"RightThumb", GAMEPAD_BUTTON_RIGHT_THUMB}
        });

        lua.new_enum<GamepadAxis>("GamepadAxis", {
            {"LeftX", GAMEPAD_AXIS_LEFT_X},
            {"LeftY", GAMEPAD_AXIS_LEFT_Y},
            {"RightX", GAMEPAD_AXIS_RIGHT_X},
            {"RightY", GAMEPAD_AXIS_RIGHT_Y},
            {"LeftTrigger", GAMEPAD_AXIS_LEFT_TRIGGER},
            {"RightTrigger", GAMEPAD_AXIS_RIGHT_TRIGGER}
        });

        sol::usertype<Gamepad> gamepadType = lua.new_usertype<Gamepad>("Gamepad");

        gamepadType["type"] = &Gamepad::type;
        gamepadType["getId"] = &Gamepad::getId;
        gamepadType["getName"] = &Gamepad::getName;
        gamepadType["isDown"] = &Gamepad::isDown;
        gamepadType["isPressed"] = &Gamepad::isPressed;
        gamepadType["isReleased"] = &Gamepad::isReleased;
        gamepadType["getAxisCount"] = &Gamepad::getAxisCount;
        gamepadType["getAxisMovement"] = &Gamepad::getAxisMovement;

        sol::table gamepad = lua.create_table();

        gamepad["getGamepads"] = getGamepads;
        gamepad["getGamepadCount"] = getGamepadCount;

        return gamepad;
    }

    void update()
    {
        sol::state& lua = pesto::getLua();

        if (gamepadCount != getGamepadCount())
        {
            sol::protected_function luaGamepadadded = lua["gamepadadded"];

            if (luaGamepadadded.valid())
            {
                Gamepad gamepad;
                gamepad.id = getGamepadCount() - 1; // check

                auto result = luaGamepadadded(gamepad);

                if (!result.valid())
                {
                    sol::error err = result;
                    error::throwError(err.what());
                }
            }

            gamepadCount = getGamepadCount();
        }
    }

    void resetState()
    {
        gamepadCount = 0;
    }

    // API
    sol::table getGamepads()
    {
        sol::table gamepads = pesto::getLua().create_table();

        for (int i = 0; i < MAX_GAMEPADS; i++)
        {
            if (IsGamepadAvailable(i))
            {
                Gamepad gamepad;
                gamepad.id = i;

                gamepads.add(gamepad);
            }
        }

        return gamepads;
    }

    int getGamepadCount()
    {
        int count = 0;

        for (int i = 0; i < MAX_GAMEPADS; i++)
        {
            if (IsGamepadAvailable(i))
            {
                count++;
            }
        }

        return count;
    }
}
