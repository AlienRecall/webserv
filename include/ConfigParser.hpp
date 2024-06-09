#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Config.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#define LOCATION_KEY 12
static std::string expected_keys[13] = {
    "server_name",      "host",  "port",           "error_page",
    "client_body_size", "index", "allow_methods",  "redirect",
    "autoindex",        "root",  "cgi_extensions", "upload_location",
    "location"};

class ConfigParser {
  private:
    Logger logger;
    std::string _config_file;
    unsigned int _key_type;
    std::string _key, _value;
    std::vector<Config> _configs;
    Config *_actual;

    ConfigParser();
    ConfigParser(const ConfigParser &);

    Error take_key(std::string);
    Error take_value(std::string);
    Error detect_field(std::string);

    Error parse_segment(std::ifstream &, RouteConfig *, std::string = "");
    Error parse_location(std::ifstream &);
    Error parse_server(std::ifstream &);

    Error populate_route_config(RouteConfig &);
    Error populate_config(Config &);

  public:
    typedef std::vector<Config>::iterator iterator;
    iterator begin() { return _configs.begin(); }
    iterator end() { return _configs.end(); }

    ConfigParser(const std::string &);
    ~ConfigParser();
    ConfigParser &operator=(const ConfigParser &);

    Error load_config();
};

#endif
