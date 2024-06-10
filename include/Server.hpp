#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"

class Server {
  private:
    Config &_config;

  public:
    Server(Config &conf);
    ~Server();
    Server(const Server &);
    Server &operator=(const Server &);

    void start_server();
};

#endif
