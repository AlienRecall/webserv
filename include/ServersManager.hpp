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
    ServersManager();
    ~ServersManager();

    typedef std::vector<Server>::iterator iterator;
    iterator begin();
    iterator end();

    void remove_connection(int client_fd);
    void push_connection(int server_fd, int client_fd);
    void push_server(const Server &s);
    Server *get_server(int server_fd);
    int search(int fd, Server **ptr);
};

#endif
