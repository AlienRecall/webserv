#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "PagesCache.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include <algorithm>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <utility>

#define PROTOCOL_11 "HTTP/1.1"

#define STATUS_OK 200
#define STATUS_CREATED 201
#define STATUS_MOVED_PERM 301
#define STATUS_FOUND 302
#define STATUS_BAD_REQUEST 400
#define STATUS_UNAUTHORIZED 401
#define STATUS_FORBIDDEN 403
#define STATUS_NOT_FOUND 404
#define STATUS_METHOD_NOT_ALLOWED 405
#define STATUS_URI_TOO_LONG 414
#define STATUS_RATE_LIMITED 429
#define STATUS_INTERNAL_SERVER_ERROR 500
#define STATUS_NOT_IMPLEMENTED 501
#define STATUS_BAD_GATEWAY 502
#define TIMEOUT_SECONDS 5 // Imposta il timeout

#define PYTHON 0
#define PHP 1
#define ERROR_OPEN_PIPE "error opening the pipe"
#define ERROR_FORK "forking error motherfuckers"
#define ERROR_EXECVE "execve, not working, not working!"
#define ERROR_FILE_NOT_ACCESS "file not accessible"

static std::string supported_types[2] = {
    "image/png",
    "image/jpeg",
};

typedef struct t_file {
    std::string data;
    std::string file_name;
} s_file;

class Response {
  private:
    size_t _size;
    std::string _protocol;
    std::string _status_code;
    std::string _status;
    std::map<std::string, std::string> _headers;
    std::string _body;

    std::string status_text(int); // Metodo per ottenere il messaggio di stato

    void add_default_headers();

    void make_autoindex(const std::string &, Config *);
    void handle_cgi_response(Request &req, Response *resp, int language);
    bool check_timer(int fd, pid_t pid);
    void save_file(Request &, const std::string &);

  public:
    Response();
    ~Response();

    static std::string itoa(int nbr) {
        std::stringstream ss;
        ss << nbr;
        return (ss.str());
    };

    const std::string &get_status_code() { return _status_code; }

    void set_protocol(const std::string &protocol);
    // protocollo HTTP
    void set_status(int);
    // codice di stato
    void set_header(const std::string &, const std::string &); // intestazione
    void set_body(const std::string &);
    // corpo della risposta
    char *c_str();
    // Metodo per ottenere la risposta come stringa C
    size_t length() const;
    // calcolare la lunghezza di tutto quello che c'è da passare

    // methods to prepare the response
    void make_400(Config *config);
    void make_404(Config *);
    void make_405(Config *config);
    void make_302(const std::string &, Config *config);
    void make_500(Config *config);
    void make_page();
    void make_timeout();

    void prepare_response(Request &, Server *);

    // std::string uitoa(unsigned int v);
    // bool file_exists(const std::string &path);
    bool set_body_custom(Config *config, unsigned int error_number);
};

#endif
