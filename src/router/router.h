#pragma once

#include <string>
#include <unordered_map>
#include <functional>

class Router
{
private:
    std::unordered_map<std::string, std::function<std::string()>> get_routes;
    std::unordered_map<std::string, std::function<std::string()>> post_routes;

public:
    std::string route(const std::string& method,
                      const std::string& path);

    void get(const std::string& path,
             std::function<std::string()> handler);

    void post(const std::string& path,
              std::function<std::string()> handler);

    bool hasRoute(const std::string& method,
                  std::string& path);
};