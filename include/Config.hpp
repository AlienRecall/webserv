#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Route.hpp"
#include <iostream>
#include <map>
#include <string>

class Config {
  private:
    std::string _server_name;
    std::string _host;
    std::string _port;
    unsigned int _client_body_size;
    std::string _root;
    std::string _index;

    std::map<unsigned int, std::string> _error_pages;
    std::map<std::string, Route> _routes;

  public:
    Config();
    ~Config();
    Config(const Config &);
    Config &operator=(const Config &);

    void set_server_name(const std::string &);
    void set_host(const std::string &);
    void set_port(const std::string &);
    void set_client_body_size(unsigned int);
    void set_root(const std::string &);
    void set_index(const std::string &);
    void set_error_page(unsigned int, const std::string &);
    void set_location(const std::string &);
};

#endif
