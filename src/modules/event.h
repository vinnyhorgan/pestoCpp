#pragma once

#include <sol/sol.hpp>
#include <raylib.h>

#include <string>

namespace event
{
    // Functions

    // Utility
    sol::table registerEventAPI(sol::state& lua);
    bool shouldQuit();
    void quit();
    void resetState();

    // API
    void run(std::string path);
    void reload();
    void lquit();
    void add(std::string name, sol::function function);
    void remove(std::string name);
    void call(std::string name, sol::variadic_args args);
    void clear();
}
