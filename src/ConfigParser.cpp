#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser() : logger(Logger("ConfigParser")) {}

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
    case 3:
        conf.set_client_body_size(atoi(_value.c_str()));
        break;
    case 4: {
        int tosplit = _value.find_first_of("    ");
        unsigned int status = atoi(_value.substr(0, tosplit).c_str());
        conf.set_error_page(status, _value.substr(tosplit + 1));
        break;
    }
    case 5:
        conf.set_index(_value);
        break;
    case 6:
        conf.set_client_body_size(atoi(_value.c_str()));
        break;
    case 7:
        conf.set_client_body_size(atoi(_value.c_str()));
        break;
    }
    return OK;
}

Error ConfigParser::take_value(std::string line) {
    size_t pos = line.find(';');
    if (_key_type == 11)
        pos = line.find(" {");
    if (pos == std::string::npos)
        return _key_type == 11 ? NO_CURLY : NO_SEMICOLON;
    size_t start = line.find(_key) + _key.length();
    start = line.find_first_not_of("     ", start);
    if (line[start] == ';')
        return NO_VALUE;
    // for (; start < line.length(); start++) {
    //     if (std::isspace(line[start]))
    //         continue;
    //     if (line[start] == ';')

    //     break;
    // }
    _value = line.substr(start, pos - start);
    return OK;
}

unsigned int is_valid_key(std::string key) {
    for (int i = 0; i < 11; i++)
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
    if (_key_type == -1)
        return logger.log_error(NO_KEY_FOUND, line);
    if (_key_type == 11)
        return OK;
    err = take_value(line);
    if (err != OK)
        return logger.log_error(err, line);
    return OK;
}

Error ConfigParser::parse_server(std::ifstream &config) {
    Error err;
    std::string line;
    Config conf;

    while (config) {
        std::getline(config, line);
        if (line == "}")
            break;
        if (line.empty())
            continue;
        err = detect_field(line);
        if (err != OK && err != EMPTY_LINE)
            return err;
        if (_key_type == 11)
            parse_location(config);
    }
    _configs.push_back(conf);
}

Error ConfigParser::load_config() {
    std::ifstream config;
    std::string line;
    Error err;

    config.open(_config_file);
    if (!config.is_open()) {
        return OPEN_FILE;
    }

    while (config) {
        std::getline(config, line);
        if (line.find("server {") != std::string::npos) {
            err = parse_server(config);
            if (err != OK)
                return err;
        }
    }
    return OK;
}
