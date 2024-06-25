#include "../include/Logger.hpp"

Logger::Logger() {}

Logger::Logger(const std::string &t) : _debug(false), _title(t) {}

Logger::~Logger() {}

Logger &Logger::operator=(const Logger &l) {
    if (this == &l)
        return *this;
    _debug = l._debug;
    _title = l._title;
    return *this;
}

void Logger::print_ts() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    std::cout << "[" << buf << "] ";
}
void Logger::print_title() { std::cout << _title << ": "; }

void Logger::set_debug(bool v) { _debug = v; }

void Logger::set_title(const std::string &t) { _title = t; }

void Logger::print_ts(std::string line) {
    print_ts();
    std::cout << line << std::endl;
}

void Logger::print(std::string line) {
    print_title();
    std::cout << line << std::endl;
}

Error Logger::error(const std::string &str) {
    print_title();
    std::cout << str << std::endl;
    return CUSTOM;
}

Error Logger::debug(const std::string &str) {
    if (!_debug) {
        return CUSTOM;
    }
    std::cout << str << std::endl;
    return CUSTOM;
}

Error Logger::log_error(Error err, std::string line) {
    print_title();
    switch (err) {
    case OPEN_FILE:
        std::cout << "cannot open file";
        break;
    case EMPTY_LINE:
        std::cout << "empty line";
        break;
    case NO_VALUE:
        std::cout << "key does not have a value";
        break;
    case WRONG_SEMICOLON:
        std::cout << "';' in wrong place";
        break;
    case NO_SEMICOLON:
        std::cout << "no ';' at the end of line";
        break;
    case NO_CURLY:
        std::cout << "expected '{' after location declaration";
        break;
    case NO_KEY_FOUND:
        std::cout << "unexpected key";
        break;
    case METHOD_NOT_SUPPORTED:
        std::cout << "method not supported";
        break;
    case NO_SLASH_PATH:
        std::cout << "invalid path, it should start with '/'";
        break;
    case INVALID_PROTOCOL:
        std::cout << "invalid protocol";
        break;
    case INVALID_HEADERS:
        std::cout << "error parsing headers";
        break;
    case NO_CONTENT_LEN:
        std::cout << "no content length header found";
        break;
    case BODY_SIZE_EXCEEDED:
        std::cout << "payload is larger than max body size";
        break;
    case CONTENT_DIFF:
        std::cout << "content length is different from the payload length";
        break;
    case INVALID_CUSTOM_FILE:
        std::cout << "file not accepted, modify the name";
        break;
    default:
        std::cout << "error code: " << err;
        break;
    }
    std::cout << std::endl;
    if (line != "")
        std::cout << "\ton line: " << line << std::endl;
    return err;
}
