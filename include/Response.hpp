#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>
#include <utility>

#define STATUS_OK 200
#define STATUS_FOUND 302
#define STATUS_UNAUTHORIZED 401
#define STATUS_FORBIDDEN 403

class Response {
  private:
    std::string _protocol;
    std::string _status_code;
    std::string _status;
    std::map<std::string, std::string> _headers;
    std::string _body;

    std::string status_text(int);

  public:
    Response();
    ~Response();

    void set_protocol(double);
    void set_status(int);
    void set_header(const std::string &, const std::string &);
    void set_body(const std::string &);
    char *c_str() const;
};

#endif
