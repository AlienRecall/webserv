#include "include/ConfigParser.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Expected one argument.\n\tUsage: ./webserv [config.conf]"
                  << std::endl;
        return 1;
    }

    std::string f = argv[1];
    ConfigParser parser(f);
}
