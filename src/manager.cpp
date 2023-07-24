#include "manager.h"

#include "pesto.h"
#include "debug.h"
#include "logger.h"

#include "embedded/pestoTypes.h"

#include <raylib.h>

#include <string>
#include <filesystem>

#define MAX_INPUT_CHARS 16

FilePathList projectPaths;

bool waitingForInput = false;
char name[MAX_INPUT_CHARS] = "\0";
int letterCount = 0;
Rectangle textBox = { 800 / 2 - 100, 600 / 2 - 50, 225, 50 };
bool mouseOnText = false;
int framesCounter = 0;

void exportProject(std::string path)
{
    logger::info("Starting export of project " + path);

    if (!DirectoryExists((path + "/build").c_str()))
    {
        logger::error("Build folder does not exist in project " + path);
        return;
    }

    if (DirectoryExists((path + "/exported").c_str()))
    {
        std::filesystem::remove_all(path + "/exported");

        logger::info("Removed old export folder");
    }

    std::filesystem::create_directory(path + "/exported");
    std::filesystem::create_directory(path + "/exported/data");

    if (DirectoryExists((path + "/assets").c_str()))
        std::filesystem::create_directory(path + "/exported/data/assets");

    std::filesystem::copy(path + "/build", path + "/exported/data", std::filesystem::copy_options::recursive);

    if (DirectoryExists((path + "/assets").c_str()))
        std::filesystem::copy(path + "/assets", path + "/exported/data/assets", std::filesystem::copy_options::recursive);

    std::string pestoPath = GetApplicationDirectory() + std::string("pesto.exe");

    std::filesystem::copy(pestoPath, path + "/exported/pesto.exe");

    logger::info("Finished export of project " + path);
}

namespace manager
{
    void init()
    {
        if (!DirectoryExists("projects"))
            std::filesystem::create_directory("projects");

        projectPaths = LoadDirectoryFiles("projects");
    }

