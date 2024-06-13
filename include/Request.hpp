#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Config.hpp"
#include "Logger.hpp"
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
    int method;
    std::string path;
    std::string protocol;
    std::map<std::string, std::string> headers;
    std::string body;

  public:
    Request();
    ~Request();

    int popRequest(char *buffer, int client_request);
    int getMethod(std::stringstream &buffer);
    int getPath(std::stringstream &buffer);
    int getProtocol(std::stringstream &buffer);
    int getHeaders(std::stringstream &buffer);
    int getBody(std::stringstream &buffer, int ClientFd);
};

#endif
