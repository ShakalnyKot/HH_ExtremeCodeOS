#ifndef PRINTF_H
#define PRINTF_H
#include <stdarg.h>
#include <types.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#ifdef __cplusplus
extern "C" void lock_printf();
extern "C" void unlock_printf();

extern "C" int serial_printf(const char *format, ...);
#ifndef HEADLESS
extern "C" int printf (const char *format, ...);
extern "C" int kmsg(const char *format, ...);
#else
#define printf(format...) serial_printf(format)
#define kmsg(format...) serial_printf(format)
#endif
extern "C" void print_ok(const bool state, const char* format, ...);
#else
void lock_printf();
void unlock_printf();

int serial_printf(const char *format, ...);
#ifndef HEADLESS
int printf (const char *format, ...);
int kmsg(const char *format, ...);
#else
#define printf(format...) serial_printf(format)
#define kmsg(format...) serial_printf(format)
#endif
void print_ok(const bool state, const char* str);
#endif

#endif //  PRINTF_H