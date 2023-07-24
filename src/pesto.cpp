#include "pesto.h"

#include "logger.h"
#include "error.h"
#include "debug.h"

#include "manager.h"
#include "server.h"

#include "modules/ai.h"
#include "modules/audio.h"
#include "modules/data.h"
#include "modules/event.h"
#include "modules/filesystem.h"
#include "modules/gamepad.h"
#include "modules/graphics.h"
#include "modules/gui.h"
#include "modules/keyboard.h"
#include "modules/math.h"
#include "modules/mouse.h"
#include "modules/networking.h"
#include "modules/physics.h"
#include "modules/system.h"
#include "modules/timer.h"
#include "modules/window.h"

#include <raylib.h>
#include <raymath.h>
#include <rlImGui.h>

#include "embedded/icon.h"
#include "embedded/robotoSmall.h"
#include "embedded/robotoMedium.h"
#include "embedded/robotoLarge.h"
#include "embedded/tl.h"

#define PESTO_VERSION_STRING "0.1.0 alpha"
#define PESTO_VERSION_MAJOR 0
#define PESTO_VERSION_MINOR 1
#define PESTO_VERSION_PATCH 0
#define PESTO_VERSION_CODENAME "alpha"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

bool release = false;

std::string currentProject;
Texture logo;
Font robotoSmall;
Font robotoMedium;
Font robotoLarge;

int defaultWidth = 800;
int defaultHeight = 600;

bool letterbox = true;
RenderTexture2D target;

pesto::Screen currentScreen = pesto::Screen::Manager;

sol::state lua;

sol::protected_function luaUpdate;
sol::protected_function luaDraw;
sol::protected_function luaGui;

std::tuple<int, int, int, std::string> getVersion()
{
    return std::make_tuple(PESTO_VERSION_MAJOR, PESTO_VERSION_MINOR, PESTO_VERSION_PATCH, PESTO_VERSION_CODENAME);
}

// Thanks StackOverflow!
std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}

// integrate better the runRelease and runFolder functions
void runRelease(std::string path)
{
    currentProject = path;

    lua["package"]["path"] = (lua["package"]["path"].get<std::string>() + ";" + path + "/?.lua").c_str(); // Magic line :)

    auto result = lua.safe_script_file(path + "/main.lua", sol::script_pass_on_error);

    if (!result.valid())
    {
        sol::error err = result;
        error::throwError(err.what());

        return;
    }

    SetWindowTitle("Pesto v" PESTO_VERSION_STRING);

    luaUpdate = lua["update"];
    luaDraw = lua["draw"];
    luaGui = lua["gui"];
}

namespace pesto
{
    void runFolder(std::string path)
    {
        // Reset state
        event::resetState();
        gamepad::resetState();
        graphics::resetState();
        math::resetState();
        networking::resetState();
        timer::resetState();
        window::resetState();

        // Basic error checking
        if (!FileExists((path + "/main.tl").c_str()))
        {
            error::throwError("Path does not contain main.tl");

            return;
        }

        if (!FileExists((path + "/pesto.d.tl").c_str()))
        {
            error::throwError("Path does not contain type definition file!");

            return;
        }

        currentProject = path; // fix first error not reloading bug :)

        // Compile
        std::string typeCheck = exec(("cd " + path + " && ..\\..\\tl.exe build 2>&1").c_str()); // 2>&1 redirects stderr to stdout

        if (typeCheck.find("warning:") != std::string::npos || typeCheck.find("warnings:") != std::string::npos)
        {
            logger::warn("Warning compiling script: " + typeCheck);
            debug::message("The script compiled with warnings!");
        }

        if (typeCheck.find("error:") != std::string::npos || typeCheck.find("errors:") != std::string::npos)
        {
            error::throwError(typeCheck);

            return;
        }

        // Run the script

        lua["package"]["path"] = (lua["package"]["path"].get<std::string>() + ";" + path + "/build/?.lua").c_str(); // Magic line :)

        auto result = lua.safe_script_file(path + "/build/main.lua", sol::script_pass_on_error);

        if (!result.valid())
        {
            sol::error err = result;
            error::throwError(err.what());

            return;
        }

        logger::info("Successfully ran " + path);

        SetWindowTitle(("Pesto v" PESTO_VERSION_STRING " - " + path).c_str());

        luaUpdate = lua["update"];
        luaDraw = lua["draw"];
        luaGui = lua["gui"];
    }

