CROSS_PATH=toolchain/cross
CROSS_BIN=$(CROSS_PATH)/bin
ARCH=i386
TARGET=$(ARCH)-elf
WORDLENGTH=32

LINK_HH=i686-linux-gnu-ld -m elf_$(ARCH) -T src/arch/$(ARCH)/hyperhal/link.ld -o

PY=python3
CC=i686-linux-gnu-gcc
CPP=i686-linux-gnu-g++
ASM=nasm

CFLAGS=-O0 -Wall -Isrc/include -fno-builtin -ffreestanding -nostdinc -nostdlib -mgeneral-regs-only -c	# compiler arguments
CPPFLAGS=-O0 -Wall -Isrc/include -fno-builtin -ffreestanding -nostdinc -nostdlib -mgeneral-regs-only -fpermissive -c
AFLAGS=-f elf$(WORDLENGTH) -g -F dwarf