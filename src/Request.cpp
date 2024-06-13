#include "../include/Request.hpp"
#include <cctype>

int Request::getMethod(std::stringstream &buffer) {
    std::string buff;
    buffer >> buff;
    switch (buff.length()) {
    case 3:
        if (buff[0] == 'G' && buff[1] == 'E' && buff[2] == 'T')
            method = GET;
        else if (buff[0] == 'P' && buff[1] == 'U' && buff[2] == 'T')
            method = PUT;
        break;
    case 6:
        if (buff[0] == 'D' && buff[1] == 'E' && buff[2] == 'L' && buff[3] == 'E' &&
            buff[4] == 'T' && buff[5] == 'E')
            method = DELETE;
        break;
    case 4:
        if (buff[0] == 'P' && buff[1] == 'O' && buff[2] == 'S' && buff[3] == 'T')
            method = POST;
        break;
    default:
        return ERROR_GETTING_METHOD;
    }
    return (OK);
}

int Request::getPath(std::stringstream &buffer) {
    std::string buff;
    buffer >> buff;
    if (buff[0] != '/')
        return ERROR_GETTING_PATH;
    path = buff;
    return OK;
}

int Request::getProtocol(std::stringstream &buffer) {
    std::string buff;
    buffer >> buff;
    if (buff.size() < 5 || buff[0] != 'H' || buff[1] != 'T' || buff[2] != 'T' ||
        buff[3] != 'P' || buff[4] != '/')
        return ERROR_GETTING_PROTOCOL;
    return OK;
}

int Request::getHeaders(std::stringstream &buffer) {
    std::string buff;
    std::string key;
    std::string value;
    size_t i = 0;
    int r = 0;
    while (std::getline(buffer, buff)) {
        if ((buff.empty() || buff[i] == '\r') && r == 1)
            break;
        if (buff[i] == '\r' && r == 0) {
            r = 1;
            continue;
        }
        for (; i < buff.length(); i++) {
            if (buff[i] == ':' && buff[i + 1] == ' ') {
                break;
            }
            key.push_back(buff[i]);
        }
        buff.erase(0, i + 2);
        for (i = 0; i < buff.length(); i++) {
            value.push_back(buff[i]);
        }
        headers[key] = value;
        key.clear();
        value.clear();
        i = 0;
    }
    if (headers.empty())
        return ERROR_GETTING_HEADERS;
    return OK;
}

int Request::getBody(std::stringstream &buffer, int client_fd) {
    if (method == GET)
        return OK;
    std::map<std::string, std::string>::iterator it = headers.find("content-length");
    if (it == headers.end()) {
        it = headers.find("Content-Length");
        if (it == headers.end())
            return (ERROR_GETTING_BODY);
    }
    int i = std::atoi(it->second.c_str());
    char buff[i];
    buffer >> this->body;
    if (body.length() == static_cast<size_t>(i))
        return OK;
    int r = read(client_fd, buff, i - static_cast<int>(body.length()));
    if (r != -1) {
        buff[r] = 0;
        body.append(buff);
    }
    return OK;
}

int Request::popRequest(char *buf, int client_fd) {
    std::string buffer = buf;
    std::stringstream newBuffer(buffer);
    if (getMethod(newBuffer) == ERROR_GETTING_METHOD)
        return (ERROR_GETTING_METHOD);
    if (getPath(newBuffer) == ERROR_GETTING_PATH)
        return (ERROR_GETTING_PATH);
    if (getProtocol(newBuffer) == ERROR_GETTING_PROTOCOL)
        return (ERROR_GETTING_PROTOCOL);
    if (getHeaders(newBuffer) == ERROR_GETTING_HEADERS)
        return (ERROR_GETTING_HEADERS);
    if (getBody(newBuffer, client_fd) == ERROR_GETTING_BODY)
        return (ERROR_GETTING_BODY);
    return OK;
}

Request::Request() {
    method = 0;
    path = "";
    body = "";
}

Request::~Request() {}
