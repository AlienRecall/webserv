#pragma once

#define PYTHON 0
#define PHP 1
#define ERROR_OPEN_PIPE "error opening the pipe"
#define ERROR_FORK "forking error motherfuckers"
#define ERROR_EXECVE "execve, not working, not working!"
#define ERROR_FILE_NOT_ACCESS "file not accessible"

#include "request.hpp"
#include "unistd.h"

class cgiHandler
{
private:


public:
	cgiHandler();
	~cgiHandler();

	void cgi_response(Request &req);
};