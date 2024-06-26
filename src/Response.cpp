#include "../include/PagesCache.hpp"
#include "../include/Response.hpp"
#include "../include/webserv.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

// Costruttore della classe Response
Response::Response() {
    _protocol = "HTTP/1.1"; // Imposta il protocollo HTTP utilizzato
    _status_code = "200";   // Imposta il codice di stato di default a 200 (OK)
    _status = "OK";         // Imposta il messaggio di stato di default
    _body = "";
    _size = 0;
}

// Distruttore della classe Response
Response::~Response() {}

// Restituisce il testo associato a un codice di stato HTTP
std::string Response::status_text(int status_code) {
    switch (status_code) {
    case STATUS_OK:
        return ("OK");
    case STATUS_FOUND:
        return ("Found");
    case STATUS_UNAUTHORIZED:
        return ("Unauthorized");
    case STATUS_FORBIDDEN:
        return ("Forbidden");
    case STATUS_NOT_FOUND:
        return ("Not Found");
    default:
        return ("Internal Server Error");
    }
}

// Imposta il protocollo HTTP della risposta
void Response::set_protocol(const std::string &protocol) { _protocol = protocol; }

// stato della risposta
void Response::set_status(int status_code) {
    _status_code = itoa(status_code);
    _status = status_text(status_code);
}

