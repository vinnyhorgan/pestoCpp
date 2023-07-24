#include "debug.h"

#include "pesto.h"
#include "logger.h"
#include "error.h"

#include <raylib.h>

#include <filesystem>

bool isDebug = false;
debug::DebugMessage* debugMessage = nullptr;
long lastModified = 0;

namespace debug
{
    void setDebug(bool debug)
    {
        isDebug = debug;

        logger::setDebug(debug);
    }

    bool getDebug()
    {
        return isDebug;
    }

    void message(std::string message)
    {
        if (isDebug)
        {
            if (debugMessage != nullptr)
            {
                delete debugMessage;
                debugMessage = nullptr;
            }

            debugMessage = new DebugMessage(message);

            logger::debug(message);
        }
    }

    void update()
    {
        if (isDebug)
        {
            if (IsKeyDown(KEY_ESCAPE))
            {
                pesto::changeScreen(pesto::Screen::Manager);
                error::reset();
            }

            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R))
            {
                error::reset();

                message("Reloaded!");

                lastModified = GetFileModTime(pesto::getCurrentProject().c_str());

                pesto::runFolder(pesto::getCurrentProject());
            }

            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T))
            {
                if (FileExists("assets/pesto.d.tl"))
                {
                    unsigned int bytesRead;
                    unsigned char* data = LoadFileData("assets/pesto.d.tl", &bytesRead);
                    ExportDataAsCode(data, bytesRead, "src/embedded/pestoTypes.h");

                    message("Regenerated type definitions!");

                    if (DirectoryExists("projects"))
                    {
                        FilePathList projectPaths = LoadDirectoryFiles("projects");

                        for (int i = 0; i < (int)projectPaths.count; i++)
                        {
                            std::string projectPath = projectPaths.paths[i];
                            std::string pestoPath = projectPath + "/pesto.d.tl";

                            std::filesystem::copy_file("assets/pesto.d.tl", pestoPath, std::filesystem::copy_options::overwrite_existing);
                        }
                    }
                }
            }

            // make it recursive
            if (DirectoryExists(pesto::getCurrentProject().c_str()))
            {
                FilePathList files = LoadDirectoryFiles(pesto::getCurrentProject().c_str());

                for (int i = 0; i < (int)files.count; i++)
                {
                    char* filename = files.paths[i];

                    if (IsPathFile(filename))
                    {
                        if (IsFileExtension(filename, ".tl"))
                        {
                            if (GetFileModTime(filename) > lastModified)
                            {
                                error::reset();

                                message("Reloaded!");

                                lastModified = GetFileModTime(filename);

                                pesto::runFolder(pesto::getCurrentProject());
                            }
                        }
                    }
                }
            }
        }
    }

    void draw()
    {
        if (isDebug)
        {
            Font robotoSmall = pesto::getSmallFont();

            DrawTextEx(robotoSmall, TextFormat("FPS: %d", GetFPS()), { (float)GetScreenWidth() - MeasureTextEx(robotoSmall, TextFormat("FPS: %d", GetFPS()), (float)robotoSmall.baseSize, 0).x - 10, 10 }, (float)robotoSmall.baseSize, 0, GREEN);
            DrawTextEx(robotoSmall, pesto::getCurrentProject().c_str(), { (float)GetScreenWidth() - MeasureTextEx(robotoSmall, pesto::getCurrentProject().c_str(), (float)robotoSmall.baseSize, 0).x - 10, 50 }, (float)robotoSmall.baseSize, 0, GREEN);

            if (debugMessage != nullptr)
            {
                if (debugMessage->time > 0.0f)
                {
                    debugMessage->time -= GetFrameTime();
                    debugMessage->alpha--;

                    DrawTextEx(robotoSmall, debugMessage->message.c_str(), { (float)GetScreenWidth() - MeasureTextEx(robotoSmall, debugMessage->message.c_str(), (float)robotoSmall.baseSize, 0).x - 10, (float)GetScreenHeight() - 30 }, (float)robotoSmall.baseSize, 0, { 0, 255, 0, (unsigned char)debugMessage->alpha});

                    if (debugMessage->time <= 0.0f)
                    {
                        delete debugMessage;
                        debugMessage = nullptr;
                    }
                }
            }
        }
    }
}
