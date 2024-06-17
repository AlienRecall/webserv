#include "../include/Response.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Costruttore della classe Response
Response::Response() {
    _protocol = "HTTP/1.1"; // Imposta il protocollo HTTP utilizzato
    _status_code = "200";   // Imposta il codice di stato di default a 200 (OK)
    _status = "OK";         // Imposta il messaggio di stato di default
    _body = "";
}

// Distruttore della classe Response
Response::~Response() {}

// Restituisce il testo associato a un codice di stato HTTP
std::string Response::status_text(int status_code) {
    if (status_code == STATUS_OK)
        return "OK";
    if (status_code == STATUS_FOUND)
        return "Found";
    if (status_code == STATUS_UNAUTHORIZED)
        return "Unauthorized";
    if (status_code == STATUS_FORBIDDEN)
        return "Forbidden";
    return "Internal Server Error";
}

// Imposta il protocollo HTTP della risposta
void Response::set_protocol(const std::string &protocol) { _protocol = protocol; }

// stato della risposta
void Response::set_status(int status_code) {
    // Usa std::stringstream per convertire status_code in stringa
    std::stringstream ss;
    ss << status_code;
    _status_code = ss.str();

    _status = status_text(status_code);
}

// intestazione nella risposta
void Response::set_header(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void Response::set_body(const std::string &body) { _body = body; }

// Converte la risposta in una stringa C per essere inviata al client
char *Response::c_str() const {
    if (_protocol == "")
        return 0;
    std::ostringstream response;
    response << _protocol << " " << _status_code << " " << _status << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    response << "\r\n";
    response << _body;
    std::string res_str = response.str();
    char *res_cstr = new char[res_str.size() + 1];
    std::copy(res_str.begin(), res_str.end(), res_cstr);
    res_cstr[res_str.size()] = '\0';
    return res_cstr;
}

// Calcola la lunghezza della risposta completa
size_t Response::length() const {
    std::ostringstream response;
    response << _protocol << " " << _status_code << " " << _status << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    response << "\r\n" << _body;
    return response.str().length();
}

void Response::add_default_headers() {
    set_header("Server", "Webserv");
    if (_body.length() > 0)
        set_header("Content-Length", itoa(_body.length()));
}

void Response::make_405() {
    set_protocol(PROTOCOL_11);
    set_status(STATUS_METHOD_NOT_ALLOWED);

    add_default_headers();
}

// Prepara la risposta HTTP in base alla richiesta
void Response::prepare_response(Request &req, Server *server) {
    Error err;
    RouteConfig route_config;

    (void)err;
    server->get_path_config(req.get_path(), route_config);
    std::cout << route_config << std::endl;

    if (route_config.get_allowed_methods() != 0 &&
        (route_config.get_allowed_methods() & req.get_method()) == 0)
        return make_405(); // return 405 method not allowed

    //  check body size

    // checkiamo se la location/server ha una root dir
    // check se ha una redirect
}
