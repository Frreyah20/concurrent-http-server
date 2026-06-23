#include "config.h"

#include <fstream>
#include <string>

Config config;

bool loadConfig(const std::string& filename)
{
    std::ifstream file(filename);

    if(!file.is_open())
    {
        return false;
    }

    std::string line;

    while(std::getline(file, line))
    {
        size_t pos = line.find('=');

        if(pos == std::string::npos)
        {
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if(key == "port")
        {
            config.port = std::stoi(value);
        }
        else if(key == "workers")
        {
            config.workers = std::stoi(value);
        }
        else if(key == "root")
        {
            config.root = value;
        }
    }

    return true;
}