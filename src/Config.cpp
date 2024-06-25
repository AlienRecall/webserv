#include "../include/Config.hpp"
#include <string>

Config::Config() : RouteConfig() {
    _server_name = "";
    _host = "";
    _port = "";
    _client_body_size = 0;
}

void Config::clear_routes() {
    for (routes_iterator it = _routes.begin(); it != _routes.end(); it++) {
        delete it->second;
    }
}

Config::~Config() { clear_routes(); }

Config::Config(const Config &c) : RouteConfig(c) { *this = c; }

Config &Config::operator=(const Config &c) {
    if (this == &c)
        return *this;
    _server_name = c._server_name;
    _host = c._host;
    _port = c._port;
    _allowed_methods = c._allowed_methods;
    _client_body_size = c._client_body_size;
    _redirect = c._redirect;
    _root = c._root;
    _index = c._index;
    _dir_listing = c._dir_listing;
    _cgi_extensions = c._cgi_extensions;
    _error_pages = c._error_pages;
    clear_routes();
    for (const_routes_iterator it = c._routes.begin(); it != c._routes.end(); it++) {
        _routes[it->first] = new RouteConfig(*(it->second));
    }
    return *this;
}

bool Config::error_pages_empty() const
{
    return _error_pages.empty();
}

void Config::set_server_name(const std::string &v) { _server_name = v; }

void Config::set_host(const std::string &v) { _host = v; }

void Config::set_port(const std::string &v) { _port = v; }

void Config::set_client_body_size(unsigned int v) { _client_body_size = v; }

void Config::set_location(const std::string &loc, RouteConfig *conf) {
    _routes[loc] = conf;
}

void Config::set_error_page(const std::string &s, const std::string &p) {
    unsigned int status = std::atoi(s.c_str());
    if (status == 0)
        return;
    _error_pages[status] = p;
}

void Config::set_error_pages(std::string &error_pages) {
        std::cout << "xono dentro ciclo di set error_pages" << std::endl;
    size_t pos = error_pages.find_last_of(' ');
    if (pos == std::string::npos)
        return;
    std::string page = error_pages.substr(pos + 1);
    error_pages.erase(pos);
    std::string token;
    while ((pos = error_pages.find_first_of("	 ")) != std::string::npos) {
        token = error_pages.substr(0, pos);
        set_error_page(token, page);
        error_pages.erase(0, pos + 1);
    }
    if (!error_pages.empty()) {
        set_error_page(error_pages, page);
    }
}

std::string Config::get_error_page(unsigned int status) {
    error_pages_iterator it = _error_pages.find(status);
    if (it != _error_pages.end())
        return it->second;
    return "";
}

std::string Config::get_server_name() const { return _server_name; }

std::string Config::get_host() const { return _host; }

std::string Config::get_port() const { return _port; }

unsigned int Config::get_client_body_size() const { return _client_body_size; }

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
    if (conf.cgi_ext_size() > 0) {
        os << "cgi extensions: ";
        for (RouteConfig::cgi_ext_iterator it = conf.cgi_ext_begin();
             it != conf.cgi_ext_begin(); it++) {
            os << *it << " ";
        }
        os << std::endl;
    }
    if (conf.get_upload_location() != "")
        os << "upload location: " << conf.get_upload_location() << std::endl;
    for (Config::error_pages_iterator it = conf.error_pages_begin();
         it != conf.error_pages_end(); it++) {
        os << "error page for status `" << it->first << "` is: " << it->second
           << std::endl;
    }
    for (Config::routes_iterator it = conf.routes_begin(); it != conf.routes_end();
         it++) {
        os << "location: " << it->first << std::endl;
        os << *(it->second) << std::endl;
    }
    os << std::endl << std::endl;
    return os;
}
