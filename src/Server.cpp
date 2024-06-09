#include "../include/Server.hpp"

Server::Server(Config &conf) : _config(conf) {}

Server::~Server() {}

Server::Server(const Server &s) : _config(s._config) { *this = s; }

Server &Server::operator=(const Server &s) {
    if (this == &s)
        return *this;
    _config = s._config;
    return *this;
}
