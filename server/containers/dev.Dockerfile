FROM ubuntu:devel
RUN apt update --fix-missing

RUN apt install -y clang-18 g++-14 gcc-14
RUN apt install -y libfcgi libfcgi-dev
RUN apt install -y libpq5 libpq-dev
RUN apt install -y cmake ninja-build make
RUN apt install -y clang-tools-18
RUN apt install -y pkg-config

WORKDIR /app

ENV CXX=clang++
ENV CC=clang
ENV TERM=xterm-256color
