#include "logger.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

Logger::Logger()
{
    log_file.open("server.log", std::ios::app);
}

std::string Logger::getTimestamp()
{
    auto now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

void Logger::info(const std::string& message)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    std::string log_message =
        "[" + getTimestamp() + "] [INFO] " + message;

    std::cout << log_message << '\n';
    log_file << log_message << '\n';
    log_file.flush();
}

void Logger::warning(const std::string& message)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    std::string log_message =
        "[" + getTimestamp() + "] [WARNING] " + message;

    std::cout << log_message << '\n';
    log_file << log_message << '\n';
    log_file.flush();
}

void Logger::error(const std::string& message)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    std::string log_message =
        "[" + getTimestamp() + "] [ERROR] " + message;

    std::cout << log_message << '\n';
    log_file << log_message << '\n';
    log_file.flush();
}