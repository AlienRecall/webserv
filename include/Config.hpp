#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <string>

#define BUFFER_SIZE 1024

static std::string expected_methods[4] = {"GET", "POST", "DELETE", "PUT"};

enum Methods {
    GET = 1,
    POST = 1 << 1,
    DELETE = 1 << 2,
    PUT = 1 << 3,
};

class RouteConfig {
  protected:
    int _allowed_methods;
    std::string _redirect;
    std::string _root;
    std::string _index;
    int _dir_listing;
    std::string _upload_location;
    std::list<std::string> _cgi_extensions;

    void append_method(const std::string &method);

  public:
    RouteConfig();
    virtual ~RouteConfig();
    RouteConfig(const RouteConfig &);
    RouteConfig &operator=(const RouteConfig &);

    typedef std::list<std::string>::iterator cgi_ext_iterator;
    cgi_ext_iterator cgi_ext_begin() { return _cgi_extensions.begin(); }
    cgi_ext_iterator cgi_ext_end() { return _cgi_extensions.end(); }
    unsigned int cgi_ext_size() { return _cgi_extensions.size(); }

    void must_set_allowed_method(unsigned int);
    void set_allowed_methods(std::string &);
    void set_redirect(const std::string &);
    void set_root(const std::string &);
    void set_index(const std::string &);
    void set_dir_listing(const std::string &);
    void set_cgi_ext(std::string &);
    void set_upload_location(const std::string &);

    int get_allowed_methods() const;
    std::string get_redirect() const;
    std::string get_root() const;
    std::string get_index() const;
    int get_dir_listing() const;
    std::string get_upload_location() const;
};

class Config : public RouteConfig {
  private:
    std::string _server_name;
    std::string _host;
    std::string _port;
    unsigned int _client_body_size;

    std::map<std::string, RouteConfig *> _routes;
    std::map<unsigned int, std::string> _error_pages;

  public:
    Config();
    ~Config();
    Config(const Config &);
    Config &operator=(const Config &);

    typedef std::map<std::string, RouteConfig *>::const_iterator const_routes_iterator;
    typedef std::map<std::string, RouteConfig *>::iterator routes_iterator;
    routes_iterator routes_begin() { return _routes.begin(); }
    routes_iterator routes_end() { return _routes.end(); }
    unsigned int routes_size() { return _routes.size(); }

    typedef std::map<unsigned int, std::string>::iterator error_pages_iterator;
    error_pages_iterator error_pages_begin() { return _error_pages.begin(); }
    error_pages_iterator error_pages_end() { return _error_pages.end(); }

    void set_server_name(const std::string &);
    void set_host(const std::string &);
    void set_port(const std::string &);
    void set_client_body_size(unsigned int);
    void set_location(const std::string &, RouteConfig *);
    void set_error_page(const std::string &, const std::string &);
    void set_error_pages(std::string &);

    std::string get_server_name() const;
    std::string get_host() const;
    std::string get_port() const;
    unsigned int get_client_body_size() const;
    std::string get_error_page(unsigned int);
    RouteConfig *get_location(const std::string &loc);

    void clear_routes();
};

std::ostream &operator<<(std::ostream &, Config &);
std::ostream &operator<<(std::ostream &, RouteConfig &);

#endif
