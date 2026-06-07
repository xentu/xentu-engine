#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>

#include "../Globals.h"
#include "DataManager.h"

namespace xen
{
    DataManager::DataManager()
    {
        // keep a pointer to this instance.
        if (instance != nullptr) {
            XEN_ERROR("Error, tried to create more than one AssetManager!");
            exit(111);
        }
        instance = this;
    }

    DataManager::~DataManager()
    {

        instance = nullptr;
    }


    DataManager* DataManager::instance = nullptr;
    DataManager* DataManager::GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new DataManager();
            //XEN_ERROR("Error, called DataManager::GetInstance() before initialization!");
            //exit(123);
        }
        return instance;
    }


    DataBuffer DataManager::ReadAllData(const std::string filename)
    {
        std::ifstream file(filename.c_str(), std::ios::binary);

        // Get length of file
        file.seekg(0, std::ios::end);
        size_t length = file.tellg();
        file.seekg(0, std::ios::beg);

        uint8_t* buffer = new uint8_t[length];
        //file.read(data2, 0);
        file.read (reinterpret_cast<char*>(buffer), (std::streamsize)length);

        return DataBuffer { buffer, length, length, false };
    }

    std::string DataManager::ReadAllText(const std::string filename)
    {
        XEN_DEBUG("> Loading text from file %s\n", filename.c_str());
        std::ifstream file(filename.c_str(), std::ios::binary);
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string res = buffer.str();
        file.close();
        return res;
    }
}