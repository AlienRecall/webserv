#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"

enum Methods {
    GET = 1,
    POST = 1 << 1,
    DELETE = 1 << 2,
    PUT = 1 << 3,
    OPTION = 1 << 4,
    HEAD = 1 << 5,
};

class Server {
  private:
    Config &_config;

    Server(const Server &);
    Server &operator=(const Server &);

  public:
    Server(Config &conf);
    ~Server();

    void start_server();
};

#endif
