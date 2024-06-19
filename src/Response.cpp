#include "../include/PagesCache.hpp"
#include "../include/Response.hpp"
#include "../include/webserv.hpp"

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
        return "OK";
    case STATUS_FOUND:
        return "Found";
    case STATUS_UNAUTHORIZED:
        return "Unauthorized";
    case STATUS_FORBIDDEN:
        return "Forbidden";
    default:
        return "Internal Server Error";
    }
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

char *make_c_str(size_t _size, const std::string &data) {
    char *str = new char[_size + 1];
    str[_size] = 0;
    std::copy(data.begin(), data.end(), str);
    return str;
}

// Converte la risposta in una stringa C per essere inviata al client
char *Response::c_str() {
    if (_protocol == "" && !_body.empty()) {
        _size = _body.size();
        return make_c_str(_size, _body);
    } else if (_body.empty())
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

    _size = res_str.size();
    return make_c_str(_size, res_str);
}

// Calcola la lunghezza della risposta completa
size_t Response::length() const {
    if (_size != 0)
        return _size;
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
    set_body(Pages::get_405());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_302(const std::string &loc) {
    set_protocol(PROTOCOL_11);
    set_status(STATUS_FOUND);
    set_header("Location", loc);
    set_body(Pages::get_302(loc));
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_500() {
    set_protocol(PROTOCOL_11);
    set_status(STATUS_INTERNAL_SERVER_ERROR);
    set_body(Pages::get_500());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

void Response::make_timeout()
{
    set_protocol(PROTOCOL_11);
    set_status(STATUS_INTERNAL_SERVER_ERROR);
    set_body(Pages::get_timeout());
    set_header("Content-Type", "text/html; charset=utf-8");
    add_default_headers();
}

bool Response::check_timer(int fd, pid_t pid)
{
    fd_set set;             // Insieme di file descriptor per `select`
    struct timeval timeout; // Struttura per impostare il timeout

    // Imposta il timeout
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    FD_ZERO(&set);       // Inizializza l'insieme di file descriptor
    FD_SET(fd, &set); // Aggiunge il file descriptor della pipe all'insieme

    // Usa `select` per aspettare fino a quando il file descriptor è pronto o il timeout scade
    int value = select(fd + 1, &set, NULL, NULL, &timeout); // fd + 1 è il numero di file descriptor da monitorare
    if (value == -1) // Se `select` ritorna -1, c'è stato un errore
    {
        close(fd);
        return false;
    }
    else if (value == 0) // Se `select` ritorna 0, il timeout è stato raggiunto
    {
        std::cerr << "Timeout" << std::endl;
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        close(fd);
        return false;
    }
    return true;
}

/* funzione per la gestione della response della cgi */
void Response::handle_cgi_response(Request &req, Response *resp, int language)
{
    char *argv[3];
    std::string path = req.get_path().substr(1);
    std::string cmd;

    if (language == PYTHON)
    {
        std::cout << "sono dentro python" << std::endl;
        argv[1] = (char *)path.c_str();
        argv[2] = NULL;
        cmd = "/usr/bin/python3";
    }

    else //(language == PHP)
    {
        argv[1] = (char *)path.c_str();
        argv[2] = NULL;
        cmd = "/usr/bin/php";
    }

    argv[0] = (char *)cmd.c_str();
    /* apro il file con ACCESS */
    std::cout << "quando qua dentro per capire se la path è corretta :" << path << std::endl;
    if (access(path.c_str(), R_OK | X_OK) != -1)
    {
        int fd[2];
        if (pipe(fd) == -1)
        {
            std::cout << ERROR_OPEN_PIPE << std::endl;
            return;
        }
        /* faccio il fork e lavoro il processo figlio */
        int pid = fork();
        if (pid == -1)
        {
            std::cout << ERROR_FORK << std::endl;
            return;
        }
        if (pid == 0) // CHILD PROCESS
        {
            close(fd[0]); // unused
            if (dup2(fd[1], STDOUT_FILENO) == -1)
            {
                std::cout << ERROR_EXECVE << std::endl;
                exit(EXIT_FAILURE);
            }
            close(fd[1]); // unused
            execve(cmd.c_str(), argv, 0);
            std::cout << ERROR_EXECVE << std::endl;
            exit(EXIT_FAILURE);
        }
        else // parent process
        {
            close(fd[1]);
            if (check_timer(fd[0], pid) == false)
            {
                std::cout << "Process terminated due to timeout" << std::endl;
                return make_timeout(); 
            }

            char buffer[BUFFER_SIZE];
            std::string output;
            int byteread;

            while ((byteread = read(fd[0], buffer, BUFFER_SIZE)) > 0)
            {
                output.append(buffer, byteread);
            }
            close(fd[0]); // close dopo che ho finito di leggere
            int status;
            waitpid(pid, &status, 0);
            std::cout << "questo è l'output" << output << std::endl;
            resp->set_body(output); // uscita dalla funzione dell'output
            return;
        }
    }
    else
    {
        std::cout << ERROR_FILE_NOT_ACCESS << std::endl;
        return;
    }
}

// Prepara la risposta HTTP in base alla richiesta
void Response::prepare_response(Request &req, Server *server) {
    Error err;
    RouteConfig route_config;

    (void)err;
    
    server->get_path_config(req.get_path(), route_config);
    std::cout << route_config << std::endl;

    if (req.get_path().find(".py") != std::string::npos)
        return (handle_cgi_response(req, this, PYTHON));
    if (req.get_path().find(".php") != std::string::npos)
       return (handle_cgi_response(req, this, PHP));
        
    if (route_config.get_allowed_methods() != 0 &&
        (route_config.get_allowed_methods() & req.get_method()) == 0)
        return make_405();

    if (route_config.get_redirect() != "")
        return make_302(route_config.get_redirect());

    std::string path = route_config.get_root().empty()
                           ? req.get_path()
                           : route_config.get_root() + req.get_path();

    if (route_config.get_dir_listing() == 1)
        return make_autoindex(path);

    // checkiamo se la location/server ha una root dir

    return make_500();
}


