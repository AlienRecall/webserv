#include "include/ConfigParser.hpp"
#include "include/Logger.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Expected one argument.\n\tUsage: ./webserv [config.conf]"
                  << std::endl;
        return 1;
    }

    ConfigParser parser(argv[1]);
    Error err = parser.load_config();
    std::cout << "error: " << err << std::endl;

    for (ConfigParser::iterator it = parser.begin(); it != parser.end(); it++)
        std::cout << *it << std::endl;
}
