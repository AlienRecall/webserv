#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include <map>
#include <string>
#include <utility>

#define STATUS_OK 200           // Definizione del codice di stato 200 (OK)
#define STATUS_FOUND 302        // Definizione del codice di stato 302 (Found)
#define STATUS_UNAUTHORIZED 401 // Definizione del codice di stato 401 (Unauthorized)
#define STATUS_FORBIDDEN 403    // Definizione del codice di stato 403 (Forbidden)

class Response
{
private:
  std::string _protocol;                       // Stringa per il protocollo HTTP
  std::string _status_code;                    // Stringa per il codice di stato
  std::string _status;                         // Stringa per il messaggio di stato
  std::map<std::string, std::string> _headers; // Mappa per gli headers della risposta
  std::string _body;                           // Stringa per il corpo della risposta

  std::string status_text(int); // Metodo per ottenere il messaggio di stato in base al codice

public:
  Response();  // Costruttore della classe Response
  ~Response(); // Distruttore della classe Response

  void prepare_response(const Request &request);
  void set_protocol(const std::string &protocol); // Metodo per impostare il protocollo HTTP
  void set_status(int);                                      // Metodo per impostare il codice di stato
  void set_header(const std::string &, const std::string &); // Metodo per impostare un'intestazione
  void set_body(const std::string &);                        // Metodo per impostare il corpo della risposta
  char *c_str() const;                                       // Metodo per ottenere la risposta come stringa C
  size_t length() const;
};

#endif
