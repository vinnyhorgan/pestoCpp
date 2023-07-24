#include "event.h"

#include "../pesto.h"
#include "../error.h"

#include <map>

bool shouldQuitFlag = false;

std::map<std::string, sol::function> events;

namespace event
{
    // Functions

    // Utility
    sol::table registerEventAPI(sol::state& lua)
    {
        sol::table event = lua.create_table();

        event["run"] = run;
        event["reload"] = reload;
        event["quit"] = lquit;
        event["add"] = add;
        event["remove"] = remove;
        event["call"] = call;
        event["clear"] = clear;

        return event;
    }

    bool shouldQuit()
    {
        return shouldQuitFlag || WindowShouldClose();
    }

    void quit()
    {
        shouldQuitFlag = true;
    }

    void resetState()
    {
        events.clear();
    }

    // API
    void run(std::string path)
    {
        pesto::runFolder(pesto::getCurrentProject() + "/" + path);
    }

    void reload()
    {
        pesto::runFolder(pesto::getCurrentProject());
    }

    void lquit()
    {
        sol::protected_function luaQuit = pesto::getLua()["quit"];

        if (luaQuit.valid())
        {
            auto result = luaQuit();

            if (!result.valid())
            {
                sol::error err = result;
                error::throwError(err.what());

                return;
            }

            shouldQuitFlag = result;
        }
        else
        {
            shouldQuitFlag = true;
        }
    }

    void add(std::string name, sol::function function)
    {
        events[name] = function;
    }

    void remove(std::string name)
    {
        events.erase(name);
    }

    void call(std::string name, sol::variadic_args args)
    {
        if (events.find(name) != events.end())
        {
            events[name](args);
        }
    }

    void clear()
    {
        events.clear();
    }
}
