#pragma once

#include "data.h"

#include <sol/sol.hpp>

namespace filesystem
{
    // Functions

    // Utility
    sol::table registerFilesystemAPI(sol::state& lua);

    // API (raylib)
    data::Data loadFileData(std::string filename);
    void saveFileData(std::string filename, data::Data data);
    std::string loadFileText(std::string filename);
    void saveFileText(std::string filename, std::string text);
    bool fileExists(std::string filename);
    bool directoryExists(std::string path);
    bool isFileExtension(std::string filename, std::string ext);
    int getFileSize(std::string filename);
    std::string getFileExtension(std::string filename);
    std::string getFileName(std::string filename);
    std::string getFileNameWithoutExtension(std::string filename);
    std::string getDirectoryPath(std::string filename);
    std::string getPrevDirectoryPath(std::string filename);
    std::string getWorkingDirectory();
    std::string getAppDirectory();
    bool changeDirectory(std::string path);
    bool isPathFile(std::string path);
    sol::table getDirectoryFiles(std::string path);
    float getFileModTime(std::string filename);

    // Extra
    bool createDirectory(std::string path);
    bool removeFile(std::string filename);
    bool removeDirectory(std::string path);
    void copyFile(std::string src, std::string dst);
    void moveFile(std::string src, std::string dst);
}
