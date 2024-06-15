#include "../include/webserv.hpp"

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
    Request req;
    int err, bytesRead;
    char buffer[BUFFER_SIZE + 1];

    while ((bytesRead = read(client_fd, buffer, BUFFER_SIZE)) == -1) {
        continue;
    }
    if (bytesRead == 0) {
        w->sm.remove_connection(client_fd);
        return CUSTOM;
    }
    buffer[bytesRead] = 0;
    std::cout << buffer << std::endl;
    err = req.popRequest(buffer, client_fd);
    if (err != OK) {
        close(client_fd);
        return err;
    }
    // Response res = make_response(req, w->_ptr);
    // int bytesWrite = write(client_fd, res.str(), res.lenght());
    // if (bytesRead == 0) {
    //     close(client_fd);
    //     return CUSTOM;
    // }
    close(client_fd);
    return OK;
}
