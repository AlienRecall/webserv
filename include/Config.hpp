#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <string>

static std::string expected_methods[6] = {"GET", "POST",   "DELETE",
                                          "PUT", "OPTION", "HEAD"};

enum Methods {
    GET = 1,
    POST = 1 << 1,
    DELETE = 1 << 2,
    PUT = 1 << 3,
    OPTION = 1 << 4,
    HEAD = 1 << 5,
};

class RouteConfig {
  private:
    std::list<std::string> _methods;
    int _allowed_methods;
    unsigned int _client_body_size;
    std::string _redirect;
    std::string _root;
    std::string _index;
    bool _dir_listing;
    std::list<std::string> _cgi_extensions;
    std::string _upload_location;
    std::map<unsigned int, std::string> _error_pages;

    void append_method(const std::string &method);

  public:
    RouteConfig();
    virtual ~RouteConfig();
    RouteConfig(const RouteConfig &);
    RouteConfig &operator=(const RouteConfig &);

    typedef std::map<unsigned int, std::string>::iterator error_pages_iterator;
    error_pages_iterator begin() { return _error_pages.begin(); }
    error_pages_iterator end() { return _error_pages.end(); }

    void set_allowed_methods(std::string &);
    void set_client_body_size(unsigned int);
    void set_redirect(const std::string &);
    void set_root(const std::string &);
    void set_index(const std::string &);
    void set_dir_listing(const std::string &);
    void set_cgi_ext(std::string &);
    void set_upload_location(const std::string &);
    void set_error_page(unsigned int, const std::string &);

    int get_allowed_methods() const;
    unsigned int get_client_body_size() const;
    std::string get_redirect() const;
    std::string get_root() const;
    std::string get_index() const;
    bool get_dir_listing() const;
    std::list<std::string> get_cgi_ext() const;
    std::string get_upload_location() const;
    std::string get_error_page(unsigned int);
};

class Config : public RouteConfig {
  private:
    std::string _server_name;
    std::string _host;
    std::string _port;

    std::map<std::string, RouteConfig *> _routes;

  public:
    Config();
    ~Config();
    Config(const Config &);
    Config &operator=(const Config &);

    typedef std::map<std::string, RouteConfig *>::iterator routes_iterator;
    routes_iterator begin() { return _routes.begin(); }
    routes_iterator end() { return _routes.end(); }
    unsigned int size() { return _routes.size(); }

    void set_server_name(const std::string &);
    void set_host(const std::string &);
    void set_port(const std::string &);
    void set_location(const std::string &, RouteConfig *);

    std::string get_server_name() const;
    std::string get_host() const;
    std::string get_port() const;
    RouteConfig *get_location(const std::string &loc);
};

std::ostream &operator<<(std::ostream &, Config &);
std::ostream &operator<<(std::ostream &, RouteConfig &);

#endif
