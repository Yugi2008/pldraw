FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV ECE3574_REFERENCE_ENV="Y"

RUN apt-get update && apt-get upgrade
RUN apt-get -y install build-essential coreutils cmake
RUN apt-get -y install python3-pip python3-dev
RUN apt-get -y install valgrind lcov graphviz doxygen
RUN apt-get -y install clang clang-tidy
RUN apt-get -y install qt5-default
RUN apt-get -y install dos2unix

RUN pip3 install gcovr pexpect

RUN --mount=type=bind,source=.,target=/mnt
WORKDIR /mnt
