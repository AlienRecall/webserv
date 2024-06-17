#include "../include/Config.hpp"

RouteConfig::RouteConfig() {
    _allowed_methods = 0;
    _redirect = "";
    _root = "";
    _index = "";
    _dir_listing = -1;
    _upload_location = "";
}

RouteConfig::~RouteConfig() {}

RouteConfig::RouteConfig(const RouteConfig &rc) { *this = rc; }

RouteConfig &RouteConfig::operator=(const RouteConfig &rc) {
    if (this == &rc)
        return *this;
    _allowed_methods = rc._allowed_methods;
    _redirect = rc._redirect;
    _root = rc._root;
    _index = rc._index;
    _dir_listing = rc._dir_listing;
    _upload_location = rc._upload_location;
    _cgi_extensions = rc._cgi_extensions;
    return *this;
}

void RouteConfig::append_method(const std::string &method) {
    for (int i = 0; i < 4; i++) {
        if (method == expected_methods[i]) {
            _allowed_methods |= 1 << i;
            break;
        }
    }
}

void RouteConfig::set_allowed_methods(std::string &methods) {
    _allowed_methods = 0;

    size_t pos = 0;
    std::string token;
    while ((pos = methods.find_first_of("	 ")) != std::string::npos) {
        token = methods.substr(0, pos);
        append_method(token);
        methods.erase(0, pos + 1);
    }
    if (!methods.empty()) {
        append_method(methods);
    }
}

void RouteConfig::must_set_allowed_method(unsigned int method) {
    _allowed_methods = method;
}

void RouteConfig::set_redirect(const std::string &v) { _redirect = v; }

void RouteConfig::set_root(const std::string &v) { _root = v; }

void RouteConfig::set_index(const std::string &v) { _index = v; }

void RouteConfig::set_dir_listing(const std::string &v) {
    if (v == "on")
        _dir_listing = 1;
    else
        _dir_listing = 0;
}

void RouteConfig::set_cgi_ext(std::string &cgi_exts) {
    size_t pos = 0;
    std::string token;
    while ((pos = cgi_exts.find_first_of("	 ")) != std::string::npos) {
        token = cgi_exts.substr(0, pos);
        _cgi_extensions.push_back(token);
        cgi_exts.erase(0, pos + 1);
    }
    if (!cgi_exts.empty()) {
        _cgi_extensions.push_back(cgi_exts);
    }
}

void RouteConfig::set_upload_location(const std::string &v) { _upload_location = v; }

int RouteConfig::get_allowed_methods() const { return _allowed_methods; }

std::string RouteConfig::get_redirect() const { return _redirect; }

std::string RouteConfig::get_root() const { return _root; }

std::string RouteConfig::get_index() const { return _index; }

int RouteConfig::get_dir_listing() const { return _dir_listing; }

std::string RouteConfig::get_upload_location() const { return _upload_location; }

std::ostream &operator<<(std::ostream &os, RouteConfig &conf) {
    int am = conf.get_allowed_methods();
    if (am != 0) {
        os << "	allowed methods: ";
        for (int i = 0; i < 4; i++) {
            if ((am & 1 << i) != 0) {
                os << expected_methods[i] << " ";
            }
        }
        os << std::endl;
    }
    if (conf.get_redirect() != "")
        os << "	redirect: " << conf.get_redirect() << std::endl;
    if (conf.get_root() != "")
        os << "	root: " << conf.get_root() << std::endl;
    if (conf.get_index() != "")
        os << "	index: " << conf.get_index() << std::endl;
    os << "	is directory listing set: " << conf.get_dir_listing() << std::endl;
    if (conf.cgi_ext_size() > 0) {
        os << "	cgi extensions: ";
        for (RouteConfig::cgi_ext_iterator it = conf.cgi_ext_begin();
             it != conf.cgi_ext_end(); it++) {
            os << *it << " ";
        }
        os << std::endl;
    }
    if (conf.get_upload_location() != "")
        os << "	upload location: " << conf.get_upload_location() << std::endl;
    return os;
}