// intestazione nella risposta
void Response::set_header(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void Response::set_body(const std::string &body) { _body = body; }

char *make_c_str(size_t _size, const std::string &data) {
    char *str;

    str = new char[_size + 1];
    str[_size] = 0;
    std::copy(data.begin(), data.end(), str);
    return (str);
}

// Converte la risposta in una stringa C per essere inviata al client
char *Response::c_str() {
    if (_protocol == "" && !_body.empty()) {
        _size = _body.size();
        return (make_c_str(_size, _body));
    } else if (_body.empty())
        return (0);

    std::ostringstream response;
    response << _protocol << " " << _status_code << " " << _status << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    response << "\r\n";
    response << _body;

    std::string res_str = response.str();
    _size = res_str.size();
    return (make_c_str(_size, res_str));
}

// Calcola la lunghezza della risposta completa
size_t Response::length() const {
    if (_size != 0)
        return (_size);
    std::ostringstream response;
    response << _protocol << " " << _status_code << " " << _status << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    response << "\r\n" << _body;
    return (response.str().length());
}

void read_file(const std::string &file_path, std::string &str) {
    std::ifstream file(file_path.c_str(), std::ios::binary);
    std::stringstream ss;

    if (!file.is_open()) {
        return;
    }

    ss << file.rdbuf();
    str = ss.str();
}

void Response::add_default_headers() {
    time_t rawtime;
    std::string timeStr;

    time(&rawtime);
    timeStr = ctime(&rawtime);
    timeStr = timeStr.substr(0, timeStr.size() - 1);

    set_header("Server", "Webserv");
    set_header("Date", timeStr);
    if (_body.length() > 0)
        set_header("Content-Length", itoa(_body.length()));
}

bool Response::set_body_custom(Config *config, unsigned int error_number)
{
    for (Config::error_pages_iterator it_err = config->error_pages_begin(); it_err != config->error_pages_end(); it_err++)
    {
        if (config->error_pages_empty())
            break;
        if (it_err->first == error_number)
        {
            if (Pages::get(uitoa(it_err->first) + "_custom") != Pages::get_500()) // se va a buon fine
            {
                set_body(Pages::get(uitoa(it_err->first) + "_custom"));
            }
            else // se NON va a buon fine
                set_body(Pages::get_500());
            return true;
        }
    }
    return false;
}

void Response::make_400(Config *config)
{
    set_protocol(PROTOCOL_11);
    set_status(STATUS_BAD_REQUEST);
    if (set_body_custom(config, 400) == false)
        set_body(Pages::get_400());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_404(Config *config) {
    set_protocol(PROTOCOL_11);
    set_status(STATUS_NOT_FOUND);
    if (set_body_custom(config, 404) == false)
        set_body(Pages::get_404());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_405(Config *config)
{
    set_protocol(PROTOCOL_11);
    set_status(STATUS_METHOD_NOT_ALLOWED);
    if (set_body_custom(config, 405) == false)
        set_body(Pages::get_405());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_302(const std::string &loc, Config *config)
{
    set_protocol(PROTOCOL_11);
    set_status(STATUS_FOUND);
    set_header("Location", loc);
    if (set_body_custom(config, 302) == false)
        set_body(Pages::get_302(loc));
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_500(Config *config)
{
    set_protocol(PROTOCOL_11);
    set_status(STATUS_INTERNAL_SERVER_ERROR);
    if (set_body_custom(config, 500) == false)
        set_body(Pages::get_500());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_timeout() {
    set_protocol(PROTOCOL_11);
    set_status(STATUS_INTERNAL_SERVER_ERROR);
    set_body(Pages::get_timeout());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_page() {
    set_protocol(PROTOCOL_11);
    set_status(STATUS_OK);
    set_body(_body);
    add_default_headers();
}

bool Response::check_timer(int fd, pid_t pid) {
    fd_set set;             // Insieme di file descriptor per `select`
    struct timeval timeout; // Struttura per impostare il timeout
    // Imposta il timeout
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;
    FD_ZERO(&set);    // Inizializza l'insieme di file descriptor
    FD_SET(fd, &set); // Aggiunge il file descriptor della pipe all'insieme
    // Usa `select` per aspettare fino a quando il file descriptor è pronto o il timeout
    // scade
    int value = select(fd + 1, &set, NULL, NULL,
                       &timeout); // fd + 1 è il numero di file descriptor da monitorare
    if (value == -1)
    // Se `select` ritorna -1, c'è stato un errore
    {
        close(fd);
        return (false);
    } else if (value == 0) // Se `select` ritorna 0, il timeout è stato raggiunto
    {
        std::cerr << "Timeout" << std::endl;
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        close(fd);
        return (false);
    }
    return (true);
}

/* funzione per la gestione della response della cgi */
void Response::handle_cgi_response(Request &req, Response *resp, int language) {
    char *argv[3];
    int fd[2];
    int pid;
    char buffer[BUFFER_SIZE];
    int byteread;
    int status;

    std::string path = req.get_path().substr(1);
    std::string cmd;
    if (language == PYTHON) {
        argv[1] = (char *)path.c_str();
        argv[2] = NULL;
        cmd = "/usr/bin/python3";
    } else //(language == PHP)
    {
        argv[1] = (char *)path.c_str();
        argv[2] = NULL;
        cmd = "/usr/bin/php";
    }
    argv[0] = (char *)cmd.c_str();
    /* apro il file con ACCESS */
    // std::cout << "quando qua dentro per capire se la path è corretta :" << path <<
    // std::endl;
    if (access(path.c_str(), R_OK | X_OK) != -1) {
        if (pipe(fd) == -1) {
            std::cout << ERROR_OPEN_PIPE << std::endl;
            return;
        }
        /* faccio il fork e lavoro il processo figlio */
        pid = fork();
        if (pid == -1) {
            std::cout << ERROR_FORK << std::endl;
            return;
        }
        if (pid == 0) // CHILD PROCESS
        {
            close(fd[0]); // unused
            if (dup2(fd[1], STDOUT_FILENO) == -1) {
                std::cout << ERROR_EXECVE << std::endl;
                return;
            }
            close(fd[1]); // unused
            execve(cmd.c_str(), argv, 0);
            std::cout << ERROR_EXECVE << std::endl;
            return;
        }
        else // parent process
        {
            close(fd[1]);
            if (check_timer(fd[0], pid) == false) {
                std::cout << "Process terminated due to timeout" << std::endl;
                return (make_timeout());
            }
            std::string output;
            while ((byteread = read(fd[0], buffer, BUFFER_SIZE)) > 0) {
                output.append(buffer, byteread);
            }
            close(fd[0]); // close dopo che ho finito di leggere
            waitpid(pid, &status, 0);
            std::cout << "questo è l'output" << output << std::endl;
            resp->set_body(output); // uscita dalla funzione dell'output
            return;
        }
    } else {
        std::cout << ERROR_FILE_NOT_ACCESS << std::endl;
        return;
    }
}

std::string make_path(std::string &req_path, RouteConfig &rc, Config &srv) {
    std::string rc_root = rc.get_root();
    std::string rc_index = rc.get_index();
    std::string srv_root = srv.get_root();
    std::string srv_index = srv.get_index();

    std::string root = rc_root.empty() ? (srv_root.empty() ? "." : srv_root) : rc_root;
    std::string path = root + req_path;

    if (req_path == "/" && !srv_index.empty())
        return path + srv_index;
    if (rc_index.empty() || rc_index == "no-index")
        return path;
    return root + (rc_index[0] == '/' ? rc_index : '/' + rc_index);
}


// Prepara la risposta HTTP in base alla richiesta
void Response::prepare_response(Request &req, Server *server) {
    RouteConfig route_config;
    Config &server_config = *server->get_config();
    std::string req_path = req.get_path();

    server->get_path_config(req_path, route_config);
    std::cout << route_config << std::endl;

    if (req_path.find(".py") != std::string::npos)
        return (handle_cgi_response(req, this, PYTHON));
    if (req_path.find(".php") != std::string::npos)
        return handle_cgi_response(req, this, PHP);

    if (route_config.get_allowed_methods() != 0 &&
        (route_config.get_allowed_methods() & req.get_method()) == 0)
        return make_405(&server_config);

    if (route_config.get_redirect() != "") {
        return make_302(route_config.get_redirect(), &server_config);
    }

    std::string path = make_path(req_path, route_config, server_config);
    std::cout << "path is: " << path << std::endl;

    if (route_config.get_dir_listing() == 1)
        return make_autoindex(path, &server_config);

    if (route_config.get_upload_location() != "")
        return save_file(req, route_config.get_upload_location());

    if (access(path.c_str(), R_OK) != -1)
        read_file(path, _body);

    set_header("Content-Type", "text/html; charset=utf-8");
    if (path.find(".jpg") != std::string::npos)
        set_header("Content-Type", "image/jpeg");
    else if (path.find(".png") != std::string::npos)
        set_header("Content-Type", "image/png");

    if (_body.size() == 0) {
        std::cout << "empty." << std::endl;
        return make_404(&server_config);
    }
    return make_page();
}
