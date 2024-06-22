#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Config.hpp"
#include "Logger.hpp"
#include <cctype>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#define ERROR_GETTING_METHOD 111
#define ERROR_GETTING_PATH 222
#define ERROR_GETTING_PROTOCOL 333
#define ERROR_GETTING_HEADERS 444
#define ERROR_GETTING_BODY 555
#define ERROR_READ_CONTENT_DIFF 666
#define MAX_CLIENT_BODY_SIZE_EXCEEDED 777

class Request {
  private:
    int _method;
    std::string _path;
    std::string _protocol;
    std::map<std::string, std::string> _headers;
    std::string _body;

    int parseMethod(std::stringstream &);
    int parsePath(std::stringstream &);
    int parseProtocol(std::stringstream &);
    int parseHeaders(std::stringstream &);
    int parseBody(std::stringstream &, int, int);

  public:
    Request();
    ~Request();

    typedef std::map<std::string, std::string>::iterator iterator;
    typedef std::map<std::string, std::string>::const_iterator const_iterator;
    iterator begin() { return _headers.begin(); }
    iterator end() { return _headers.end(); }

    int get_method() const;
    std::string &get_path();
    const std::string &get_protocol() const;
    iterator get_header(const std::string &);
    std::string &get_body();

    int popRequest(char *, int, unsigned int);
};

#endif
