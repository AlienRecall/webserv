#include "../include/ServersManager.hpp"

ServersManager::ServersManager(){};

ServersManager::~ServersManager() {
    for (iterator it = begin(); it != end(); it++) {
        (*it).close_fd();
    }
};

ServersManager::iterator ServersManager::begin() { return _servers.begin(); }

ServersManager::iterator ServersManager::end() { return _servers.end(); }

void ServersManager::remove_connection(int client_fd) {
    close(client_fd);
    _fds.erase(client_fd);
}

void ServersManager::push_connection(int server_fd, int client_fd) {
    _fds[client_fd] = server_fd;
}

void ServersManager::push_server(const Server &s) {
    _servers.push_back(s);
    _fds[s.get_fd()] = s.get_fd();
}

Server *ServersManager::get_server(int server_fd) {
    for (iterator it = begin(); it != end(); it++)
        if ((*it).get_fd() == server_fd)
            return &(*it);
    return 0;
}

int ServersManager::search(int fd, Server **ptr) {
    *ptr = get_server(fd);
    if (*ptr != 0)
        return 1;

    _fds_iterator it = _fds.find(fd);
    if (it == _fds.end()) {
        return -1;
    }
    *ptr = get_server(it->second);
    return 0;
}
