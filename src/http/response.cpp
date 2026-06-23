#include "response.h"

#include <sys/socket.h>
#include <string>

std::string getMimeType(const std::string& path)
{
    if (path.find(".html") != std::string::npos)
        return "text/html";

    if (path.find(".css") != std::string::npos)
        return "text/css";

    if (path.find(".js") != std::string::npos)
        return "application/javascript";

    if (path.find(".png") != std::string::npos)
        return "image/png";

    return "text/plain";
}

bool sendRequest(int fd, const std::string& response)
{
    ssize_t sent = send(fd, response.c_str(), response.size(), 0);
    return sent >= 0;
}