#include "system.h"

#include <raylib.h>

namespace psystem
{
    // Functions

    // Utility
    sol::table registerSystemAPI(sol::state& lua)
    {
        lua.new_enum<OS>("OS", {
            {"Windows", OS::Windows},
            {"Linux", OS::Linux},
            {"MacOS", OS::MacOS}
        });

        sol::table system = lua.create_table();

        system["getClipboardText"] = getClipboardText;
        system["getOS"] = getOS;
        system["openURL"] = openURL;
        system["setClipboardText"] = setClipboardText;

        return system;
    }

    // API
    std::string getClipboardText()
    {
        return GetClipboardText();
    }

    OS getOS()
    {
        #if defined(_WIN32)
            return OS::Windows;
        #elif defined(__APPLE__)
            return OS::MacOS;
        #elif defined(__linux__)
            return OS::Linux;
        #endif
    }

    void openURL(std::string url)
    {
        OpenURL(url.c_str());
    }

    void setClipboardText(std::string text)
    {
        SetClipboardText(text.c_str());
    }
}
