#pragma once

#include <sol/sol.hpp>

namespace gui
{
    // Functions

    // Utility
    sol::table registerGuiAPI(sol::state& lua);
    void updateFileDialog(sol::state& lua);

    // API
    void enableDocking();
    void disableDocking();
    void enableKeyboard();
    void disableKeyboard();
    void dockSpace();
    bool begin(std::string title, bool open, sol::table flags);
    void end();
    void text(std::string text);
    bool button(std::string text);
    bool beginMainMenuBar();
    void endMainMenuBar();
    bool beginMenuBar();
    void endMenuBar();
    bool beginMenu(std::string label);
    void endMenu();
    bool menuItem(std::string label);
    int sliderInt(std::string label, int value, int min, int max);
    float sliderFloat(std::string label, float value, float min, float max);
    bool checkbox(std::string label, bool value);
    std::string inputText(std::string label, std::string value);
    void sameLine();
    void separator();
    void beginChild(std::string label);
    void endChild();
    void textColored(std::string text, int r, int g, int b, int a);
    void openFileDialog(std::string title, std::string filter);
}
