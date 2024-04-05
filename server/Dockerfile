FROM ubuntu:devel
RUN apt update

RUN apt install -y clang g++ gcc
RUN apt install -y libfcgi libfcgi-dev
RUN apt install -y cmake ninja-build make
RUN apt install -y clang-tools

COPY ./src/ /app/src
COPY ./CMakeLists.txt /app
COPY ./Makefile /app

WORKDIR /app

ENV CXX=clang++
ENV CC=clang

RUN make build-debug

CMD "./flick"

