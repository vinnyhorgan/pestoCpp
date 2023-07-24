#include "data.h"

#include "../pesto.h"
#include "../logger.h"

#include <raylib.h>

#include <crc32.h>
#include <md5.h>
#include <sha1.h>
#include <sha256.h>

#include <iostream>

namespace data
{
    // Types
    std::string Data::type()
    {
        return "Data";
    }

    void Data::unload()
    {
        std::vector<unsigned char>().swap(data);
    }

    Data Data::clone()
    {
        Data newData;

        for (int i = 0; i < (int)data.size(); i++)
        {
            newData.data.push_back(data[i]);
        }

        return newData;
    }

    int Data::getSize()
    {
        return (int)data.size();
    }

    sol::table Data::getData()
    {
        sol::table data = pesto::getLua().create_table();

        for (int i = 0; i < (int)this->data.size(); i++)
        {
            data[i + 1] = this->data[i];
        }

        return data;
    }

    void Data::append(sol::table data)
    {
        for (int i = 0; i < (int)data.size(); i++)
        {
            this->data.push_back(data[i + 1]);
        }
    }

    // Functions

    // Utility
    sol::table registerDataAPI(sol::state& lua)
    {
        lua.new_enum<HashFunction>("HashFunction", {
            {"Crc32", HashFunction::Crc32},
            {"Md5", HashFunction::Md5},
            {"Sha1", HashFunction::Sha1},
            {"Sha256", HashFunction::Sha256}
        });

        sol::usertype<Data> dataType = lua.new_usertype<Data>("Data");

        dataType["type"] = &Data::type;
        dataType["unload"] = &Data::unload;
        dataType["clone"] = &Data::clone;
        dataType["getSize"] = &Data::getSize;
        dataType["getData"] = &Data::getData;
        dataType["append"] = &Data::append;

        sol::table data = lua.create_table();

        data["compress"] = compress;
        data["decode"] = decode;
        data["decompress"] = decompress;
        data["encode"] = encode;
        data["hash"] = sol::overload(sol::resolve<std::string(std::string, HashFunction)>(hash), sol::resolve<std::string(Data, HashFunction)>(hash));
        data["newData"] = newData;
        data["printTable"] = printTable;

        return data;
    }

    // API
    Data compress(Data data)
    {
        int outputSize;
        unsigned char* compressed = CompressData(data.data.data(), (int)data.data.size(), &outputSize);

        Data compressedData;

        for (int i = 0; i < outputSize; i++)
        {
            compressedData.data.push_back(compressed[i]);
        }

        MemFree(compressed);

        logger::info("Compressed data from " + std::to_string(data.data.size()) + " bytes to " + std::to_string(compressedData.data.size()) + " bytes");

        return compressedData;
    }

    Data decode(std::string data)
    {
        int outputSize;
        unsigned char* decoded = DecodeDataBase64(reinterpret_cast<const unsigned char*>(data.c_str()), &outputSize);

        Data decodedData;

        for (int i = 0; i < outputSize; i++)
        {
            decodedData.data.push_back(decoded[i]);
        }

        MemFree(decoded);

        logger::info("Decoded data from " + std::to_string(data.size()) + " bytes to " + std::to_string(decodedData.data.size()) + " bytes");

        return decodedData;
    }

    Data decompress(Data data)
    {
        int outputSize;
        unsigned char* decompressed = DecompressData(data.data.data(), (int)data.data.size(), &outputSize);

        Data decompressedData;

        for (int i = 0; i < outputSize; i++)
        {
            decompressedData.data.push_back(decompressed[i]);
        }

        MemFree(decompressed);

        logger::info("Decompressed data from " + std::to_string(data.data.size()) + " bytes to " + std::to_string(decompressedData.data.size()) + " bytes");

        return decompressedData;
    }

    std::string encode(Data data)
    {
        int outputSize;
        char* encoded = EncodeDataBase64(data.data.data(), (int)data.data.size(), &outputSize);

        std::string encodedData(encoded);

        MemFree(encoded);

        logger::info("Encoded data from " + std::to_string(data.data.size()) + " bytes to " + std::to_string(encodedData.size()) + " bytes");

        return encodedData;
    }

    std::string hash(std::string data, HashFunction hashFunction)
    {
        switch (hashFunction)
        {
            case HashFunction::Crc32:
            {
                CRC32 crc32;
                return crc32(data);

                break;
            }
            case HashFunction::Md5:
            {
                MD5 md5;
                return md5(data);

                break;
            }
            case HashFunction::Sha1:
            {
                SHA1 sha1;
                return sha1(data);

                break;
            }
            case HashFunction::Sha256:
            {
                SHA256 sha256;
                return sha256(data);

                break;
            }
            default:
            {
                return "";
            }
        }
    }

    std::string hash(Data data, HashFunction hashFunction)
    {
        switch (hashFunction)
        {
            case HashFunction::Crc32:
            {
                CRC32 crc32;
                return crc32(data.data.data(), data.data.size());

                break;
            }
            case HashFunction::Md5:
            {
                MD5 md5;
                return md5(data.data.data(), data.data.size());

                break;
            }
            case HashFunction::Sha1:
            {
                SHA1 sha1;
                return sha1(data.data.data(), data.data.size());

                break;
            }
            case HashFunction::Sha256:
            {
                SHA256 sha256;
                return sha256(data.data.data(), data.data.size());

                break;
            }
            default:
            {
                return "";
            }
        }
    }

    Data newData(sol::table data)
    {
        Data newData;

        for (int i = 0; i < (int)data.size(); i++)
        {
            newData.data.push_back(data[i + 1]);
        }

        return newData;
    }

    void printTable(sol::table table)
    {
        std::cout << "{\n";

        for (const auto& pair : table) {
            std::cout << "    ";

            if (pair.first.is<std::string>())
            {
                std::cout << pair.first.as<std::string>();
            }
            else
            {
                std::cout << "[" << pair.first.as<int>() << "]";
            }

            std::cout << " = ";
            sol::object value = pair.second;

            switch (value.get_type())
            {
                case sol::type::number:
                    std::cout << value.as<double>();
                    break;
                case sol::type::boolean:
                    std::cout << std::boolalpha << value.as<bool>();
                    break;
                case sol::type::string:
                    std::cout << "\"" << value.as<std::string>() << "\"";
                    break;
                case sol::type::table:
                    printTable(value.as<sol::table>());
                    break;
                default:
                    std::cout << "nil";
            }

            std::cout << ",\n";
        }

        std::cout << "}\n";
    }
}