    void changeScreen(Screen screen)
    {
        currentScreen = screen;

        if (screen == Screen::Game)
        {

        }
        else if (screen == Screen::Manager)
        {
            manager::init();
            SetWindowTitle("Pesto v" PESTO_VERSION_STRING " - Project Manager");
            currentProject = "";
        }
        else if (screen == Screen::Server)
        {
            server::init();
            SetWindowTitle("Pesto v" PESTO_VERSION_STRING " - Server");
            currentProject = "";
        }
    }

    sol::state& getLua()
    {
        return lua;
    }

    std::string getCurrentProject()
    {
        return currentProject;
    }

    int getGameWidth()
    {
        return defaultWidth;
    }

    int getGameHeight()
    {
        return defaultHeight;
    }

    Texture getLogo()
    {
        return logo;
    }

    Font getSmallFont()
    {
        return robotoSmall;
    }

    Font getMediumFont()
    {
        return robotoMedium;
    }

    Font getLargeFont()
    {
        return robotoLarge;
    }

    bool getLetterbox()
    {
        return letterbox;
    }

    void enableLetterbox(int width, int height)
    {
        letterbox = true;

        target = LoadRenderTexture(width, height);
        SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

        logger::info("Enabled letterbox");
    }

    void disableLetterbox()
    {
        UnloadRenderTexture(target);
        letterbox = false;

        logger::info("Disabled letterbox");
    }

    void init(int argc, char* argv[])
    {
        if (DirectoryExists("data"))
        {
            if (!FileExists("data/main.lua"))
            {
                logger::warn("data folder exists but does not contain main.lua!");
            }
            else
            {
                release = true;

                logger::info("Running from release folder");
            }
        }
        else
            debug::setDebug(true);

        logger::info("Pesto v" PESTO_VERSION_STRING);

        std::string debugMode = debug::getDebug() ? "true" : "false";
        logger::info("Debug mode: " + debugMode);

        // Window
        SetTraceLogCallback(logger::rayLog);

        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
        InitWindow(defaultWidth, defaultHeight, "Pesto v" PESTO_VERSION_STRING " - Project Manager");
        SetWindowMinSize(defaultWidth, defaultHeight);
        // SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
        SetExitKey(0);

        InitAudioDevice();

        enableLetterbox(defaultWidth, defaultHeight);

        rlImGuiSetup(true);

        Image logoImage;

        logoImage.data = ICON_DATA;
        logoImage.width = ICON_WIDTH;
        logoImage.height = ICON_HEIGHT;
        logoImage.format = ICON_FORMAT;
        logoImage.mipmaps = 1;

        SetWindowIcon(logoImage);

        logo = LoadTextureFromImage(logoImage);

        robotoSmall = LoadFont_RobotoSmall();
        robotoMedium = LoadFont_RobotoMedium();
        robotoLarge = LoadFont_RobotoLarge();

        if (!FileExists("tl.exe") && !release)
        {
            SaveFileData("tl.exe", TL_DATA, sizeof(TL_DATA));

            logger::info("Extracted embedded tl.exe");
        }

        logger::info("Loaded embedded resources successfully");

        // Lua
        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::utf8, sol::lib::table, sol::lib::math, sol::lib::os, sol::lib::bit32);

        lua["pesto"] = lua.create_table();

        lua["pesto"]["getVersion"] = getVersion;

