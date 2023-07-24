#pragma once

#include <sol/sol.hpp>

#include <string>
#include <vector>

namespace data
{
    // Enums
    enum HashFunction
    {
        Crc32,
        Md5,
        Sha1,
        Sha256
    };

    // Types
    class Data
    {
    public:
        std::vector<unsigned char> data;

        std::string type();
        void unload();

        Data clone();
        int getSize();
        sol::table getData();
        void append(sol::table data);

        // TODO: Add more functions
    };

    // Functions

    // Utility
    sol::table registerDataAPI(sol::state& lua);

    // API
    Data compress(Data data);
    Data decode(std::string data);
    Data decompress(Data data);
    std::string encode(Data data);
    std::string hash(std::string data, HashFunction hashFunction);
    std::string hash(Data data, HashFunction hashFunction);
    Data newData(sol::table data);
    void printTable(sol::table table);

    // serialization functions
}
