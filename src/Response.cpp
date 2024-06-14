#include "../include/Response.hpp"
#include "../include/Request.hpp"
#include <sstream>
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
	return "Internal Server Error"; // Messaggio di default se il codice di stato non è riconosciuto
}

// Imposta il protocollo HTTP della risposta
void Response::set_protocol(const std::string &protocol)
{
	_protocol = protocol;
}

// Imposta il codice di stato della risposta
void Response::set_status(int status_code)
{
	// Usa std::stringstream per convertire status_code in stringa
	std::stringstream ss;
	ss << status_code;
	_status_code = ss.str(); // Converte il codice di stato in stringa

	_status = status_text(status_code); // Imposta il messaggio di stato basato sul codice di stato
}

// Imposta un'intestazione nella risposta
void Response::set_header(const std::string &key, const std::string &value)
{
	_headers[key] = value; // Imposta l'intestazione nella mappa dei headers
}

// Imposta il corpo della risposta
void Response::set_body(const std::string &body)
{
	_body = body; // Imposta il corpo della risposta
}



// Converte la risposta in una stringa C per essere inviata al client
char *Response::c_str() const
{
	std::ostringstream response;
	// Costruisce la linea di stato nella forma "HTTP/1.1 200 OK"
	response << _protocol << " " << _status_code << " " << _status << "\r\n";
	// Aggiunge tutte le intestazioni alla risposta
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		response << it->first << ": " << it->second << "\r\n";
	}
	response << "\r\n"; // Aggiunge una riga vuota tra le intestazioni e il corpo della risposta
	response << _body;	// Aggiunge il corpo della risposta

	std::string res_str = response.str(); // Converte stringstream in stringa C++
	char *res_cstr = new char[res_str.size() + 1];
	std::copy(res_str.begin(), res_str.end(), res_cstr); // Copia la stringa C++ nella stringa C
	res_cstr[res_str.size()] = '\0';
	return res_cstr; // Restituisce la stringa C
}

// Calcola la lunghezza della risposta completa
size_t Response::length() const
{
	std::ostringstream response;
	// Costruisce la linea di stato nella forma "HTTP/1.1 200 OK"
	response << _protocol << " " << _status_code << " " << _status << "\r\n";
	// Aggiunge tutte le intestazioni alla risposta
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		response << it->first << ": " << it->second << "\r\n";
	}
	response << "\r\n"
			 << _body;				// Aggiunge una riga vuota e il corpo della risposta
	return response.str().length(); // Restituisce la lunghezza della risposta completa
}








// Prepara la risposta HTTP in base alla richiesta
void Response::prepare_response(const Request &request)
{
	std::string method = request.get_method(); // Ottiene il metodo della richiesta
	std::string path = request.get_path();	   // Ottiene il percorso della richiesta

	// Gestione della richiesta GET
	if (method == "GET")
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
			buffer << file.rdbuf(); // Legge il contenuto del file
			_body = buffer.str();
			set_status(STATUS_OK);
		}
	}
	else if (method == "POST")
	{
		// Gestione della richiesta POST (es. scrittura su file, database, ecc.)
		_body = "<html><body><h1>POST Request Handled</h1></body></html>";
		set_status(STATUS_OK);
	}
	else if (method == "DELETE")
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
	set_header("Content-Length", std::to_string(_body.length()));
	set_header("Content-Type", "text/html");
}