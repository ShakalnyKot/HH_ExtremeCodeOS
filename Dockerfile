FROM debian

RUN apt update
RUN apt upgrade -y
RUN apt install -y gcc
RUN apt install -y g++
RUN apt install -y make
RUN apt install -y python3
RUN apt install -y xorriso
RUN apt install -y nasm
RUN apt install -y grub-pc-bin
RUN apt install -y grub-common

VOLUME /root/env
WORKDIR /root/env
