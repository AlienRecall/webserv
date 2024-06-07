#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

ConfigParser::ConfigParser(const ConfigParser &cp) { *this = cp; }

ConfigParser &ConfigParser::operator=(const ConfigParser &cp) {
    if (this == &cp)
        return *this;
    return *this;
}

ConfigParser::ConfigParser(const std::string &file) : _config_file(file) {}

void ConfigParser::open_file() {}

void ConfigParser::load_config() {}
