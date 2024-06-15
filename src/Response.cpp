#include "../include/Response.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Costruttore della classe Response
Response::Response()
{
	_protocol = "HTTP/1.1"; // Imposta il protocollo HTTP utilizzato
	_status_code = "200";	// Imposta il codice di stato di default a 200 (OK)
	_status = "OK";			// Imposta il messaggio di stato di default
	_body = "";
}

// Distruttore della classe Response
Response::~Response() {}

// Restituisce il testo associato a un codice di stato HTTP
std::string Response::status_text(int status_code)
{
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
void Response::set_protocol(const std::string &protocol)
{
	_protocol = protocol;
}

// stato della risposta
void Response::set_status(int status_code)
{
	// Usa std::stringstream per convertire status_code in stringa
	std::stringstream ss;
	ss << status_code;
	_status_code = ss.str();

	_status = status_text(status_code);
}

// intestazione nella risposta
void Response::set_header(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

void Response::set_body(const std::string &body)
{
	_body = body;
}



// Converte la risposta in una stringa C per essere inviata al client
char *Response::c_str() const
{
	std::ostringstream response;
	response << _protocol << " " << _status_code << " " << _status << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
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
size_t Response::length() const
{
	std::ostringstream response;
	response << _protocol << " " << _status_code << " " << _status << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		response << it->first << ": " << it->second << "\r\n";
	}
	response << "\r\n" << _body;
	return response.str().length();
}


// Prepara la risposta HTTP in base alla richiesta
void Response::prepare_response(int method, std::string path)
{
	// Gestione della richiesta GET
	if (method == GET)
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
		{ // Se il file non viene aperto
			set_status(STATUS_NOT_FOUND);
			_body = "<html><body><h1>404 Not Found</h1></body></html>";
		}
		else
		{
			std::stringstream buffer;
			buffer << file.rdbuf(); // Leggo il contenuto del file
			_body = buffer.str();
			// _body = "<html><body><h1>miao</h1></body></html>";
			set_status(STATUS_OK);
		}
	}
	else if (method == POST)
	{
		// Gestione della richiesta POST (es. scrittura su file)
		_body = "<html><body><h1>POST Request Handled</h1></body></html>";
		set_status(STATUS_OK);
	}
	else if (method == DELETE)
	{
		if (remove(path.c_str()) != 0)
		{ // Se il file non viene trovato
			set_status(STATUS_NOT_FOUND);
			_body = "<html><body><h1>404 Not Found</h1></body></html>";
		}
		else
		{
			set_status(STATUS_OK);
			_body = "<html><body><h1>File Deleted</h1></body></html>";
		}
	}
	else
	{
		// Metodo non consentito
		set_status(STATUS_METHOD_NOT_ALLOWED);
		_body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
	}

	// Imposta le intestazioni Content-Length e Content-Type
	std::ostringstream oss;
	oss << _body.length();
	set_header("Content-Length", oss.str());
	set_header("Content-Type", "text/html");
}