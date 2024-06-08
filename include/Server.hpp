#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"

enum Methods {
    GET,
    POST,
    DELETE,
    PUT,
    OPTION,
    HEAD,
};

class Server {
  private:
    Config &_config;

  public:
    Server(const Config &conf);
    ~Server();
    Server(const Server &);
    Server &operator=(const Server &);

    void start_server();
};

#endif
