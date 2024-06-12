FROM ubuntu:latest

RUN apt-get -y update && apt-get -y --no-install-recommends install \
    build-essential \
    g++ \
    make

COPY . /webserv_proj

WORKDIR /webserv_proj

RUN make re

EXPOSE 8080
EXPOSE 8081

CMD ["./webserv", "configs/basic.conf", "-d"]
