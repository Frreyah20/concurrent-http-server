#pragma once

#include <string>

std::string getMimeType(const std::string& path);

bool sendRequest(int fd, const std::string& response);