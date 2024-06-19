#include "../include/Request.hpp"

Request::Request() {
    _method = 0;
    _path = "";
}

Request::~Request() {}

int Request::get_method() const { return _method; }

std::string &Request::get_path() { return _path; }

const std::string &Request::get_protocol() const { return _protocol; }

Request::iterator Request::get_header(const std::string &k) { return _headers.find(k); }

const std::string &Request::get_body() const { return _body; }

int Request::parseMethod(std::stringstream &buffer) {
    std::string buff;
    buffer >> buff;
    switch (buff.length()) {
    case 3:
        if (buff[0] == 'G' && buff[1] == 'E' && buff[2] == 'T')
            _method = GET;
        else if (buff[0] == 'P' && buff[1] == 'U' && buff[2] == 'T')
            _method = PUT;
        break;
    case 6:
        if (buff[0] == 'D' && buff[1] == 'E' && buff[2] == 'L' && buff[3] == 'E' &&
            buff[4] == 'T' && buff[5] == 'E')
            _method = DELETE;
        break;
    case 4:
        if (buff[0] == 'P' && buff[1] == 'O' && buff[2] == 'S' && buff[3] == 'T')
            _method = POST;
        break;
    default:
        return ERROR_GETTING_METHOD;
    }
    return (OK);
}

int Request::parsePath(std::stringstream &buffer) {
    std::string buff;
    buffer >> buff;
    if (buff[0] != '/')
        return ERROR_GETTING_PATH;
    size_t size = buff.size();
    if (buff[size - 1] == '/')
        buff.substr(0, size - 1);
    _path = buff;
    return OK;
}

int Request::parseProtocol(std::stringstream &buffer) {
    std::string buff;
    buffer >> buff;
    if (buff.size() < 5 || buff[0] != 'H' || buff[1] != 'T' || buff[2] != 'T' ||
        buff[3] != 'P' || buff[4] != '/')
        return ERROR_GETTING_PROTOCOL;
    _protocol = buff;
    return OK;
}

int Request::parseHeaders(std::stringstream &buffer) {
    std::string buff;
    std::string key;
    std::string value;
    int r = 0;
    size_t i;
    while (std::getline(buffer, buff)) {
        if ((buff.empty() || buff[0] == '\r') && r == 1)
            break;
        if (buff[0] == '\r' && r == 0) {
            r = 1;
            continue;
        }
        for (i = 0; i < buff.length(); i++) {
            if (buff[i] == ':' && buff[i + 1] == ' ') {
                break;
            }
            key.push_back(buff[i]);
        }
        if (i == buff.size())
            return ERROR_GETTING_HEADERS;
        buff.erase(0, i + 2);
        for (i = 0; i < buff.length(); i++)
            value.push_back(buff[i]);
        _headers[key] = value;
        key.clear();
        value.clear();
    }
    if (r == 0 || _headers.empty())
        return ERROR_GETTING_HEADERS;
    return OK;
}

int Request::parseBody(std::stringstream &buffer, int client_fd, int mbs) {
    if (_method == GET)
        return OK;

    Request::iterator it = _headers.find("content-length");
    if (it == _headers.end()) {
        it = _headers.find("Content-Length");
        if (it == _headers.end())
            return (ERROR_GETTING_BODY);
    }

    std::string str = buffer.str();
    size_t pos = str.find("\r\n\r\n");
    if (pos == std::string::npos)
        return CUSTOM; // wtf!!!! (should never happen)

    _body = str.substr(pos + 4);
    int read_body = _body.length();
    int content_len = std::atoi(it->second.c_str());
    int r, diff = content_len - read_body;
    if (content_len > mbs)
        return MAX_CLIENT_BODY_SIZE_EXCEEDED;
    if (read_body > content_len)
        return ERROR_READ_CONTENT_DIFF;
    if (read_body == content_len)
        return OK;

    char buff[BUFFER_SIZE + 1];
    while (diff > 0) {
        r = read(client_fd, buff, diff < BUFFER_SIZE ? diff : BUFFER_SIZE);
        if (r == -1) {
            continue;
        } else if (r == 0)
            break;
        buff[r] = 0;
        _body.append(buff, r);
        diff -= r;
    }
    return OK;
}

int Request::popRequest(char *buf, int client_fd, unsigned int mbs) {
    std::string buffer = buf;
    std::stringstream newBuffer(buffer);
    if (parseMethod(newBuffer) != OK)
        return (ERROR_GETTING_METHOD);
    if (parsePath(newBuffer) != OK)
        return (ERROR_GETTING_PATH);
    if (parseProtocol(newBuffer) != OK)
        return (ERROR_GETTING_PROTOCOL);
    if (parseHeaders(newBuffer) != OK)
        return (ERROR_GETTING_HEADERS);
    if (parseBody(newBuffer, client_fd, mbs) != OK)
        return (ERROR_GETTING_BODY);
    return OK;
}
