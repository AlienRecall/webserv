#include "include/ConfigParser.hpp"
#include "include/Logger.hpp"
#include "include/Server.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Expected one argument.\n\tUsage: ./webserv [config.conf]"
                  << std::endl;
        return 1;
    }

    ConfigParser parser(argv[1]);
    Error err = parser.load_config();
    if (err != OK) {
        std::cout << "error code: " << err << std::endl;
        return 1;
    }

    for (ConfigParser::iterator it = parser.begin(); it != parser.end(); it++) {
        std::cout << *it << std::endl;
        Server s(*it);
        s.start_server();
    }
}
