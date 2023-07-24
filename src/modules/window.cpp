#include "window.h"

#include "../pesto.h"
#include "../error.h"

#include <raylib.h>

std::string currentTitle = "";
graphics::pImage currentIcon;
bool focused = true;
bool visible = true;

namespace window
{
    // Functions

    // Utility
    sol::table registerWindowAPI(sol::state& lua)
    {
        currentIcon.texture = pesto::getLogo();

        lua.new_enum<ConfigFlags>("Flag", {
            {"VSync", FLAG_VSYNC_HINT},
            {"Fullscreen", FLAG_FULLSCREEN_MODE},
            {"Resizable", FLAG_WINDOW_RESIZABLE},
            {"Undecorated", FLAG_WINDOW_UNDECORATED},
            {"Hidden", FLAG_WINDOW_HIDDEN},
            {"Minimized", FLAG_WINDOW_MINIMIZED},
            {"Maximized", FLAG_WINDOW_MAXIMIZED},
            {"Unfocused", FLAG_WINDOW_UNFOCUSED},
            {"Topmost", FLAG_WINDOW_TOPMOST},
            {"AlwaysRun", FLAG_WINDOW_ALWAYS_RUN},
            {"Transparent", FLAG_WINDOW_TRANSPARENT},
            {"HighDPI", FLAG_WINDOW_HIGHDPI},
            {"MousePassthrough", FLAG_WINDOW_MOUSE_PASSTHROUGH},
            {"MSAA4X", FLAG_MSAA_4X_HINT},
            {"Interlaced", FLAG_INTERLACED_HINT}
        });

        sol::table window = lua.create_table();

        window["enableLetterbox"] = enableLetterbox;
        window["disableLetterbox"] = disableLetterbox;
        window["getDPIScale"] = getDPIScale;
        window["getDesktopDimensions"] = getDesktopDimensions;
        window["getDimensions"] = getDimensions;
        window["getDisplayCount"] = getDisplayCount;
        window["getDisplayName"] = getDisplayName;
        window["getFullscreen"] = getFullscreen;
        window["getHeight"] = getHeight;
        window["getIcon"] = getIcon;
        window["getLetterbox"] = getLetterbox;
        window["getMode"] = getMode;
        window["getPosition"] = getPosition;
        window["getTitle"] = getTitle;
        window["getVSync"] = getVSync;
        window["getWidth"] = getWidth;
        window["hasFocus"] = hasFocus;
        window["isMaximized"] = isMaximized;
        window["isMinimized"] = isMinimized;
        window["isOpen"] = isOpen;
        window["isResized"] = isResized;
        window["isVisible"] = isVisible;
        window["maximize"] = maximize;
        window["minimize"] = minimize;
        window["restore"] = restore;
        window["setFullscreen"] = setFullscreen;
        window["setIcon"] = setIcon;
        window["setMinSize"] = setMinSize;
        window["setMode"] = setMode;
        window["setPosition"] = setPosition;
        window["setTitle"] = setTitle;
        window["setVSync"] = setVSync;
        window["unsetMode"] = unsetMode;

        return window;
    }

    void update()
    {
        sol::state& lua = pesto::getLua();

        if (IsWindowResized())
        {
            sol::protected_function luaResize = lua["resize"];

            if (luaResize.valid())
            {
                auto result = luaResize(GetScreenWidth(), GetScreenHeight());

                if (!result.valid())
                {
                    sol::error err = result;
                    error::throwError(err.what());
                }
            }
        }

        if (IsFileDropped())
        {
            sol::protected_function luaFiledropped = lua["filedropped"];

            if (luaFiledropped.valid())
            {
                FilePathList droppedFiles = LoadDroppedFiles();

                sol::table droppedFilesTable = lua.create_table();

                for (int i = 0; i < (int)droppedFiles.count; i++)
                {
                    std::string file = droppedFiles.paths[i];

                    droppedFilesTable.add(file);
                }

                auto result = luaFiledropped(droppedFilesTable);

                if (!result.valid())
                {
                    sol::error err = result;
                    error::throwError(err.what());
                }

                UnloadDroppedFiles(droppedFiles);
            }
        }

        if (focused != IsWindowFocused())
        {
            focused = IsWindowFocused();

            sol::protected_function luaFocus = lua["focus"];

            if (luaFocus.valid())
            {
                auto result = luaFocus(focused);

                if (!result.valid())
                {
                    sol::error err = result;
                    error::throwError(err.what());
                }
            }
        }

        if (visible != !IsWindowHidden())
        {
            visible = !IsWindowHidden();

            sol::protected_function luaVisible = lua["visible"];

            if (luaVisible.valid())
            {
                auto result = luaVisible(visible);

                if (!result.valid())
                {
                    sol::error err = result;
                    error::throwError(err.what());
                }
            }
        }
    }

    void resetState()
    {
        currentTitle = "";
        focused = true;
        visible = true;
    }

    // API
    void enableLetterbox(int width, int height)
    {
        pesto::enableLetterbox(width, height);
    }

    void disableLetterbox()
    {
        pesto::disableLetterbox();
    }

    std::tuple<float, float> getDPIScale()
    {
        return std::make_tuple(GetWindowScaleDPI().x, GetWindowScaleDPI().y);
    }

    std::tuple<int, int> getDesktopDimensions(int display)
    {
        return std::make_tuple(GetMonitorWidth(display), GetMonitorHeight(display));
    }

