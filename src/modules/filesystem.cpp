#include "filesystem.h"

#include "../pesto.h"
#include "../logger.h"

#include <raylib.h>

#include <filesystem>

namespace filesystem
{
    // Functions

    // Utility
    sol::table registerFilesystemAPI(sol::state& lua)
    {
        sol::table filesystem = lua.create_table();

        filesystem["loadFileData"] = loadFileData;
        filesystem["saveFileData"] = saveFileData;
        filesystem["loadFileText"] = loadFileText;
        filesystem["saveFileText"] = saveFileText;
        filesystem["fileExists"] = fileExists;
        filesystem["directoryExists"] = directoryExists;
        filesystem["isFileExtension"] = isFileExtension;
        filesystem["getFileSize"] = getFileSize;
        filesystem["getFileExtension"] = getFileExtension;
        filesystem["getFileName"] = getFileName;
        filesystem["getFileNameWithoutExtension"] = getFileNameWithoutExtension;
        filesystem["getDirectoryPath"] = getDirectoryPath;
        filesystem["getPrevDirectoryPath"] = getPrevDirectoryPath;
        filesystem["getWorkingDirectory"] = getWorkingDirectory;
        filesystem["getAppDirectory"] = getAppDirectory;
        filesystem["changeDirectory"] = changeDirectory;
        filesystem["isPathFile"] = isPathFile;
        filesystem["getDirectoryFiles"] = getDirectoryFiles;
        filesystem["getFileModTime"] = getFileModTime;

        return filesystem;
    }

    // API (raylib)
    data::Data loadFileData(std::string filename)
    {
        data::Data data;

        unsigned int size = 0;
        unsigned char *fileData = LoadFileData(filename.c_str(), &size);

        if (fileData != NULL)
        {
            for (int i = 0; i < (int)size; i++)
            {
                data.data.push_back(fileData[i]);
            }

            UnloadFileData(fileData);

            logger::info("Loaded file data: " + filename);
        }
        else
        {
            logger::error("Failed to load file data: " + filename);
        }

        return data;
    }

    void saveFileData(std::string filename, data::Data data)
    {
        SaveFileData(filename.c_str(), data.data.data(), (unsigned int)data.data.size());

        logger::info("Saved file data: " + filename);
    }

    std::string loadFileText(std::string filename)
    {
        std::string text;

        char *fileText = LoadFileText(filename.c_str());

        if (fileText != NULL)
        {
            text = fileText;

            UnloadFileText(fileText);

            logger::info("Loaded file text: " + filename);
        }
        else
        {
            logger::error("Failed to load file text: " + filename);
        }

        return text;
    }

    void saveFileText(std::string filename, std::string text)
    {
        SaveFileText(filename.c_str(), const_cast<char*>(text.c_str()));

        logger::info("Saved file text: " + filename);
    }

    bool fileExists(std::string filename)
    {
        return FileExists(filename.c_str());
    }

    bool directoryExists(std::string path)
    {
        return DirectoryExists(path.c_str());
    }

    bool isFileExtension(std::string filename, std::string ext)
    {
        return IsFileExtension(filename.c_str(), ext.c_str());
    }

    int getFileSize(std::string filename)
    {
        return GetFileLength(filename.c_str());
    }

    std::string getFileExtension(std::string filename)
    {
        return GetFileExtension(filename.c_str());
    }

    std::string getFileName(std::string filename)
    {
        return GetFileName(filename.c_str());
    }

    std::string getFileNameWithoutExtension(std::string filename)
    {
        return GetFileNameWithoutExt(filename.c_str());
    }

    std::string getDirectoryPath(std::string filename)
    {
        return GetDirectoryPath(filename.c_str());
    }

    std::string getPrevDirectoryPath(std::string filename)
    {
        return GetPrevDirectoryPath(filename.c_str());
    }

    std::string getWorkingDirectory()
    {
        return GetWorkingDirectory();
    }

    std::string getAppDirectory()
    {
        return GetApplicationDirectory();
    }

    bool changeDirectory(std::string path)
    {
        return ChangeDirectory(path.c_str());
    }

    bool isPathFile(std::string path)
    {
        return IsPathFile(path.c_str());
    }

    sol::table getDirectoryFiles(std::string path)
    {
        sol::table files = pesto::getLua().create_table();

        FilePathList list = LoadDirectoryFiles(path.c_str());

        for (int i = 0; i < (int)list.count; i++)
        {
            files.add(list.paths[i]);
        }

        UnloadDirectoryFiles(list);

        return files;
    }

    float getFileModTime(std::string filename)
    {
        return (float)GetFileModTime(filename.c_str());
    }

    // Extra
    bool createDirectory(std::string path)
    {
        return std::filesystem::create_directory(path);
    }

    bool removeFile(std::string filename)
    {
        return std::filesystem::remove(filename);
    }

    bool removeDirectory(std::string path)
    {
        return std::filesystem::remove_all(path);
    }

    void copyFile(std::string src, std::string dst)
    {
        return std::filesystem::copy(src, dst);
    }

    void moveFile(std::string src, std::string dst)
    {
        return std::filesystem::rename(src, dst);
    }
}
