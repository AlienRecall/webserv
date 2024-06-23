#include "../include/webserv.hpp"
#include <iostream>

int add_epoll(int epoll_fd, int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        return 1;
    }
    return OK;
}

Error handle_client(int client_fd, t_webserv *w) {
    Error err;
    Request req;
    Response res; // Istanza della classe Response per generare la risposta HTTP
    int bytesRead;
    char buffer[BUFFER_SIZE + 1];

    while ((bytesRead = read(client_fd, buffer, BUFFER_SIZE)) == -1)
        continue;
    if (bytesRead == 0) {
        close(client_fd);
        return CUSTOM;
    }
    buffer[bytesRead] = 0;
    std::cout << buffer << std::endl;

    err = req.popRequest(buffer, client_fd, w->_ptr->get_client_body_size());
    if (err != OK) {
        close(client_fd);
        return err;
    }
    std::cout << "received req on uri: " << req.get_path() << std::endl;

    res.prepare_response(req, w->_ptr); // Prepara la risposta in base alla richiesta
    char *response_str = res.c_str();   // Converte la risposta in una stringa C
    if (!response_str) {
        std::cout << "close client (no response)" << std::endl;
        close(client_fd);
        return CUSTOM;
    }

    std::cout << "response code: " << res.get_status_code()
              << " response len: " << res.length() << std::endl;
    write(client_fd, response_str, res.length()); // Scrive la risposta al client
    close(client_fd);
    std::cout << "sent response to client" << std::endl;
    delete[] response_str; // Dealloca la memoria allocata per la stringa di risposta
    return OK;
}
