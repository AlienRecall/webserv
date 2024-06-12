#include "../include/Server.hpp"

Server::Server(Config &conf) : _fd(0), logger(Logger("")), _config(conf) {
    std::string t = _config.get_server_name();
    if (t.empty())
        t = _config.get_host() + ":" + _config.get_port();
    logger.set_title("Server " + t);
}

Server::~Server() {}

Server::Server(const Server &s) : logger(Logger("")), _config(s._config) { *this = s; }

Server &Server::operator=(const Server &s) {
    if (this == &s)
        return *this;
    _fd = s._fd;
    logger = s.logger;
    _config = s._config;
    return *this;
}

void Server::close_fd() { close(_fd); }

int Server::get_fd() const { return _fd; }

Config &Server::get_config() const { return _config; }

Error Server::open_socket() {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        return logger.error("error opening port: " + _config.get_port());
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(std::atoi(_config.get_port().c_str()));
    if (bind(_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        logger.error("bind error");
        close(_fd);
        return CUSTOM;
    }

    if (listen(_fd, MAX_CLIENTS) == -1) {
        logger.error("listen error");
        close(_fd);
        return CUSTOM;
    }
    return OK;
}
