# HyperHAL/ExtremeCodeOS

![License](https://img.shields.io/github/license/ShakalnyKot/HH_ExtremeCodeOS?style=flat-square)
![Issules](https://img.shields.io/github/issues/ShakalnyKot/HH_ExtremeCodeOS?style=flat-square)
![Pull Requests](https://img.shields.io/github/issues-pr/ShakalnyKot/HH_ExtremeCodeOS?style=flat-square)
![Releases](https://img.shields.io/github/downloads/ShakalnyKot/HH_ExtremeCodeOS/latest/total?style=flat-square)

![Repo Size](https://img.shields.io/github/repo-size/ShakalnyKot/HH_ExtremeCodeOS?style=flat-square)
![Status](https://img.shields.io/github/workflow/status/ShakalnyKot/HH_ExtremeCodeOS/Build%20HHOS?style=flat-square)

Порт псевдотерминала, вытащенный из ExtremeCodeOS и вкрученный в HyperHAL 
версии 0.0.0.148

данная хуйня была сделена по фану потому что мне было нечего делать, не благодарите, я запилил её в рамках ExtremeCodeOS, и с моим поделием она не связана ничем, кроме кодовой базы от неё

зависимости для сборки
```
gcc - компилятор языков c / c++
make - для Makefile
xorriso - для сборки ISO-образа
mtools - зависимость xorriso
python3 - петухон, нужен для генерации некоторых файлов
nasm - нужен для компиляции файлов на языке Ассемблера для x86
```

Опции компиляции самого порта можно найти в src/include/types.h:

```
#define DEBUG - собираются отладочные куски кода
#define HEADLESS - собирается в Headless-режиме, в котором единственным способом коммуникации с системой является COM1-порт
```

цели сборки для make:
```
make cross-compiler - скомпилировать и установить кросс-компилятор (зависимости для сборки можно найти на https://wiki.osdev.org/GCC_Cross-Compiler)
make isoimg - собрать ISO-образ
make all - собрать бинарники
make clean - подчистить говно, обязательно делайте это перед коммитами, в репе должны быть только исходники
```