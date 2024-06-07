#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <map>

class Config {
  private:
    std::string _host;
    std::string _port;
    std::string _name;
    unsigned int _client_body_size;

    std::map<std::string, struct route> _routes;

  public:
    Config();
    ~Config();
    Config(const Config &);
    Config &operator=(const Config &);
};

#endif
