#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"

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
