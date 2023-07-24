#pragma once

#include <sol/sol.hpp>

namespace psystem // Name conflict with stdlib
{
    enum OS
    {
        Windows,
        Linux,
        MacOS
    };

    // Functions

    // Utility
    sol::table registerSystemAPI(sol::state& lua);

    // API
    std::string getClipboardText();
    OS getOS();
    void openURL(std::string url);
    void setClipboardText(std::string text);
}