        lua["pesto"]["ai"] = ai::registerAiAPI(lua);
        lua["pesto"]["audio"] = audio::registerAudioAPI(lua);
        lua["pesto"]["data"] = data::registerDataAPI(lua);
        lua["pesto"]["event"] = event::registerEventAPI(lua);
        lua["pesto"]["filesystem"] = filesystem::registerFilesystemAPI(lua);
        lua["pesto"]["gamepad"] = gamepad::registerGamepadAPI(lua);
        lua["pesto"]["graphics"] = graphics::registerGraphicsAPI(lua);
        lua["pesto"]["gui"] = gui::registerGuiAPI(lua);
        lua["pesto"]["keyboard"] = keyboard::registerKeyboardAPI(lua);
        lua["pesto"]["math"] = math::registerMathAPI(lua);
        lua["pesto"]["mouse"] = mouse::registerMouseAPI(lua);
        lua["pesto"]["networking"] = networking::registerNetworkingAPI(lua);
        lua["pesto"]["physics"] = physics::registerPhysicsAPI(lua);
        lua["pesto"]["system"] = psystem::registerSystemAPI(lua);
        lua["pesto"]["timer"] = timer::registerTimerAPI(lua);
        lua["pesto"]["window"] = window::registerWindowAPI(lua);

        logger::info("Registered Lua APIs successfully");

        if (argc == 1)
        {
            if (!release)
            {
                changeScreen(Screen::Manager);

                logger::info("No project specified, opening project manager");
            }
            else
            {
                changeScreen(Screen::Game);

                logger::info("Running from release folder");

                runRelease("data");
            }
        }
        else
        {
            if (!release)
            {
                changeScreen(Screen::Game);

                logger::info("Opening project: " + std::string(argv[1]));

                runFolder(argv[1]);
            }
        }
    }

    void run()
    {
        while (!event::shouldQuit())
        {
            if (error::state())
            {
                debug::update();

                error::draw();

                debug::draw();
            }
            else if (currentScreen == Screen::Manager)
            {
                manager::draw();

                debug::draw();
            }
            else if (currentScreen == Screen::Server)
            {
                server::update();

                server::draw();

                debug::draw();
            }
            else if (currentScreen == Screen::Game)
            {
                debug::update();

                gamepad::update();
                networking::update();
                timer::update();
                window::update();

                if (luaUpdate.valid())
                {
                    auto result = luaUpdate(GetFrameTime());

                    if (!result.valid())
                    {
                        sol::error err = result;
                        error::throwError(err.what());
                    }
                }

                if (letterbox)
                {
                    float scale = MIN((float)GetScreenWidth() / defaultWidth, (float)GetScreenHeight() / defaultHeight);

                    mouse::update(scale);

                    BeginTextureMode(target);

                        ClearBackground(BLACK);

                        if (luaDraw.valid())
                        {
                            auto result = luaDraw();

                            if (!result.valid())
                            {
                                sol::error err = result;
                                error::throwError(err.what());
                            }
                        }

                    EndTextureMode();

                    BeginDrawing();

                        ClearBackground(BLACK);

                        DrawTexturePro(target.texture, { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                                    { (GetScreenWidth() - ((float)defaultWidth*scale))*0.5f, (GetScreenHeight() - ((float)defaultHeight*scale))*0.5f,
                                    (float)defaultWidth*scale, (float)defaultHeight*scale }, { 0, 0 }, 0.0f, WHITE);

                        rlImGuiBegin();

                        if (luaGui.valid())
                        {
                            auto result = luaGui();

                            if (!result.valid())
                            {
                                sol::error err = result;
                                error::throwError(err.what());
                            }
                        }

                        gui::updateFileDialog(lua);

                        rlImGuiEnd();

                        debug::draw();

                    EndDrawing();
                }
                else
                {
                    BeginDrawing();

                        ClearBackground(BLACK);

                        if (luaDraw.valid())
                        {
                            auto result = luaDraw();

                            if (!result.valid())
                            {
                                sol::error err = result;
                                error::throwError(err.what());
                            }
                        }

                        rlImGuiBegin();

                        if (luaGui.valid())
                        {
                            auto result = luaGui();

                            if (!result.valid())
                            {
                                sol::error err = result;
                                error::throwError(err.what());
                            }
                        }

                        gui::updateFileDialog(lua);

                        rlImGuiEnd();

                        debug::draw();

                    EndDrawing();
                }
            }
        }
    }

    void shutdown()
    {
        // Trying to free the embedded resources crashes the program

        if (letterbox)
            UnloadRenderTexture(target);

        if (currentScreen == Screen::Server)
            server::deinit();

        rlImGuiShutdown();

        CloseAudioDevice();

        CloseWindow();
    }
}
