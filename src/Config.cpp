#include "../include/Config.hpp"
#include <string>

Config::Config() : RouteConfig() {}

Config::~Config() {}

Config::Config(const Config &c) : RouteConfig(c) { *this = c; }

Config &Config::operator=(const Config &c) {
    if (this == &c)
        return *this;
    _server_name = c._server_name;
    _host = c._host;
    _port = c._port;
    _routes = c._routes;
    return *this;
}

void Config::set_server_name(const std::string &v) { _server_name = v; }

void Config::set_host(const std::string &v) { _host = v; }

void Config::set_port(const std::string &v) { _port = v; }

void Config::set_location(const std::string &loc, RouteConfig *conf) {
    _routes[loc] = conf;
}

std::string Config::get_server_name() const { return _server_name; }

std::string Config::get_host() const { return _host; }

std::string Config::get_port() const { return _port; }

RouteConfig *Config::get_location(const std::string &loc) {
    routes_iterator it = _routes.find(loc);
    if (it != _routes.end())
        return it->second;
    return 0;
}

std::ostream &operator<<(std::ostream &os, Config &conf) {
    os << "server_name: " << conf.get_server_name() << std::endl;
    os << "host: " << conf.get_host() << std::endl;
    os << "port: " << conf.get_port() << std::endl;
    int am = conf.get_allowed_methods();
    if (am != 0) {
        os << "allowed methods: ";
        for (int i = 0; i < 6; i++) {
            if ((am & 1 << i) != 0) {
                os << expected_methods[i] << " ";
            }
        }
        os << std::endl;
    }
    if (conf.get_client_body_size() != 0)
        os << "client body size: " << conf.get_client_body_size() << std::endl;
    if (conf.get_redirect() != "")
        os << "redirect: " << conf.get_redirect() << std::endl;
    if (conf.get_root() != "")
        os << "root: " << conf.get_root() << std::endl;
    if (conf.get_index() != "")
        os << "index: " << conf.get_index() << std::endl;
    os << "is directory listing set: " << conf.get_dir_listing() << std::endl;
    std::list<std::string> l = conf.get_cgi_ext();
    if (!l.empty()) {
        os << "cgi extensions: ";
        for (std::list<std::string>::iterator it = l.begin(); it != l.end();
             it++) {
            os << *it << " ";
        }
        os << std::endl;
    }
    if (conf.get_upload_location() != "")
        os << "upload location: " << conf.get_upload_location() << std::endl;
    for (RouteConfig::error_pages_iterator it = conf.error_pages_begin();
         it != conf.error_pages_end(); it++) {
        os << "error page for status: " << it->first << " is: " << it->second
           << std::endl;
    }
    for (Config::routes_iterator it = conf.routes_begin();
         it != conf.routes_end(); it++) {
        os << "location: " << it->first << std::endl;
        os << *(it->second) << std::endl;
    }
    os << std::endl << std::endl;
    return os;
}
