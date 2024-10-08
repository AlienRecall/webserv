#include "include/webserv.hpp"

std::map<const std::string, std::string> Pages::cache;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    Pages::init();
    Error err;
    t_webserv w;
    if (argc < 2 || argc > 3)
        return w.logger.error("Expected one argument.\n\tUsage: ./webserv [config.conf]");

    std::string config_file = argv[1];
    if (argc == 3 && std::strncmp(argv[2], "-d", 2) == 0)
        w.logger.set_debug(true);

    {
        w.logger.debug("parsing `" + config_file + "`");
        ConfigParser parser(config_file);
        err = parser.load_config();
        if (err != OK) {
            std::cout << "error code: " << err << std::endl;
            return 1;
        }

        w.logger.debug("loaded " + uitoa(parser.size()) + " servers.");
        w.logger.debug("setting up servers...");
        for (ConfigParser::iterator it = parser.begin(); it != parser.end(); it++) {
            Server s = Server(new Config(*it));
            s.open_socket();
            w.sm.push_server(s);

            Config *config = s.get_config();

            for (Config::error_pages_iterator it_err = config->error_pages_begin();
                 it_err != config->error_pages_end(); it_err++) {
                if (access(("./html/" + it_err->second).c_str(), F_OK) != -1)
                    Pages::push("./html/" + it_err->second,
                                uitoa(it_err->first) + "_custom");
            }
        }
    }
    // std::cout << "Contents of Pages::cache:" << std::endl;
    // std::map<const std::string, std::string>::iterator it;
    // for (it = Pages::cache.begin(); it != Pages::cache.end(); ++it)
    // {
    //     std::cout << "Key: " << it->first << std::endl;
    // }

    w.logger.debug("initializing webserv.");
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
        return w.logger.error("epoll_create error");
    set_fd_non_blocking(epoll_fd);

    for (ServersManager::iterator it = w.sm.begin(); it != w.sm.end(); it++) {
        std::cout << "server fd: " << (*it).get_fd() << std::endl;
        std::cout << *(*it).get_config() << std::endl;
        add_epoll(epoll_fd, (*it).get_fd());
    }

    w.logger.print_ts("ready.");
    int fd;
    struct epoll_event events[MAX_EVENTS];
    while (true) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1)
            return w.logger.error("epoll_wait error");

        for (int i = 0; i < num_events; i++) {
            fd = events[i].data.fd;
            w.logger.debug("received event on fd: " + uitoa(fd));
            int is_server = w.sm.search(events[i].data.fd, &w._ptr);
            if (is_server == 1) {
                struct sockaddr in_addr;
                socklen_t in_addr_len = sizeof(in_addr);
                int client_fd = accept(fd, &in_addr, &in_addr_len);
                if (client_fd == -1) {
                    return 1;
                }

                w.logger.print_ts("accepted new connection on fd: " + uitoa(client_fd));
                w.sm.push_connection(fd, client_fd);
                set_fd_non_blocking(client_fd);
                if (add_epoll(epoll_fd, client_fd))
                    return w.logger.error("webserv: epoll_ctl error");

                err = handle_client(client_fd, &w);
                if (err != OK)
                    w.logger.log_error(err);
                w.sm.remove_connection(client_fd);
            } else if (is_server == -1) {
                return w.logger.error("unexpected error: fd not recognized.");
            } else {
                err = handle_client(fd, &w);
                if (err != OK)
                    w.logger.log_error(err);
                w.sm.remove_connection(fd);
            }
        }
    }
    close(epoll_fd);
}
