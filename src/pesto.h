#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <string>

// Forward declarations
struct Texture;
struct Font;

namespace pesto
{
    enum Screen
    {
        Manager,
        Server,
        Game
    };

    void runFolder(std::string path);
    void changeScreen(Screen screen);
    sol::state& getLua();
    std::string getCurrentProject();
    int getGameWidth();
    int getGameHeight();
    Texture getLogo();
    Font getSmallFont();
    Font getMediumFont();
    Font getLargeFont();
    bool getLetterbox();
    void enableLetterbox(int width, int height);
    void disableLetterbox();

    void init(int argc, char* argv[]);
    void run();
    void shutdown();
}
