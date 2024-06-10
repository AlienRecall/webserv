#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser() : logger(Logger("ConfigParser")) { _actual = 0; }

ConfigParser::~ConfigParser() {}

ConfigParser::ConfigParser(const ConfigParser &cp)
    : logger(Logger("ConfigParser")) {
    *this = cp;
}

ConfigParser &ConfigParser::operator=(const ConfigParser &cp) {
    if (this == &cp)
        return *this;
    return *this;
}

ConfigParser::ConfigParser(const std::string &file)
    : logger(Logger("ConfigParser")), _config_file(file) {}

Error ConfigParser::populate_config(Config &conf) {
    // std::cout << "populating key_type: " << _key_type << ", with value: " <<
    // _value << std::endl;
    switch (_key_type) {
    case 0:
        conf.set_server_name(_value);
        break;
    case 1:
        conf.set_host(_value);
        break;
    case 2:
        conf.set_port(_value);
        break;
    }
    return OK;
}

Error ConfigParser::populate_route_config(RouteConfig &conf) {
    // std::cout << "populating key_type: " << _key_type << ", with value: " <<
    // _value << std::endl;
    switch (_key_type) {
    case 3: {
        int tosplit = _value.find_first_of("	 ");
        unsigned int status = std::atoi(_value.substr(0, tosplit).c_str());
        conf.set_error_page(status, _value.substr(tosplit + 1));
        break;
    }
    case 4:
        conf.set_client_body_size(std::atoi(_value.c_str()));
        break;
    case 5:
        conf.set_index(_value);
        break;
    case 6:
        conf.set_allowed_methods(_value);
        break;
    case 7:
        conf.set_redirect(_value);
        break;
    case 8:
        conf.set_dir_listing(_value);
        break;
    case 9:
        conf.set_root(_value);
        break;
    case 10:
        conf.set_cgi_ext(_value);
        break;
    case 11: {
        conf.set_upload_location(_value);
        break;
    }
    }
    return OK;
}

Error ConfigParser::take_value(std::string line) {
    size_t pos = line.find(';');
    if (_key_type == LOCATION_KEY)
        pos = line.find(" {");
    if (pos == std::string::npos)
        return _key_type == LOCATION_KEY ? NO_CURLY : NO_SEMICOLON;
    size_t start = line.find(_key) + _key.length();
    start = line.find_first_not_of("	 ", start);
    if (line[start] == ';')
        return NO_VALUE;
    _value = line.substr(start, pos - start);
    if (_key_type == LOCATION_KEY && _value[0] != '/')
        _value = '/' + _value;
    return OK;
}

unsigned int is_valid_key(std::string key) {
    for (int i = 0; i < 13; i++)
        if (key == expected_keys[i])
            return i;
    return -1;
}

Error ConfigParser::take_key(std::string line) {
    int start = -1, len = -1;
    for (std::string::iterator it = line.begin(); it != line.end(); it++) {
        if (*it == ';')
            return WRONG_SEMICOLON;
        if (std::isspace(*it)) {
            if (start != -1) {
                len = (it - line.begin()) - start;
                break;
            }
            continue;
        }
        if (start == -1)
            start = it - line.begin();
    }
    if (start == -1)
        return EMPTY_LINE;
    else if (len == -1)
        return NO_VALUE;
    _key = line.substr(start, len);
    return OK;
}

Error ConfigParser::detect_field(std::string line) {
    _key = "";
    _value = "";
    _key_type = -1;

    Error err;
    err = take_key(line);
    if (err != OK)
        return logger.log_error(err, line);
    _key_type = is_valid_key(_key);
    if (static_cast<int>(_key_type) == -1)
        return logger.log_error(NO_KEY_FOUND, line);
    err = take_value(line);
    if (err != OK)
        return logger.log_error(err, line);
    return OK;
}

Error ConfigParser::parse_segment(std::ifstream &file, RouteConfig *rc,
                                  std::string route) {
    Error err;
    std::string line;
    RouteConfig *route_conf;

    while (file) {
        std::getline(file, line);
        if (line.empty())
            continue;
        if (line.find('}') != std::string::npos)
            break;
        err = detect_field(line);
        if (err != OK && err != EMPTY_LINE)
            return err;
        if (_key_type == LOCATION_KEY) {
            route_conf = new RouteConfig();
            _actual->set_location(route + _value, route_conf);
            err = parse_segment(file, route_conf, route + _value);
            if (err != OK)
                return err;
        } else if (route.empty()) {
            if (_key_type < 3)
                populate_config(*_actual);
            else
                populate_route_config(*_actual);
        } else {
            populate_route_config(*rc);
        }
    }
    return OK;
}

Error ConfigParser::parse_server(std::ifstream &file) {
    Error err;
    std::string line;
    Config conf;
    _actual = &conf;

    err = parse_segment(file, 0);
    if (err != OK)
        return err;
    _actual = 0;
    _configs.push_back(conf);
    return OK;
}

Error ConfigParser::load_config() {
    std::ifstream file;
    std::string line;
    Error err;

    file.open(_config_file);
    if (!file.is_open()) {
        return OPEN_FILE;
    }

    while (file) {
        std::getline(file, line);
        if (line.empty())
            continue;
        if (line.find("server {") != std::string::npos) {
            err = parse_server(file);
            if (err != OK)
                return err;
        }
    }
    std::cout << "size: " << _configs.size() << std::endl;
    return OK;
}
