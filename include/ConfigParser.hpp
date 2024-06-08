#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Config.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <vector>

std::string expected_keys[12] = {
    "server_name", "host",  "port",           "client_body_size",
    "error_page",  "index", "allow_methods",  "redirect",
    "autoindex",   "root",  "cgi_extensions", "location"};

class ConfigParser {
  private:
    Logger logger;
    std::string _config_file;
    unsigned int _key_type;
    std::string _key, _value;
    std::vector<Config> _configs;

    ConfigParser();

    Error take_key(std::string);
    Error take_value(std::string);
    Error detect_field(std::string);

    Error parse_location(std::ifstream &);
    Error parse_server(std::ifstream &);

    Error populate_config(Config &);

  public:
    typedef std::vector<Config>::iterator iterator;
    iterator begin() { return _configs.begin(); }
    iterator end() { return _configs.end(); }

    ConfigParser(const std::string &);
    ConfigParser(const ConfigParser &);
    ~ConfigParser();
    ConfigParser &operator=(const ConfigParser &);

    Error load_config();
};

#endif
