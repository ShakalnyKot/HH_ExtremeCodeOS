# neonOS/ExtremeCodeOS

Порт псевдотерминала, вытащенный из ExtremeCodeOS и вкрученный в neonOS HyperHAL 
версии 0.0.0.148

данная хуйня была сделена по фану потому что мне было нечего делать, не благодарите, я запилил её в рамках ExtremeCodeOS, и с моей neonOS она не связана ничем, кроме кодовой базы от неё

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
make isoimg - собрать ISO-образ
make all - собрать бинарники
make clean - подчистить говно, обязательно делайте это перед коммитами, в репе должны быть только исходники
```