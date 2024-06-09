#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include <map>
#include <string>

class RouteConfig {
  private:
    std::list<std::string> _methods;
    int _allowed_method;
    unsigned int _client_body_size;
    std::string _redirect;
    std::string _root;
    std::string _index;
    bool _dir_listing;
    std::list<std::string> _cgi_extensions;
    std::string _upload_location;
    std::map<unsigned int, std::string> _error_pages;

  public:
    RouteConfig();
    virtual ~RouteConfig();
    RouteConfig(const RouteConfig &);
    RouteConfig &operator=(const RouteConfig &);

    void set_allowed_methods(const std::string &);
    void set_client_body_size(unsigned int);
    void set_redirect(const std::string &);
    void set_root(const std::string &);
    void set_index(const std::string &);
    void set_dir_listing(const std::string &);
    void set_cgi_ext(const std::string &);
    void set_upload_location(const std::string &);
    void set_error_page(unsigned int, const std::string &);
};

class Config : public RouteConfig {
  private:
    std::string _server_name;
    std::string _host;
    std::string _port;

    std::map<std::string, RouteConfig> _routes;

  public:
    Config();
    ~Config();
    Config(const Config &);
    Config &operator=(const Config &);

    void set_server_name(const std::string &);
    void set_host(const std::string &);
    void set_port(const std::string &);
    void set_location(const std::string &, RouteConfig &);
};

#endif
