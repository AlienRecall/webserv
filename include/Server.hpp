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
    Config *get_config();
    Error open_socket();
    Error get_path_rules(std::string &, RouteConfig &){
        // trovo la path nelle locations?
        // si: scrive le rules su il config passato come paramentro

        // taglia la path
        // se trova un'altro path si richiama se stesso
        // fine
    };
};

#endif
