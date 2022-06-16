#ifndef BKEY_H
#define BKEY_H
#include <types.h>
#include <arch/i386/hyperhal/drv/pic8259a.h>

#ifdef __cplusplus
extern "C" char i386_getKey();
extern "C" char* i386_getKeyString();
extern "C" bool i386_kbd_init();
extern "C" void i386_reset_via_keyboard();
#else
char i386_getKey();
char* i386_getKeyString();
bool i386_kbd_init();
void i386_reset_via_keyboard();
#endif

#endif //  BKEY_H