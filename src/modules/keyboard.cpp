#include "keyboard.h"

namespace keyboard
{
    // Functions

    // Utility
    sol::table registerKeyboardAPI(sol::state& lua)
    {
        lua.new_enum<KeyboardKey>("Key", {
            {"Null", KEY_NULL},
            {"Apostrophe", KEY_APOSTROPHE},
            {"Comma", KEY_COMMA},
            {"Minus", KEY_MINUS},
            {"Period", KEY_PERIOD},
            {"Slash", KEY_SLASH},
            {"Zero", KEY_ZERO},
            {"One", KEY_ONE},
            {"Two", KEY_TWO},
            {"Three", KEY_THREE},
            {"Four", KEY_FOUR},
            {"Five", KEY_FIVE},
            {"Six", KEY_SIX},
            {"Seven", KEY_SEVEN},
            {"Eight", KEY_EIGHT},
            {"Nine", KEY_NINE},
            {"Semicolon", KEY_SEMICOLON},
            {"Equal", KEY_EQUAL},
            {"A", KEY_A},
            {"B", KEY_B},
            {"C", KEY_C},
            {"D", KEY_D},
            {"E", KEY_E},
            {"F", KEY_F},
            {"G", KEY_G},
            {"H", KEY_H},
            {"I", KEY_I},
            {"J", KEY_J},
            {"K", KEY_K},
            {"L", KEY_L},
            {"M", KEY_M},
            {"N", KEY_N},
            {"O", KEY_O},
            {"P", KEY_P},
            {"Q", KEY_Q},
            {"R", KEY_R},
            {"S", KEY_S},
            {"T", KEY_T},
            {"U", KEY_U},
            {"V", KEY_V},
            {"W", KEY_W},
            {"X", KEY_X},
            {"Y", KEY_Y},
            {"Z", KEY_Z},
            {"LeftBracket", KEY_LEFT_BRACKET},
            {"Backslash", KEY_BACKSLASH},
            {"RightBracket", KEY_RIGHT_BRACKET},
            {"Grave", KEY_GRAVE},
            {"Space", KEY_SPACE},
            {"Escape", KEY_ESCAPE},
            {"Enter", KEY_ENTER},
            {"Tab", KEY_TAB},
            {"Backspace", KEY_BACKSPACE},
            {"Insert", KEY_INSERT},
            {"Delete", KEY_DELETE},
            {"Right", KEY_RIGHT},
            {"Left", KEY_LEFT},
            {"Down", KEY_DOWN},
            {"Up", KEY_UP},
            {"PageUp", KEY_PAGE_UP},
            {"PageDown", KEY_PAGE_DOWN},
            {"Home", KEY_HOME},
            {"End", KEY_END},
            {"CapsLock", KEY_CAPS_LOCK},
            {"ScrollLock", KEY_SCROLL_LOCK},
            {"NumLock", KEY_NUM_LOCK},
            {"PrintScreen", KEY_PRINT_SCREEN},
            {"Pause", KEY_PAUSE},
            {"F1", KEY_F1},
            {"F2", KEY_F2},
            {"F3", KEY_F3},
            {"F4", KEY_F4},
            {"F5", KEY_F5},
            {"F6", KEY_F6},
            {"F7", KEY_F7},
            {"F8", KEY_F8},
            {"F9", KEY_F9},
            {"F10", KEY_F10},
            {"F11", KEY_F11},
            {"F12", KEY_F12},
            {"LeftShift", KEY_LEFT_SHIFT},
            {"LeftControl", KEY_LEFT_CONTROL},
            {"LeftAlt", KEY_LEFT_ALT},
            {"LeftSuper", KEY_LEFT_SUPER},
            {"RightShift", KEY_RIGHT_SHIFT},
            {"RightControl", KEY_RIGHT_CONTROL},
            {"RightAlt", KEY_RIGHT_ALT},
            {"RightSuper", KEY_RIGHT_SUPER},
            {"KbMenu", KEY_KB_MENU},
            {"Kp0", KEY_KP_0},
            {"Kp1", KEY_KP_1},
            {"Kp2", KEY_KP_2},
            {"Kp3", KEY_KP_3},
            {"Kp4", KEY_KP_4},
            {"Kp5", KEY_KP_5},
            {"Kp6", KEY_KP_6},
            {"Kp7", KEY_KP_7},
            {"Kp8", KEY_KP_8},
            {"Kp9", KEY_KP_9},
            {"KpDecimal", KEY_KP_DECIMAL},
            {"KpDivide", KEY_KP_DIVIDE},
            {"KpMultiply", KEY_KP_MULTIPLY},
            {"KpSubtract", KEY_KP_SUBTRACT},
            {"KpAdd", KEY_KP_ADD},
            {"KpEnter", KEY_KP_ENTER},
            {"KpEqual", KEY_KP_EQUAL},
            {"Back", KEY_BACK},
            {"Menu", KEY_MENU},
            {"VolumeUp", KEY_VOLUME_UP},
            {"VolumeDown", KEY_VOLUME_DOWN}
        });

        sol::table keyboard = lua.create_table();

        keyboard["isDown"] = isDown;
        keyboard["isPressed"] = isPressed;
        keyboard["isReleased"] = isReleased;

        return keyboard;
    }

    // API
    bool isDown(KeyboardKey key)
    {
        return IsKeyDown(key);
    }

    bool isPressed(KeyboardKey key)
    {
        return IsKeyPressed(key);
    }

    bool isReleased(KeyboardKey key)
    {
        return IsKeyReleased(key);
    }
}
