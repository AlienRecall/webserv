
# Webserv

This is when you finally understand why a URL starts
with HTTP.

This project is about writing your ow HTTP server.
You will be able to test it with an actual browser.
HTTP is one of the most used protocols on the internet.
Knowing its arcane will be useful, even if you won’t be working on a website.
## Prerequisites
As we choose to use epoll, our webserv requires a linux OS to work, so you can use docker or run this project locally if you have linux
 - [Docker](https://docs.docker.com/engine/install/)
 - Any linux port, we used ubuntu:latest docker image


## Installation

Clone this repository:
```bash
$ git clone https://github.com/AlienRecall/webserv.git
$ cd webserv
```

Build and start `webserv` using docker:
```bash
$ docker build -t webserv . && docker run -d --name webserv -p 8080 webserv && docker exec -it webserv /bin/bash
```

See logs via docker:
```bash
$ docker logs webserv -f
```

Run locally using make:
```bash
$ make
```

## Acknowledgements

 - [How epoll works](https://unscriptedcoding.medium.com/multithreaded-server-in-c-using-epoll-baadad32224c)