    std::tuple<int, int> getDimensions()
    {
        if (getLetterbox())
        {
            return std::make_tuple(pesto::getGameWidth(), pesto::getGameHeight());
        }
        else
        {
            return std::make_tuple(GetScreenWidth(), GetScreenHeight());
        }
    }

    int getDisplayCount()
    {
        return GetMonitorCount();
    }

    std::string getDisplayName(int display)
    {
        return GetMonitorName(display);
    }

    bool getFullscreen()
    {
        return IsWindowFullscreen();
    }

    int getHeight()
    {
        if (getLetterbox())
        {
            return pesto::getGameHeight();
        }
        else
        {
            return GetScreenHeight();
        }
    }

    graphics::pImage getIcon()
    {
        return currentIcon;
    }

    bool getLetterbox()
    {
        return pesto::getLetterbox();
    }

    sol::table getMode()
    {
        sol::table flags = pesto::getLua().create_table();

        if (IsWindowState(FLAG_FULLSCREEN_MODE))
        {
            flags.add(FLAG_FULLSCREEN_MODE);
        }

        if (IsWindowState(FLAG_WINDOW_RESIZABLE))
        {
            flags.add(FLAG_WINDOW_RESIZABLE);
        }

        if (IsWindowState(FLAG_WINDOW_UNDECORATED))
        {
            flags.add(FLAG_WINDOW_UNDECORATED);
        }

        if (IsWindowState(FLAG_WINDOW_HIDDEN))
        {
            flags.add(FLAG_WINDOW_HIDDEN);
        }

        if (IsWindowState(FLAG_WINDOW_MINIMIZED))
        {
            flags.add(FLAG_WINDOW_MINIMIZED);
        }

        if (IsWindowState(FLAG_WINDOW_MAXIMIZED))
        {
            flags.add(FLAG_WINDOW_MAXIMIZED);
        }

        if (IsWindowState(FLAG_WINDOW_UNFOCUSED))
        {
            flags.add(FLAG_WINDOW_UNFOCUSED);
        }

        if (IsWindowState(FLAG_WINDOW_TOPMOST))
        {
            flags.add(FLAG_WINDOW_TOPMOST);
        }

        if (IsWindowState(FLAG_WINDOW_ALWAYS_RUN))
        {
            flags.add(FLAG_WINDOW_ALWAYS_RUN);
        }

        if (IsWindowState(FLAG_WINDOW_TRANSPARENT))
        {
            flags.add(FLAG_WINDOW_TRANSPARENT);
        }

        if (IsWindowState(FLAG_WINDOW_HIGHDPI))
        {
            flags.add(FLAG_WINDOW_HIGHDPI);
        }

        if (IsWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH))
        {
            flags.add(FLAG_WINDOW_MOUSE_PASSTHROUGH);
        }

        if (IsWindowState(FLAG_MSAA_4X_HINT))
        {
            flags.add(FLAG_MSAA_4X_HINT);
        }

        if (IsWindowState(FLAG_INTERLACED_HINT))
        {
            flags.add(FLAG_INTERLACED_HINT);
        }

        return flags;
    }

    std::tuple<int, int> getPosition()
    {
        return std::make_tuple((int)GetWindowPosition().x, (int)GetWindowPosition().y);
    }

    std::string getTitle()
    {
        return currentTitle;
    }

    bool getVSync()
    {
        return IsWindowState(FLAG_VSYNC_HINT);
    }

    int getWidth()
    {
        if (pesto::getLetterbox())
        {
            return pesto::getGameWidth();
        }
        else
        {
            return GetScreenWidth();
        }
    }

    bool hasFocus()
    {
        return IsWindowFocused();
    }

    bool isMaximized()
    {
        return IsWindowMaximized();
    }

    bool isMinimized()
    {
        return IsWindowMinimized();
    }

    bool isOpen()
    {
        return IsWindowReady();
    }

    bool isResized()
    {
        return IsWindowResized();
    }

    bool isVisible()
    {
        return !IsWindowHidden();
    }

    void maximize()
    {
        MaximizeWindow();
    }

    void minimize()
    {
        MinimizeWindow();
    }

    void restore()
    {
        RestoreWindow();
    }

    void setFullscreen(bool fullscreen)
    {
        if (fullscreen && !IsWindowFullscreen())
        {
            ToggleFullscreen();
        }
        else if (!fullscreen && IsWindowFullscreen())
        {
            ToggleFullscreen();
        }
    }

    void setMode(sol::table flags)
    {
        int flag = 0;

        for (auto& pair : flags)
        {
            ConfigFlags configFlag = (ConfigFlags)pair.second.as<int>();

            flag |= configFlag;
        }

        SetWindowState(flag);
    }

    void setIcon(graphics::pImage image)
    {
        Image icon = LoadImageFromTexture(image.texture);
        SetWindowIcon(icon);
        UnloadImage(icon);

        currentIcon = image;
    }

    void setMinSize(int width, int height)
    {
        SetWindowMinSize(width, height);
    }

    void setPosition(int x, int y)
    {
        SetWindowPosition(x, y);
    }

    void setTitle(std::string title)
    {
        currentTitle = title;
        SetWindowTitle(title.c_str());
    }

    void setVSync(bool vsync)
    {
        if (vsync)
        {
            SetWindowState(FLAG_VSYNC_HINT);
        }
        else
        {
            ClearWindowState(FLAG_VSYNC_HINT);
        }
    }

    void unsetMode(sol::table flags)
    {
        int flag = 0;

        for (auto& pair : flags)
        {
            ConfigFlags configFlag = (ConfigFlags)pair.second.as<int>();

            flag |= configFlag;
        }

        ClearWindowState(flag);
    }
}
