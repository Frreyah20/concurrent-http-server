#include "router.h"

std::string Router::route(const std::string& method,
                          const std::string& path)
{
    if(method == "GET")
    {
        auto it = get_routes.find(path);
        if(it != get_routes.end())
        {
            return it->second();
        }
    }

    if(method == "POST")
    {
        auto it = post_routes.find(path);
        if(it != post_routes.end())
        {
            return it->second();
        }
    }

    return "404 Not Found";
}

void Router::get(const std::string& path,
                 std::function<std::string()> handler)
{
    get_routes[path] = handler;
}

void Router::post(const std::string& path,
                  std::function<std::string()> handler)
{
    post_routes[path] = handler;
}

bool Router::hasRoute(const std::string& method,
                      std::string& path)
{
    if(method == "GET")
    {
        return get_routes.find(path) != get_routes.end();
    }

    if(method == "POST")
    {
        return post_routes.find(path) != post_routes.end();
    }

    return false;
}