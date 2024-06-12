#ifndef EPOLLMANAGER_HPP
#define EPOLLMANAGER_HPP

#include "ConfigParser.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include <vector>

class ServersManager {
  private:
    std::vector<Server> _servers;
    std::map<int, int> _fds;

    typedef std::map<int, int>::iterator _fds_iterator;

  public:
    ServersManager(){};
    ~ServersManager() {
        for (iterator it = begin(); it != end(); it++)
            it->close_fd();
    };

    typedef std::vector<Server>::iterator iterator;
    iterator begin() { return _servers.begin(); }
    iterator end() { return _servers.end(); }

    void remove_connection(int client_fd) {
        close(client_fd);
        _fds.erase(client_fd);
    }
    void push_connection(int server_fd, int client_fd) { _fds[client_fd] = server_fd; }
    void push_server(Server &s) {
        _servers.push_back(s);
        _fds[s.get_fd()] = s.get_fd();
    };
    Server *get_server(int server_fd) {
        for (iterator it = begin(); it != end(); it++)
            if (it->get_fd() == server_fd)
                return &(*it);
        return 0;
    };
    int search(int fd, Server **ptr) {
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
};

#endif
