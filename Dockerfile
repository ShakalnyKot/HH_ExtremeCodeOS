FROM debian

RUN apt update
RUN apt upgrade -y
RUN apt install xorriso mtools python3 nasm build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo grub2-common grub-pc-bin gcc-i686-linux-gnu -y

VOLUME /root/env
WORKDIR /root/env
