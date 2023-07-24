#pragma once

#include <sol/sol.hpp>
#include <raylib.h>

namespace mouse
{
    // Functions

    // Utility
    sol::table registerMouseAPI(sol::state& lua);
    void update(float scale);

    // API
    std::tuple<int, int> getPosition();
    std::tuple<float, float> getDelta();
    float getWheelMove();
    int getX();
    int getY();
    bool isDown(MouseButton button);
    bool isPressed(MouseButton button);
    bool isReleased(MouseButton button);
    bool isGrabbed();
    bool isVisible();
    void setCursor(MouseCursor cursor);
    void setGrabbed(bool grabbed);
    void setPosition(int x, int y);
    void setVisible(bool visible);
    void setX(int x);
    void setY(int y);
}
