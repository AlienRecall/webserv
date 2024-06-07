#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Server.hpp"

class ConfigParser {
  private:
    std::string _config_file;
    std::vector<Server> _servers;

    ConfigParser();

    void open_file();    // first step
    void parse_server(); // second step
  public:
    ConfigParser(const std::string &);
    ConfigParser(const ConfigParser &);
    ~ConfigParser();
    ConfigParser &operator=(const ConfigParser &);

    void load_config(); // called from main
};

#endif
