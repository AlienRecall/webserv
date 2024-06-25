#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <string>

enum Error
{
  OK = 0,
  CUSTOM = -1,
  OPEN_FILE,
  EMPTY_LINE,
  NO_VALUE,
  WRONG_SEMICOLON,
  NO_SEMICOLON,
  NO_CURLY,
  NO_KEY_FOUND,
  METHOD_NOT_SUPPORTED,
  NO_SLASH_PATH,
  INVALID_PROTOCOL,
  INVALID_HEADERS,
  NO_CONTENT_LEN,
  BODY_SIZE_EXCEEDED,
  CONTENT_DIFF,
  INVALID_CUSTOM_FILE
};

class Logger {
  private:
    bool _debug;
    std::string _title;

    Logger();

    void print_ts();
    void print_title();

  public:
    Logger(const std::string &);
    ~Logger();
    Logger &operator=(const Logger &);

    void set_debug(bool);
    void set_title(const std::string &);

    void print(std::string);
    void print_ts(std::string);
    Error error(const std::string &str);
    Error debug(const std::string &str);
    Error log_error(Error err, std::string line = "");
};

#endif
