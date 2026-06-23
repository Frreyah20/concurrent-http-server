#pragma once

#include <string>
#include <fstream>
#include <mutex>

class Logger
{
private:
    std::ofstream log_file;
    std::mutex log_mutex;

    std::string getTimestamp();

public:
    Logger();

    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
};