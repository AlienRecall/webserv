#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "PagesCache.hpp"
#include "Request.hpp"
#include "Server.hpp"
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
#include <utility>

#define PROTOCOL_11 "HTTP/1.1"

#define STATUS_OK 200
#define STATUS_CREATED 201
#define STATUS_MOVED_PERM 301
#define STATUS_FOUND 302
#define STATUS_UNAUTHORIZED 401
#define STATUS_FORBIDDEN 403
#define STATUS_NOT_FOUND 404
#define STATUS_METHOD_NOT_ALLOWED 405
#define STATUS_URI_TOO_LONG 414
#define STATUS_RATE_LIMITED 429
#define STATUS_INTERNAL_SERVER_ERROR 500
#define STATUS_NOT_IMPLEMENTED 501
#define STATUS_BAD_GATEWAY 502

class Response {
  private:
    size_t _size;
    std::string _protocol;                       // Stringa per il protocollo HTTP
    std::string _status_code;                    // Stringa per il codice di stato
    std::string _status;                         // Stringa per il messaggio di stato
    std::map<std::string, std::string> _headers; // Mappa per gli headers della risposta
    std::string _body;                           // Stringa per il corpo della risposta

    std::string status_text(int); // Metodo per ottenere il messaggio di stato

    void add_default_headers();

    void make_autoindex(const std::string &);

    // methods to prepare the response
    void make_404();
    void make_405();
    void make_302(const std::string &);
    void make_500();

  public:
    Response();
    ~Response();

    static std::string itoa(int nbr) {
        std::stringstream ss;
        ss << nbr;
        return ss.str();
    };

    void set_protocol(const std::string &protocol);            // protocollo HTTP
    void set_status(int);                                      // codice di stato
    void set_header(const std::string &, const std::string &); // intestazione
    void set_body(const std::string &);                        // corpo della risposta
    char *c_str();         // Metodo per ottenere la risposta come stringa C
    size_t length() const; // calcolare la lunghezza di tutto quello che c'è da passare

    void prepare_response(Request &, Server *);
};

#endif
