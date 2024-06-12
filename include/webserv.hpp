#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Logger.hpp"
#include "Server.hpp"
#include "ServersManager.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <sstream>
#include <sys/epoll.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct s_webserv {
    Logger logger;
    ServersManager sm;
    Server *_ptr;

    s_webserv() : logger(Logger("webserv")) {}
} t_webserv;

std::string uitoa(unsigned int v);
void set_fd_non_blocking(int);

int add_epoll(int epoll_fd, int fd);

int handle_client(int client_fd, t_webserv *server);

#endif
