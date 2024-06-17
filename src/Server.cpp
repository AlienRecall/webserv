#include "../include/Server.hpp"
#include <string>

Server::Server(Config *conf) : _fd(0), logger(Logger("")), _config(conf) {
    std::string t = _config->get_server_name();
    if (t.empty())
        t = _config->get_host() + ":" + _config->get_port();
    logger.set_title("Server " + t);
}

Server::~Server() { delete _config; }

Server::Server(const Server &s) : logger(Logger("")), _config(s._config) { *this = s; }

Server &Server::operator=(const Server &s) {
    if (this == &s)
        return *this;
    _fd = s._fd;
    logger = s.logger;
    _config = new Config(*s._config);
    return *this;
}

void Server::close_fd() { close(_fd); }

int Server::get_fd() const { return _fd; }

Config *Server::get_config() { return _config; }

Error Server::open_socket() {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        return logger.error("error opening port: " + _config->get_port());
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(std::atoi(_config->get_port().c_str()));
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

unsigned int Server::get_client_body_size() const {
    return _config->get_client_body_size() == 0 ? MAX_CLIENT_BODY_SIZE
                                                : _config->get_client_body_size();
}

void Server::get_path_config(std::string path, RouteConfig &config) {
    RouteConfig *ptr = _config->get_location(path);
    if (ptr) {
        if (config.get_allowed_methods() == 0 && ptr->get_allowed_methods() != 0)
            config.must_set_allowed_method(ptr->get_allowed_methods());
        if (config.get_redirect() == "" && ptr->get_redirect() != "")
            config.set_redirect(ptr->get_redirect());
        if (config.get_index() == "" && ptr->get_index() != "")
            config.set_index(ptr->get_index());
        if (config.get_dir_listing() == -1 && ptr->get_dir_listing() != -1)
            config.set_dir_listing(ptr->get_dir_listing() ? "on" : "off");
        if (ptr->cgi_ext_size() > 0) {
            for (RouteConfig::cgi_ext_iterator it = ptr->cgi_ext_begin();
                 it != ptr->cgi_ext_end(); it++)
                config.set_cgi_ext(*it);
        }
    }

    size_t pos = path.find_last_of("/");
    if (pos == 0)
        return;
    return get_path_config(path.substr(0, pos), config);
}