    void draw()
    {
        if (waitingForInput)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS - 1))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0';
                    letterCount++;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }

            framesCounter++;
        }

        BeginDrawing();

            Font robotoSmall = pesto::getSmallFont();
            Font robotoMedium = pesto::getMediumFont();
            Font robotoLarge = pesto::getLargeFont();

            Texture logoTexture = pesto::getLogo();

            ClearBackground(DARKGRAY);

            DrawTexture(logoTexture, 10, 10, WHITE);

            DrawTextEx(robotoLarge, "Pesto Project Manager", { 10.0f + logoTexture.width + 20.0f, 10.0f + logoTexture.height / 2 - robotoLarge.baseSize / 2 }, (float)robotoLarge.baseSize, 0, WHITE);

            DrawTextEx(robotoMedium, "Projects", { 10, 10.0f + logoTexture.height + 50 }, (float)robotoMedium.baseSize, 0, WHITE);

            bool collides = false;
            bool showTooltip = false;

            for (int i = 0; i < (int)projectPaths.count; i++)
            {
                collides = CheckCollisionPointRec(GetMousePosition(), { 10, 10.0f + logoTexture.height + 50 + robotoMedium.baseSize + 10 + i * (robotoMedium.baseSize + 15), (float)GetScreenWidth() - 20, (float)robotoMedium.baseSize + 10 });

                if (collides)
                    showTooltip = true;

                if (collides)
                {
                    DrawRectangleRounded({ 10, 10.0f + logoTexture.height + 50 + robotoMedium.baseSize + 10 + i * (robotoMedium.baseSize + 15), (float)GetScreenWidth() - 20, (float)robotoMedium.baseSize + 10 }, 0.2f, 5, LIGHTGRAY);

                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        pesto::changeScreen(pesto::Screen::Game);
                        pesto::runFolder(projectPaths.paths[i]);
                    }

                    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                    {
                        const char* command = TextFormat("code %s", projectPaths.paths[i]);
                        system(command);

                        pesto::changeScreen(pesto::Screen::Game);
                        pesto::runFolder(projectPaths.paths[i]);
                    }

                    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
                    {
                        std::string fullPath = std::string(GetWorkingDirectory()) + "/" + std::string(projectPaths.paths[i]);

                        for (int i = 0; i < (int)fullPath.length(); i++)
                        {
                            if (fullPath[i] == '/')
                                fullPath[i] = '\\';
                        }

                        system(("explorer \"" + fullPath + "\"").c_str());
                    }

                    if (IsKeyPressed(KEY_E))
                    {
                        exportProject(projectPaths.paths[i]);
                    }
                }
                else
                {
                    DrawRectangleRounded({ 10, 10.0f + logoTexture.height + 50 + robotoMedium.baseSize + 10 + i * (robotoMedium.baseSize + 15), (float)GetScreenWidth() - 20, (float)robotoMedium.baseSize + 10 }, 0.2f, 5, GRAY);
                }

                std::string path = projectPaths.paths[i];
                path = path.substr(9, path.length() - 9);
                DrawTextEx(robotoMedium, path.c_str(), { 20, 10.0f + logoTexture.height + 50 + robotoMedium.baseSize + 10 + i * (robotoMedium.baseSize + 15) + 5 }, (float)robotoMedium.baseSize, 0, BLACK);
            }

            if (showTooltip)
            {
                DrawRectangleRounded({ (float)GetMouseX() + 10, (float)GetMouseY() + 10, 250, 100 }, 0.1f, 5, Fade(BLACK, 0.5f));
                DrawTextEx(robotoSmall, "Left click to open", { (float)GetMouseX() + 20, (float)GetMouseY() + 20 }, (float)robotoSmall.baseSize, 0, WHITE);
                DrawTextEx(robotoSmall, "Right click to open in Code", { (float)GetMouseX() + 20, (float)GetMouseY() + 20 + robotoSmall.baseSize + 5 }, (float)robotoSmall.baseSize, 0, WHITE);
                DrawTextEx(robotoSmall, "Middle click to open in Explorer", { (float)GetMouseX() + 20, (float)GetMouseY() + 20 + robotoSmall.baseSize * 2 + 10 }, (float)robotoSmall.baseSize, 0, WHITE);
                DrawTextEx(robotoSmall, "E to export", { (float)GetMouseX() + 20, (float)GetMouseY() + 20 + robotoSmall.baseSize * 3 + 15 }, (float)robotoSmall.baseSize, 0, WHITE);
            }

            DrawTextEx(robotoSmall, "Made with <3 by Vinny Horgan and contributors!", { 10, (float)GetScreenHeight() - robotoSmall.baseSize - 10 }, (float)robotoSmall.baseSize, 0, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), { (float)GetScreenWidth() - 10 - 50, (float)GetScreenHeight() - 10 - 50, 50, 50 }))
            {
                DrawRectangleRounded({ (float)GetScreenWidth() - 10 - 50, (float)GetScreenHeight() - 10 - 50, 50, 50 }, 0.5f, 5, LIGHTGRAY);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    waitingForInput = true;
                }
            }
            else
            {
                DrawRectangleRounded({ (float)GetScreenWidth() - 10 - 50, (float)GetScreenHeight() - 10 - 50, 50, 50 }, 0.5f, 5, GRAY);
            }

            if (CheckCollisionPointRec(GetMousePosition(), { (float)GetScreenWidth() - 10 - 160, (float)GetScreenHeight() - 10 - 50, 95, 50 }))
            {
                DrawRectangleRounded({ (float)GetScreenWidth() - 10 - 160, (float)GetScreenHeight() - 10 - 50, 95, 50 }, 0.5f, 5, LIGHTGRAY);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    pesto::changeScreen(pesto::Screen::Server);
                }
            }
            else
            {
                DrawRectangleRounded({ (float)GetScreenWidth() - 10 - 160, (float)GetScreenHeight() - 10 - 50, 95, 50 }, 0.5f, 5, GRAY);
            }

            DrawLineEx({ (float)GetScreenWidth() - 10 - 50 + 25 - 10, (float)GetScreenHeight() - 10 - 50 + 25 }, { (float)GetScreenWidth() - 10 - 50 + 25 + 10, (float)GetScreenHeight() - 10 - 50 + 25 }, 5, BLACK);
            DrawLineEx({ (float)GetScreenWidth() - 10 - 50 + 25, (float)GetScreenHeight() - 10 - 50 + 25 - 10 }, { (float)GetScreenWidth() - 10 - 50 + 25, (float)GetScreenHeight() - 10 - 50 + 25 + 10 }, 5, BLACK);

            DrawTextEx(robotoMedium, "Server", { (float)GetScreenWidth() - 10 - 150, (float)GetScreenHeight() - 10 - 40 }, (float)robotoMedium.baseSize, 0, BLACK);

            if (waitingForInput)
            {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.75f));

                DrawTextEx(robotoMedium, "Enter project name", { textBox.x, textBox.y - 50 }, (float)robotoMedium.baseSize, 0, WHITE);
                DrawRectangleRounded(textBox, 0.5f, 5, GRAY);
                DrawRectangleRoundedLines(textBox, 0.5f, 5, 2, BLACK);

                DrawTextEx(robotoMedium, name, { textBox.x + 5, textBox.y + 8 }, (float)robotoMedium.baseSize, 0, BLACK);

                if (letterCount < MAX_INPUT_CHARS)
                {
                    if (((framesCounter/20)%2) == 0) DrawTextEx(robotoMedium, "_", { textBox.x + 8 + MeasureTextEx(robotoMedium, name, (float)robotoMedium.baseSize, 0).x, textBox.y + 12 }, (float)robotoMedium.baseSize, 0, BLACK);
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    std::string projectName = "projects/" + std::string(name);

                    if (std::filesystem::create_directory(projectName))
                    {
                        std::string gitignore = "build/\nexported/\n";

                        std::string defaultCode = R"(global function update(dt: number)

end

global function draw()
    pesto.graphics.print("Hello!", 10, 10)
end
)";

                        std::string tlconfig = R"(return {
    source_dir = ".",
    build_dir = "build",
    include_dir = {
        "*.tl"
    },
    global_env_def = "pesto",
}
)";

                        std::string extensions = R"({
    "recommendations": [
        "pkief.material-icon-theme",
        "pdesaulniers.vscode-teal"
    ]
}
)";

                        std::string settings = R"({
    "editor.renderWhitespace": "all",
    "files.trimTrailingWhitespace": true,
    "files.insertFinalNewline": true
}
)";

                        SaveFileText((projectName + "/.gitignore").c_str(), const_cast<char*>(gitignore.c_str()));
                        SaveFileText((projectName + "/main.tl").c_str(), const_cast<char*>(defaultCode.c_str()));
                        SaveFileText((projectName + "/tlconfig.lua").c_str(), const_cast<char*>(tlconfig.c_str()));
                        SaveFileData((projectName + "/pesto.d.tl").c_str(), PESTOTYPES_DATA, sizeof(PESTOTYPES_DATA));

                        std::filesystem::create_directory(projectName + "/.vscode");

                        SaveFileText((projectName + "/.vscode/extensions.json").c_str(), const_cast<char*>(extensions.c_str()));
                        SaveFileText((projectName + "/.vscode/settings.json").c_str(), const_cast<char*>(settings.c_str()));

                        projectPaths = LoadDirectoryFiles("projects");

                        waitingForInput = false;
                    }
                    else
                    {
                        debug::message("Project already exists!");

                        waitingForInput = false;
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE))
                {
                    waitingForInput = false;
                }
            }

        EndDrawing();
    }
}
