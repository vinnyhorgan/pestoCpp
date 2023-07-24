#pragma once

#include <sol/sol.hpp>

#include "graphics.h"

namespace window
{
    // Functions

    // Utility
    sol::table registerWindowAPI(sol::state& lua);
    void update();
    void resetState();

    // API
    void enableLetterbox(int width, int height);
    void disableLetterbox();
    std::tuple<float, float> getDPIScale();
    std::tuple<int, int> getDesktopDimensions(int display);
    std::tuple<int, int> getDimensions();
    int getDisplayCount();
    std::string getDisplayName(int display);
    bool getFullscreen();
    int getHeight();
    graphics::pImage getIcon();
    bool getLetterbox();
    sol::table getMode();
    std::tuple<int, int> getPosition();
    std::string getTitle();
    bool getVSync();
    int getWidth();
    bool hasFocus();
    bool isMaximized();
    bool isMinimized();
    bool isOpen();
    bool isResized();
    bool isVisible();
    void maximize();
    void minimize();
    void restore();
    void setFullscreen(bool fullscreen);
    void setIcon(graphics::pImage image);
    void setMinSize(int width, int height);
    void setMode(sol::table flags);
    void setPosition(int x, int y);
    void setTitle(std::string title);
    void setVSync(bool vsync);
    void unsetMode(sol::table flags);
}
