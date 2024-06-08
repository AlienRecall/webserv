#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <string>

enum Error {
    OK,
    CUSTOM,
    OPEN_FILE,
    EMPTY_LINE,
    NO_VALUE,
    WRONG_SEMICOLON,
    NO_SEMICOLON,
    NO_CURLY,
    NO_KEY_FOUND,
};

class Logger {
  private:
    std::string _title;

    void print_ts() {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        std::cout << "[" << buf << "] ";
    }

    void print_title() { std::cout << _title << ": "; }

  public:
    Logger(const std::string &t) : _title(t) {}
    ~Logger() {}

    void log_server(std::string line) {
        print_ts();
        print_title();
        std::cout << line << std::endl;
    }

    Error log_custom_error(const std::string &str) {
        print_title();

        std::cout << str << std::endl;
        return CUSTOM;
    }

    Error log_error(Error err, std::string line = "") {
        print_title();
        if (line != "")
            std::cout << "On line: " << line << std::endl << "\t";
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

        default:
            break;
        }
        std::cout << std::endl;
        return OK;
    }
};

#endif
