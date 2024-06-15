#include "../include/webserv.hpp"

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
