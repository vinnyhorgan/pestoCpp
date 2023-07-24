#include "mouse.h"

#include "../pesto.h"

#include <raymath.h>

int transformedX = 0;
int transformedY = 0;
bool grab = false;

namespace mouse
{
    // Functions

    // Utility
    sol::table registerMouseAPI(sol::state& lua)
    {
        lua.new_enum<MouseButton>("Button", {
            {"Left", MOUSE_BUTTON_LEFT},
            {"Right", MOUSE_BUTTON_RIGHT},
            {"Middle", MOUSE_BUTTON_MIDDLE},
            {"Side", MOUSE_BUTTON_SIDE},
            {"Extra", MOUSE_BUTTON_EXTRA},
            {"Forward", MOUSE_BUTTON_FORWARD},
            {"Back", MOUSE_BUTTON_BACK}
        });

        lua.new_enum<MouseCursor>("Cursor", {
            {"Default", MOUSE_CURSOR_DEFAULT},
            {"Arrow", MOUSE_CURSOR_ARROW},
            {"Ibeam", MOUSE_CURSOR_IBEAM},
            {"Crosshair", MOUSE_CURSOR_CROSSHAIR},
            {"PointingHand", MOUSE_CURSOR_POINTING_HAND},
            {"ResizeEW", MOUSE_CURSOR_RESIZE_EW},
            {"ResizeNS", MOUSE_CURSOR_RESIZE_NS},
            {"ResizeNWSE", MOUSE_CURSOR_RESIZE_NWSE},
            {"ResizeNESW", MOUSE_CURSOR_RESIZE_NESW},
            {"ResizeAll", MOUSE_CURSOR_RESIZE_ALL},
            {"NotAllowed", MOUSE_CURSOR_NOT_ALLOWED}
        });

        sol::table mouse = lua.create_table();

        mouse["getPosition"] = getPosition;
        mouse["getDelta"] = getDelta;
        mouse["getWheelMove"] = getWheelMove;
        mouse["getX"] = getX;
        mouse["getY"] = getY;
        mouse["isDown"] = isDown;
        mouse["isPressed"] = isPressed;
        mouse["isReleased"] = isReleased;
        mouse["isGrabbed"] = isGrabbed;
        mouse["isVisible"] = isVisible;
        mouse["setCursor"] = setCursor;
        mouse["setGrabbed"] = setGrabbed;
        mouse["setPosition"] = setPosition;
        mouse["setVisible"] = setVisible;
        mouse["setX"] = setX;
        mouse["setY"] = setY;

        return mouse;
    }

    void update(float scale)
    {
        int gameWidth = pesto::getGameWidth();
        int gameHeight = pesto::getGameHeight();

        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameHeight * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, { 0, 0 }, { (float)gameWidth, (float)gameHeight });

        transformedX = (int)virtualMouse.x;
        transformedY = (int)virtualMouse.y;
    }

    // API
    std::tuple<int, int> getPosition()
    {
        if (transformedX != 0 && transformedY != 0)
        {
            return std::make_tuple(transformedX, transformedY);
        }
        else
        {
            return std::make_tuple(GetMouseX(), GetMouseY());
        }
    }

    std::tuple<float, float> getDelta()
    {
        Vector2 delta = GetMouseDelta();
        return std::make_tuple(delta.x, delta.y);
    }

    float getWheelMove()
    {
        return GetMouseWheelMove();
    }

    int getX()
    {
        if (transformedX != 0)
        {
            return transformedX;
        }
        else
        {
            return GetMouseX();
        }
    }

    int getY()
    {
        if (transformedY != 0)
        {
            return transformedY;
        }
        else
        {
            return GetMouseY();
        }
    }

    bool isDown(MouseButton button)
    {
        return IsMouseButtonDown(button);
    }

    bool isPressed(MouseButton button)
    {
        return IsMouseButtonPressed(button);
    }

    bool isReleased(MouseButton button)
    {
        return IsMouseButtonReleased(button);
    }

    bool isGrabbed()
    {
        return grab;
    }

    bool isVisible()
    {
        return !IsCursorHidden();
    }

    void setCursor(MouseCursor cursor)
    {
        SetMouseCursor(cursor);
    }

    void setGrabbed(bool grabbed)
    {
        grab = grabbed;

        if (grab)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
        }
    }

    void setPosition(int x, int y)
    {
        SetMousePosition(x, y);
    }

    void setVisible(bool visible)
    {
        if (visible)
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }

    void setX(int x)
    {
        if (transformedY != 0)
            SetMousePosition(x, transformedY);
        else
            SetMousePosition(x, GetMouseY());
    }

    void setY(int y)
    {
        if (transformedX != 0)
            SetMousePosition(transformedX, y);
        else
            SetMousePosition(GetMouseX(), y);
    }
}
