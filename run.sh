docker build -t webserv . && docker run --rm -d --name webserv -p 8080 webserv && docker exec -it webserv /bin/bash 
