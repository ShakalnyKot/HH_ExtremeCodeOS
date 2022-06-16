#pragma once
#include <types.h>
#ifdef HEADLESS
#include <hyperhal/arch_hal/hal_serial.h>
#endif

#ifdef __cplusplus
#ifndef HEADLESS
extern "C" char hal_kbd_getkey();
extern "C" char* hal_kbd_getstring();
extern "C" bool hal_kbd_init();
#else
#define hal_kbd_getkey() read_serial()
#define hal_kbd_getstring() hal_serial_getstring()
#define hal_kbd_init()
#endif
#else
#ifndef HEADLESS
char hal_kbd_getkey();
char* hal_kbd_getstring();
bool hal_kbd_init();
#else
#define hal_kbd_getkey() read_serial()
#define hal_kbd_getstring() hal_serial_getstring()
#define hal_kbd_init()
#endif
#endif