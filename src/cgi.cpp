#include "../include/cgi.hpp"

void handle_cgi_response(Request &req, int language)
{
	char *argv[3];
	std::string path = req.get_path();
	std::string cmd;

	if (language == PYTHON)
	{
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
	if (access(("/" + path).c_str(), R_OK | X_OK) != -1)
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
		if (pid == 0) //CHILD PROCESS
		{
			close(fd[0]); //unused
			if (dup2(fd[1], STDOUT_FILENO) == -1)
			{
				std::cout << ERROR_EXECVE << std::endl;
				exit(EXIT_FAILURE);
			}
			close(fd[1]); //unused
			execve(cmd.c_str(), argv, 0);
			std::cout << ERROR_EXECVE << std::endl;
			exit(EXIT_FAILURE);
		}
		else //parent process
		{
			close(fd[1]);
			char buffer[1];
			std::string output;
			int byteread;
			while (byteread = read(fd[0], buffer, sizeof(buffer)) > 0)
			{
				output.append(buffer, byteread);
			}
			close(fd[0]); //close dopo che ho finito di leggere
			int status;
			waitpid(pid, &status, 0);
			/* qui mandiamo l'output */
			return;
		}
	}
	else
	{
		std::cout << ERROR_FILE_NOT_ACCESS << std::endl;
		return;
	}
}