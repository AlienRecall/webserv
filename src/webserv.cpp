#include "../include/webserv.hpp"
#include <unistd.h>

void set_fd_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1)
        exit(EXIT_FAILURE);
    flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1)
        exit(EXIT_FAILURE);
}

std::string uitoa(unsigned int v) {
    std::stringstream s;
    s << v;
    return s.str();
}

int add_epoll(int epoll_fd, int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        return 1;
    }
    return OK;
}

int handle_client(int client_fd, t_webserv *w) {
    int bytesRead;
    char buffer[BUFFER_SIZE + 1];

    while ((bytesRead = read(client_fd, buffer, sizeof(buffer))) == -1) {
        continue;
    }
    if (bytesRead == 0) {
        w->sm.remove_connection(client_fd);
        return CUSTOM;
    }
    buffer[bytesRead] = 0;
    std::cout << buffer << std::endl;
    close(client_fd);
    return OK;
}
