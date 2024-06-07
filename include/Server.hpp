#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"

#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_DELETE "DELETE"
#define METHOD_PUT "PUT"

class Server {
  private:
    const Config &_config;

  public:
    Server(const Config &conf);
    ~Server();
    Server(const Server &);
    Server &operator=(const Server &);

    void start_server();
};

#endif
