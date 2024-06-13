#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Config.hpp"
#include "Logger.hpp"
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#define ERROR_GETTING_METHOD 900
#define ERROR_GETTING_PATH 909
#define ERROR_GETTING_PROTOCOL 999
#define ERROR_GETTING_HEADERS 990
#define ERROR_GETTING_BODY 9990

class Request {
  private:
    int _method;
    std::string _path;
    std::string _protocol;
    std::map<std::string, std::string> _headers;
    std::string _body;

  public:
    Request();
    ~Request();

    typedef std::map<std::string, std::string>::iterator iterator;

    int get_method();
    std::string &get_path();
    std::string &get_protocol();
    iterator get_header(const std::string &);
    std::string &get_body();

    int popRequest(char *buffer, int client_request);
    int parseMethod(std::stringstream &buffer);
    int parsePath(std::stringstream &buffer);
    int parseProtocol(std::stringstream &buffer);
    int parseHeaders(std::stringstream &buffer);
    int parseBody(std::stringstream &buffer, int ClientFd);
};

#endif
