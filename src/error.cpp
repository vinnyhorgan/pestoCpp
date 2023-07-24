#include "error.h"

#include "pesto.h"
#include "logger.h"

#include <raylib.h>

bool isError = false;
std::string errorMessage = "";
bool copied = false;

namespace error
{
    bool state()
    {
        return isError;
    }

    void reset()
    {
        isError = false;
        errorMessage = "";
        copied = false;
    }

    void throwError(std::string message)
    {
        logger::error(message);

        isError = true;
        errorMessage = message;
    }

    void draw()
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            SetClipboardText(errorMessage.c_str());
            copied = true;
        }

        BeginDrawing();

            ClearBackground(SKYBLUE);

            Font mediumFont = pesto::getMediumFont();
            Font smallFont = pesto::getSmallFont();

            DrawTextEx(mediumFont, "Error: (click to copy to clipboard)", { 10, 10 }, (float)mediumFont.baseSize, 0, WHITE);
            DrawTextEx(smallFont, errorMessage.c_str(), { 10, 40 }, (float)smallFont.baseSize, 0, WHITE);

            if (copied)
            {
                DrawTextEx(mediumFont, "Copied to clipboard!", { 10, (float)GetScreenHeight() - 40 }, (float)mediumFont.baseSize, 0, WHITE);
            }

        EndDrawing();
    }
}
