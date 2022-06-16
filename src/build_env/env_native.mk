CROSS_PATH=toolchain/cross
CROSS_BIN=$(CROSS_PATH)/bin
ARCH=i386
TARGET=$(ARCH)-elf
WORDLENGTH=32

LINK_HH=ld -m elf_$(ARCH) -T src/arch/$(ARCH)/hyperhal/link.ld -o
LINK_NK=ld -m elf_$(ARCH) -T src/arch/$(ARCH)/nk/link.ld -o


PY=python3
CC=gcc
CPP=g++
ASM=nasm

CFLAGS=-m32 -O0 -Wall -Isrc/include -fno-builtin -ffreestanding -nostdinc -nostdlib -mgeneral-regs-only -c	# compiler arguments
CPPFLAGS=-m32 -O0 -Wall -Isrc/include -fno-builtin -ffreestanding -nostdinc -nostdlib -mgeneral-regs-only -fpermissive -c
AFLAGS=-f elf$(WORDLENGTH) -g -F dwarf
