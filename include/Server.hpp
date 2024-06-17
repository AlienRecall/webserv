#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"
#include "Logger.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 10
#define MAX_CLIENTS 10

#define MAX_CLIENT_BODY_SIZE 1048576

class Server {
  private:
    int _fd;
    Logger logger;
    Config *_config;

  public:
    Server(Config *conf);
    ~Server();
    Server(const Server &);
    Server &operator=(const Server &);

    void close_fd();
    int get_fd() const;
    unsigned int get_client_body_size() const;
    Config *get_config();
    Error open_socket();
    void get_path_config(std::string, RouteConfig &);
};

#endif
