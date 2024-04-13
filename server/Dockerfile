FROM ubuntu:devel
RUN apt update --fix-missing

RUN apt install -y clang-18 g++-14 gcc-14
RUN apt install -y libfcgi libfcgi-dev
RUN apt install -y libpq5 libpq-dev
RUN apt install -y cmake ninja-build make
RUN apt install -y clang-tools-18
RUN apt install -y pkg-config

COPY ./deps/ /app/deps
COPY ./.env /app
COPY ./Makefile /app
COPY ./CMakeLists.txt /app
COPY ./src/ /app/src

WORKDIR /app

ENV CXX=clang++-18
ENV CC=clang-18

RUN --mount=type=cache,target="/app/build" \
    make build-debug

CMD "./flick"

